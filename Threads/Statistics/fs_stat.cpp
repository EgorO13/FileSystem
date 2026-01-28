#include "fs_stat.h"
#include "Entity/File/interface/i_lockable.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>

namespace {
    bool checkAccess(IFileSystemObject* obj, const User* currentUser, const std::vector<unsigned int>& userGroups, bool ignorePermissions) {
        if (!obj) return false;
        if (ignorePermissions) return true;
        if (!currentUser) return false;
        return obj->checkPermission(currentUser->getId(), userGroups, PermissionType::Read);
    }

    bool checkFileLock(IFileSystemObject* obj, bool ignorePermissions) {
        if (!obj || ignorePermissions) return true;
        if (ILockable* lockable = dynamic_cast<ILockable*>(obj)) return lockable->isReadable();
        return true;
    }
}

void FileSystemScanner::scanDirectory(IDirectory* directory, Context& context) {
    ProcessingContext procContext(repository, mapper, currentUser, userGroups, ignorePermissions);
    std::vector<IDirectory*> subdirectories;
    std::vector<IFileSystemObject*> files;

    auto allChildren = directory->listChild();
    for (auto* child : allChildren) {
        if (!checkAccess(child, currentUser, userGroups, ignorePermissions)) continue;
        if (IDirectory* dir = dynamic_cast<IDirectory*>(child)) subdirectories.push_back(dir);
        else if (checkFileLock(child, ignorePermissions)) files.push_back(child);
    }

    if (!files.empty()) context.processObjectGroup(files, procContext);
    context.processObject(dynamic_cast<IFileSystemObject*>(directory), procContext);
    if (subdirectories.empty()) return;

    int currentThreads = context.getCurrentThreadCount();
    int availableSlots = context.getMaxThreads() - currentThreads;
    int threadsToCreate = std::min(availableSlots, static_cast<int>(subdirectories.size()) - 1);

    if (threadsToCreate > 0) {
        context.incrementThreadCounter(threadsToCreate);
        std::vector<std::future<std::unique_ptr<Context>>> futures;

        for (int i = 0; i < threadsToCreate; ++i) {
            IDirectory* subdir = subdirectories[i];
            std::vector<std::unique_ptr<IMetric>> localTemplates;
            {
                std::lock_guard<std::mutex> lock(templateMetricsMutex);
                for (const auto& m : templateMetrics) {
                    localTemplates.push_back(m->createEmptyClone());
                }
            }
            auto future = std::async(std::launch::async,
                [this, subdir,
                 threadCounterRef = std::ref(this->activeThreadCounter),
                 maxThreads = this->maxThreads,
                 templates = std::move(localTemplates)]() mutable {
                    auto childContext = std::make_unique<Context>(
                        threadCounterRef.get(),
                        maxThreads,
                        templates
                    );
                    this->scanDirectory(subdir, *childContext);
                    return childContext;
                });
            futures.push_back(std::move(future));
        }
        for (size_t i = threadsToCreate; i < subdirectories.size(); ++i) {
            scanDirectory(subdirectories[i], context);
        }
        for (auto& future : futures) {
            auto childContext = future.get();
            context.mergeFromChild(*childContext);
        }
        context.decrementThreadCounter(threadsToCreate);
    } else {
        for (auto* subdir : subdirectories) {
            scanDirectory(subdir, context);
        }
    }
}

std::vector<std::vector<std::string>> FileSystemScanner::scan(IDirectory* rootDirectory, const std::vector<std::unique_ptr<IMetric>>& metrics) {
    {
        std::lock_guard<std::mutex> lock(templateMetricsMutex);
        templateMetrics.clear();
        for (const auto& metric : metrics) {
            templateMetrics.push_back(metric->createEmptyClone());
        }
    }
    activeThreadCounter.store(0, std::memory_order_release);
    Context rootContext(activeThreadCounter, maxThreads, templateMetrics);
    activeThreadCounter.fetch_add(1, std::memory_order_acq_rel);
    scanDirectory(rootDirectory, rootContext);
    activeThreadCounter.fetch_sub(1, std::memory_order_acq_rel);
    return rootContext.getResults();
}