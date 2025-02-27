#include "SharedMemoryLogger.h"

using namespace mekatronik::logger;

int main() {
    std::string shared_memory_name = "/shm_text_sports";
    std::string log_topic = "Sports";
    std::string log_file_name = "log_sports.txt";
    SharedMemoryLogger logger(shared_memory_name, log_topic, log_file_name);
    
    logger.processLogs();
    return 0;
}
