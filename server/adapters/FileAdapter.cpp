#include "FileAdapter.h"
#include <fstream>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;

FileAdapter::FileAdapter(const std::string& baseDir)
    : baseDirectory(baseDir) {

    // Ensure log directory exists
    fs::create_directories(baseDirectory);
}

std::string FileAdapter::getFileName(EventType type) const {
    switch (type) {
        case EventType::ALERT:
            return baseDirectory + "/alerts.log";
        case EventType::METRIC:
            return baseDirectory + "/metrics.log";
        case EventType::SECURITY:
            return baseDirectory + "/security.log";
        case EventType::INFO:
        default:
            return baseDirectory + "/info.log";
    }
}

void FileAdapter::write(const Event& event) {

    std::ofstream file(getFileName(event.type), std::ios::app);
    if (!file.is_open()) return;

    std::time_t now = std::time(nullptr);

    file << "[" << std::ctime(&now) << "] "
         << event.payload << "\n";
}
