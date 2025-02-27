#ifndef MEKATRONIK_LOGGER_SHARED_MEMORY_LOGGER_H
#define MEKATRONIK_LOGGER_SHARED_MEMORY_LOGGER_H

#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <atomic>
#include <thread>

#define MEKATRONIK_LOGGER_MAX_MESSAGES 10
#define MEKATRONIK_LOGGER_MESSAGE_SIZE 1024*1024 // Memory size = 1 MByte

namespace mekatronik::logger {

class SharedMemoryLogger {
public:
    enum class LogLevel { info, warning, error };
    enum class LogFormat { TEXT, HTML, JSON };

    static const char* endl;

    SharedMemoryLogger(const std::string& shmName, const std::string& producer, const std::string& logFile, LogFormat format = LogFormat::TEXT);
    SharedMemoryLogger(const std::string& producer, std::ostream& outputStream = std::cout);
    ~SharedMemoryLogger();

    class LogStream {
    public:
        LogStream(SharedMemoryLogger& logger, LogLevel level);
        template <typename T>
        LogStream& operator<<(const T& value);
        LogStream& operator<<(const char* value);
        LogStream& operator<<(std::ostream& (*manip)(std::ostream&));
        ~LogStream();

    private:
        SharedMemoryLogger& logger;
        LogLevel level;
        std::ostringstream stream;
    };

    LogStream info;
    LogStream warning;
    LogStream error;

    void processLogs();
    void stopProcessing();  

private:
    struct SharedMemory {
        sem_t mutex;
        int write_pos;
        int read_pos;
        char messages[MEKATRONIK_LOGGER_MAX_MESSAGES][MEKATRONIK_LOGGER_MESSAGE_SIZE];
    };

    std::string shmName;
    std::string producer;
    int shm_fd;
    SharedMemory* shm;
    bool logToFile;
    bool sentStyleHtml {false};
    std::string logFileName;
    std::ostream& os;
    std::atomic<bool> running;
    LogFormat logFormat;

    void logMessage(LogLevel level, const std::string& message);
    std::string getCurrentTime();
    std::string logLevelToString(LogLevel level);
};

} // namespace mekatronik::logger

#endif  
