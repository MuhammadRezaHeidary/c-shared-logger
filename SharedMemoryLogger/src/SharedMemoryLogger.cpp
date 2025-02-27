#include "SharedMemoryLogger.h"

namespace mekatronik::logger {

const char* SharedMemoryLogger::endl = "\n";

SharedMemoryLogger::SharedMemoryLogger(const std::string& shmName, const std::string& producer, const std::string& logFile, LogFormat format)
    : shmName(shmName), producer(producer), logToFile(true), logFileName(logFile), os(std::cout), running(true), logFormat(format),
      info(*this, LogLevel::info), warning(*this, LogLevel::warning), error(*this, LogLevel::error) {

    logToFile = (logFile != "terminal");

    if (logToFile) {
        std::string extension = (logFormat == LogFormat::HTML) ? ".html" :
                                (logFormat == LogFormat::JSON) ? ".json" : ".txt";
        if (logFileName.find('.') == std::string::npos) {
            logFileName += extension;
        }
    }

    if (logToFile) {
        static std::ofstream logFile(logFileName, std::ios::app);
        os.rdbuf(logFile.rdbuf());
    }

    shm_fd = shm_open(shmName.c_str(), O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedMemory));
    shm = static_cast<SharedMemory*>(mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    
    sem_init(&shm->mutex, 1, 1);
    shm->write_pos = 0;
    shm->read_pos = 0;
}

SharedMemoryLogger::SharedMemoryLogger(const std::string& producer, std::ostream& outputStream)
    : shmName("terminal"), producer(producer), logToFile(false), os(outputStream),
      running(true), info(*this, LogLevel::info), warning(*this, LogLevel::warning), error(*this, LogLevel::error) {
    // No shared memory needed for terminal logging
}

SharedMemoryLogger::~SharedMemoryLogger() {
    if (logToFile) {
        running = false;
        sem_destroy(&shm->mutex);
        munmap(shm, sizeof(SharedMemory));
        close(shm_fd);
        shm_unlink(shmName.c_str());
    }
}

void SharedMemoryLogger::logMessage(LogLevel level, const std::string& message) {
    std::string formattedMessage;
    std::string timeStr = "[" + getCurrentTime() + "]";
    std::string levelStr = "[" + logLevelToString(level) + "]";
    std::string producerStr = "[" + producer + "]";

    if (logToFile) {
        if (logFormat == LogFormat::TEXT) {
            formattedMessage = timeStr + " " + levelStr + " " + producerStr + " " + message;
        } else if (logFormat == LogFormat::HTML) {
            if (!sentStyleHtml) {
                sentStyleHtml = true;
                std::string style = "<meta http-equiv=\"refresh\" content=\"5\">"
                                    "<style>"
                                    "table { width: 100%; border-collapse: collapse; table-layout: fixed; }"
                                    "td { border: 1px solid #000; padding: 8px; text-align: center; word-wrap: break-word; overflow-wrap: break-word; }"
                                    "td:nth-child(1) { width: 15%; }"
                                    "td:nth-child(2) { width: 15%; }"
                                    "td:nth-child(3) { width: 15%; }"
                                    "td:nth-child(4) { width: 55%; }"
                                    "</style>";
                formattedMessage = style + "<table>"
                                           "<tr>"
                                           "<td>" + timeStr + "</td>"
                                           "<td>" + levelStr + "</td>"
                                           "<td>" + producerStr + "</td>"
                                           "<td>" + message + "</td>"
                                           "</tr>"
                                           "</table>";
            } else {
                formattedMessage = "<table>"
                                   "<tr>"
                                   "<td>" + timeStr + "</td>"
                                   "<td>" + levelStr + "</td>"
                                   "<td>" + producerStr + "</td>"
                                   "<td>" + message + "</td>"
                                   "</tr>"
                                   "</table>";
            }            
         } else if (logFormat == LogFormat::JSON) {
            formattedMessage = "{ \"time\": \"" + getCurrentTime() + "\", \"level\": \"" + logLevelToString(level) +
                               "\", \"producer\": \"" + producer + "\", \"message\": \"" + message + "\" }";
        }
    
        sem_wait(&shm->mutex);
        strncpy(shm->messages[shm->write_pos], formattedMessage.c_str(), MEKATRONIK_LOGGER_MESSAGE_SIZE);
        shm->write_pos = (shm->write_pos + 1) % MEKATRONIK_LOGGER_MAX_MESSAGES;
        sem_post(&shm->mutex);
    } else {
        formattedMessage = timeStr + " " + levelStr + " " + producerStr + " " + message;
        os << formattedMessage << SharedMemoryLogger::endl;
    }
}

void SharedMemoryLogger::processLogs() {
    if (!logToFile) return;

    std::ofstream logfile(logFileName, std::ios::app);
    while (running.load()) {
        sem_wait(&shm->mutex);

        if (shm->read_pos != shm->write_pos) {
            logfile << shm->messages[shm->read_pos] << SharedMemoryLogger::endl;
            logfile.flush();
            shm->read_pos = (shm->read_pos + 1) % MEKATRONIK_LOGGER_MAX_MESSAGES;
        }

        sem_post(&shm->mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void SharedMemoryLogger::stopProcessing() {
    running.store(false);
}

std::string SharedMemoryLogger::getCurrentTime() {
    time_t now = time(nullptr);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

std::string SharedMemoryLogger::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::info: return "info";
        case LogLevel::warning: return "warning";
        case LogLevel::error: return "error";
    }
    return "unknown";
}

SharedMemoryLogger::LogStream::LogStream(SharedMemoryLogger& logger, LogLevel level) : logger(logger), level(level) {}

template <typename T>
SharedMemoryLogger::LogStream& SharedMemoryLogger::LogStream::operator<<(const T& value) {
    stream << value;
    return *this;
}

SharedMemoryLogger::LogStream& SharedMemoryLogger::LogStream::operator<<(const char* value) {
    if (value == SharedMemoryLogger::endl) {
        logger.logMessage(level, stream.str());
        stream.str("");
        stream.clear();
    } else {
        stream << value;
    }
    return *this;
}

SharedMemoryLogger::LogStream::~LogStream() {
    if (!stream.str().empty()) {
        logger.logMessage(level, stream.str());
    }
}

} // namespace mekatronik::logger
