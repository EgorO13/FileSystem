#include "path.h"
#include <sstream>

std::vector<std::string> Path::splitPath(const std::string& path) {
    std::vector<std::string> parts;
    if (path.empty() || path == "/") return parts;
    std::string normalized = path;
    if (normalized[0] == '/') normalized = normalized.substr(1);
    std::istringstream iss(normalized);
    std::string part;
    while (std::getline(iss, part, '/')) {
        if (!part.empty()) parts.push_back(part);
    }
    return parts;
}

std::string Path::normalizePath(const std::string& path) {
    if (path.empty()) return "/";
    std::vector<std::string> parts = splitPath(path);
    std::vector<std::string> normalizedParts;
    for (const auto& part : parts) {
        if (part == ".") continue;
        if (part == ".." && !normalizedParts.empty()) normalizedParts.pop_back();
        else normalizedParts.push_back(part);
    }
    std::string result;
    for (const auto& part : normalizedParts) {
        result += "/" + part;
    }
    return result.empty() ? "/" : result;
}

std::string Path::resolvePath(const std::string& basePath, const std::string& relativePath) {
    if (relativePath.empty()) return normalizePath(basePath);
    if (relativePath[0] == '/') return normalizePath(relativePath);
    std::string combined;
    if (basePath == "/") combined = "/" + relativePath;
    else combined = basePath + "/" + relativePath;
    return normalizePath(combined);
}

std::string Path::getParentPath(const std::string& path) {
    std::string normalized = normalizePath(path);
    if (normalized == "/") return "/";
    size_t lastSlash = normalized.find_last_of('/');
    if (lastSlash == 0) return "/";
    return normalized.substr(0, lastSlash);
}

std::string Path::getFileName(const std::string& path) {
    std::string normalized = Path::normalizePath(path);
    if (normalized == "/") return "/";
    size_t lastSlash = normalized.find_last_of('/');
    return normalized.substr(lastSlash + 1);
}

bool Path::isValidPath(const std::string& path) {
    if (path.empty()) return false;
    if (path.find('\0') != std::string::npos) return false;
    if (path[0] != '/') return false;
    return true;
}

bool Path::matchesPattern(const std::string& name, const std::string& pattern) {
    if (pattern == "*") return true;
    if (pattern.find('?') == std::string::npos && pattern.find('*') == std::string::npos) return name == pattern;
    size_t nameIdx = 0, patternIdx = 0;
    size_t starIdx = std::string::npos;
    size_t matchIdx = 0;
    while (nameIdx < name.length()) {
        if (patternIdx < pattern.length() &&
            (pattern[patternIdx] == '?' || pattern[patternIdx] == name[nameIdx])) {
            nameIdx++;
            patternIdx++;
            }
        else if (patternIdx < pattern.length() && pattern[patternIdx] == '*') {
            starIdx = patternIdx;
            matchIdx = nameIdx;
            patternIdx++;
        }
        else if (starIdx != std::string::npos) {
            patternIdx = starIdx + 1;
            matchIdx++;
            nameIdx = matchIdx;
        }
        else return false;
    }
    while (patternIdx < pattern.length() && pattern[patternIdx] == '*') {
        patternIdx++;
    }
    return patternIdx == pattern.length();
}