#include "acl_serializer.h"
#include <sstream>
#include <algorithm>

std::string ACLSerializer::serialize(const std::vector<ACLEntry>& acl) {
    if (acl.empty()) return "";
    std::stringstream ss;
    bool firstEntry = true;
    for (const auto& entry : acl) {
        if (!firstEntry) ss << ";";
        firstEntry = false;
        ss << entry.subjectId << ":" << subjectTypeToString(entry.subjectType) << ":";
        bool firstPerm = true;
        for (const auto& [perm, effect] : entry.permissions) {
            if (!firstPerm) ss << ",";
            firstPerm = false;
            ss << permissionTypeToString(perm) << "=" << permissionEffectToString(effect);
        }
    }
    return ss.str();
}

std::vector<ACLEntry> ACLSerializer::deserialize(const std::string& str) {
    std::vector<ACLEntry> result;
    if (str.empty()) return result;
    std::stringstream ss(str);
    std::string entryStr;
    while (std::getline(ss, entryStr, ';')) {
        if (entryStr.empty()) continue;
        std::stringstream entryStream(entryStr);
        std::string subjectIdStr, subjectTypeStr, permsStr;
        std::getline(entryStream, subjectIdStr, ':');
        std::getline(entryStream, subjectTypeStr, ':');
        std::getline(entryStream, permsStr);
        ACLEntry entry;
        entry.subjectId = std::stoul(subjectIdStr);
        entry.subjectType = stringToSubjectType(subjectTypeStr);
        if (!permsStr.empty()) {
            std::stringstream permsStream(permsStr);
            std::string permEntry;
            while (std::getline(permsStream, permEntry, ',')) {
                size_t eqPos = permEntry.find('=');
                if (eqPos != std::string::npos) {
                    std::string permStr = permEntry.substr(0, eqPos);
                    std::string effectStr = permEntry.substr(eqPos + 1);

                    PermissionType perm = stringToPermissionType(permStr);
                    PermissionEffect effect = stringToPermissionEffect(effectStr);

                    entry.permissions[perm] = effect;
                }
            }
        }
        result.push_back(entry);
    }
    return result;
}

std::string ACLSerializer::subjectTypeToString(SubjectType type) {
    switch (type) {
        case SubjectType::User: return "USER";
        case SubjectType::Group: return "GROUP";
        default: return "UNKNOWN";
    }
}

SubjectType ACLSerializer::stringToSubjectType(const std::string& str) {
    if (str == "USER") return SubjectType::User;
    if (str == "GROUP") return SubjectType::Group;
    return SubjectType::User; // или бросить исключение
}

std::string ACLSerializer::permissionTypeToString(PermissionType type) {
    switch (type) {
        case PermissionType::Read : return "Read";
        case PermissionType::Write: return "Write";
        case PermissionType::Execute: return "Execute";
        case PermissionType::Modify: return "Modify";
        case PermissionType::ModifyMetadata: return "ModifyMetadata";
        case PermissionType::ChangePermissions: return "ChangePermissions";
        default: return "UNKNOWN";
    }
}

PermissionType ACLSerializer::stringToPermissionType(const std::string& str) {
    if (str == "Read") return PermissionType::Read;
    if (str == "Write") return PermissionType::Write;
    if (str == "Execute") return PermissionType::Execute;
    if (str == "Modify") return PermissionType::Modify;
    if (str == "ModifyMetadata") return PermissionType::ModifyMetadata;
    if (str == "ChangePermissions") return PermissionType::ChangePermissions;
    return PermissionType::Read;
}

std::string ACLSerializer::permissionEffectToString(PermissionEffect effect) {
    switch (effect) {
        case PermissionEffect::Allow: return "Allow";
        case PermissionEffect::Deny: return "Deny";
        default: return "UNKNOWN";
    }
}

PermissionEffect ACLSerializer::stringToPermissionEffect(const std::string& str) {
    if (str == "Allow") return PermissionEffect::Allow;
    if (str == "Deny") return PermissionEffect::Deny;
    return PermissionEffect::Deny;
}