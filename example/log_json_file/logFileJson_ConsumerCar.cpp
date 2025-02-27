#include "SharedMemoryLogger.h"

using namespace mekatronik::logger;

int main() {
    std::string shared_memory_name = "/shm_json_car";
    std::string log_topic = "Car";
    std::string log_file_name = "log_car.json";
    SharedMemoryLogger logger(shared_memory_name, log_topic, log_file_name, SharedMemoryLogger::LogFormat::JSON);
    
    logger.processLogs();
    return 0;
}
