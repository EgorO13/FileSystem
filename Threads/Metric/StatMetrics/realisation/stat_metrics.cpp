#include "stat_metrics.h"

std::string SizeMetric::getName() const { return "Size Statistics"; }

void SizeMetric::process(IFileSystemObject* obj, const ProcessingContext& context) {
    if (!obj) return;
    auto* file = dynamic_cast<IFile*>(obj);
    if (!file) return;
    unsigned int fileSize = file->getSize();
    totalSize += fileSize;
    fileCount++;
    if (fileSize > largestFileSize) {
        largestFileSize = fileSize;
        largestFilePath = context.getPath(obj);
    }
}

void SizeMetric::processGroup(const std::vector<IFileSystemObject*>& objects, const ProcessingContext& context) {
    for (auto* obj : objects) {
        process(obj, context);
    }
}

std::vector<std::string> SizeMetric::getResults() const {
    std::vector<std::string> results;
    results.push_back("=== Size Statistics ===");
    if (fileCount == 0) {
        results.push_back("No files found");
        return results;
    }
    std::ostringstream oss;
    oss << "Total size: " << totalSize << " bytes";
    results.push_back(oss.str());
    oss.str("");
    oss.clear();
    double avgSize = static_cast<double>(totalSize) / fileCount;
    oss << "Average file size: " << std::fixed << std::setprecision(2) << avgSize << " bytes";
    results.push_back(oss.str());
    results.push_back("Files processed: " + std::to_string(fileCount));
    if (largestFileSize > 0) {
        oss.str("");
        oss.clear();
        oss << "Largest file: " << largestFilePath << " (" << largestFileSize << " bytes)";
        results.push_back(oss.str());
    }
    return results;
}

void SizeMetric::reset() {
    totalSize = 0;
    largestFileSize = 0;
    largestFilePath.clear();
    fileCount = 0;
}

std::unique_ptr<IMetric> SizeMetric::createEmptyClone() const {
    return std::make_unique<SizeMetric>();
}

void SizeMetric::mergeFrom(const IMetric& other) {
    const SizeMetric* otherMetric = dynamic_cast<const SizeMetric*>(&other);
    if (!otherMetric) return;
    totalSize += otherMetric->totalSize;
    fileCount += otherMetric->fileCount;
    if (otherMetric->largestFileSize > largestFileSize) {
        largestFileSize = otherMetric->largestFileSize;
        largestFilePath = otherMetric->largestFilePath;
    }
}


std::string OwnerMetric::getName() const { return "Owner Statistics"; }

void OwnerMetric::process(IFileSystemObject* obj, const ProcessingContext& context) {
    if (!obj) return;
    auto ownerName = obj->getOwner().getName();
    ownerStats[ownerName]++;
    totalObjects++;
}

void OwnerMetric::processGroup(const std::vector<IFileSystemObject*>& objects, const ProcessingContext& context) {
    for (auto* obj : objects) {
        process(obj, context);
    }
}

std::vector<std::string> OwnerMetric::getResults() const {
    std::vector<std::string> results;
    results.push_back("=== Owner Statistics ===");
    if (ownerStats.empty()) {
        results.push_back("No objects found");
        return results;
    }
    std::vector<std::pair<std::string, unsigned int>> sortedOwners(ownerStats.begin(), ownerStats.end());
    std::sort(sortedOwners.begin(), sortedOwners.end(),[](const auto& a, const auto& b) { return a.second > b.second; });
    size_t limit = std::min<size_t>(5, sortedOwners.size());
    results.push_back("Top " + std::to_string(limit) + " owners:");
    for (size_t i = 0; i < limit; ++i) {
        std::ostringstream oss;
        double percentage = (totalObjects > 0) ? (100.0 * sortedOwners[i].second / totalObjects) : 0.0;
        oss << sortedOwners[i].first << ": " << sortedOwners[i].second << " (" << std::fixed << std::setprecision(2) << percentage << "%)";
        results.push_back(oss.str());
    }
    results.push_back("Total objects: " + std::to_string(totalObjects));
    results.push_back("Unique owners: " + std::to_string(ownerStats.size()));
    return results;
}

void OwnerMetric::reset() {
    ownerStats.clear();
    totalObjects = 0;
}

std::unique_ptr<IMetric> OwnerMetric::createEmptyClone() const {
    return std::make_unique<OwnerMetric>();
}

void OwnerMetric::mergeFrom(const IMetric& other) {
    const OwnerMetric* otherMetric = dynamic_cast<const OwnerMetric*>(&other);
    if (!otherMetric) return;
    for (const auto& [owner, count] : otherMetric->ownerStats) {
        ownerStats[owner] += count;
    }
    totalObjects += otherMetric->totalObjects;
}

std::string TypeCounterMetric::getName() const { return "Type Statistics"; }

void TypeCounterMetric::TypeCounterMetric::process(IFileSystemObject* obj, const ProcessingContext& context) {
    if (!obj) return;
    auto dto = context.mapper.mapTo(*obj);
    typeCounts[dto.type]++;
    totalObjects++;
}

void TypeCounterMetric::processGroup(const std::vector<IFileSystemObject*>& objects, const ProcessingContext& context) {
    for (auto* obj : objects) {
        process(obj, context);
    }
}

void TypeCounterMetric::mergeFrom(const IMetric& other) {
    const TypeCounterMetric* otherMetric = dynamic_cast<const TypeCounterMetric*>(&other);
    if (!otherMetric) return;
    for (const auto& [type, count] : otherMetric->typeCounts) {
        typeCounts[type] += count;
    }
    totalObjects += otherMetric->totalObjects;
}

std::vector<std::string> TypeCounterMetric::getResults() const {
    std::vector<std::string> results;
    results.push_back("=== Type Statistics ===");
    if (typeCounts.empty()) {
        results.push_back("No objects found");
        return results;
    }
    for (const auto& [type, count] : typeCounts) {
        std::ostringstream oss;
        double percentage = (totalObjects > 0) ? (100.0 * count / totalObjects) : 0.0;
        oss << type << ": " << count << " (" << std::fixed << std::setprecision(2) << percentage << "%)";
        results.push_back(oss.str());
    }
    results.push_back("Total objects: " + std::to_string(totalObjects));
    return results;
}

std::unique_ptr<IMetric> TypeCounterMetric::createEmptyClone() const {
    return std::make_unique<TypeCounterMetric>();
}

void TypeCounterMetric::reset() {
    typeCounts.clear();
    totalObjects = 0;
}