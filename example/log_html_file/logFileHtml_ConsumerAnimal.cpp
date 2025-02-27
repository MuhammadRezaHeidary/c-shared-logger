#include "SharedMemoryLogger.h"

using namespace mekatronik::logger;

int main() {
    std::string shared_memory_name = "/shm_html_animal";
    std::string log_topic = "Animal";
    std::string log_file_name = "log_animal.html";
    SharedMemoryLogger logger(shared_memory_name, log_topic, log_file_name, SharedMemoryLogger::LogFormat::HTML);
    
    logger.processLogs();
    return 0;
}
