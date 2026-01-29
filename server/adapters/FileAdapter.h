#ifndef FILE_ADAPTER_H
#define FILE_ADAPTER_H

#include "../events/Event.h"
#include <string>

class FileAdapter {
public:
    explicit FileAdapter(const std::string& baseDir = "logs");

    void write(const Event& event);

private:
    std::string baseDirectory;

    static constexpr std::size_t MAX_LOG_SIZE = 1024 * 1024; // 1MB
    static constexpr int MAX_BACKUPS = 3;

    std::string getFileName(EventType type) const;
    void rotateIfNeeded(const std::string& filename);
};

#endif
