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

	std::string filename = getFileName(event.type);

	rotateIfNeeded(filename);

	std::ofstream file(filename, std::ios::app);
	if (!file.is_open()) return;

	std::time_t now = std::time(nullptr);
	std::string ts = std::ctime(&now);
	ts.pop_back(); // remove newline

	file << "[" << ts << "] " << event.payload << "\n";
}

void FileAdapter::rotateIfNeeded(const std::string& filename) {

	if (!fs::exists(filename)) return;

	auto size = fs::file_size(filename);
	if (size < MAX_LOG_SIZE) return;

	// Delete oldest backup
	std::string oldest = filename + "." + std::to_string(MAX_BACKUPS);
	if (fs::exists(oldest)) {
		fs::remove(oldest);
	}

	// Shift backups
	for (int i = MAX_BACKUPS - 1; i >= 1; --i) {
		std::string src = filename + "." + std::to_string(i);
		std::string dst = filename + "." + std::to_string(i + 1);
		if (fs::exists(src)) {
			fs::rename(src, dst);
		}
	}

	// Rotate current file
	fs::rename(filename, filename + ".1");
}
