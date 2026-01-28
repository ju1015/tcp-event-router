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

    std::string getFileName(EventType type) const;
};

#endif
