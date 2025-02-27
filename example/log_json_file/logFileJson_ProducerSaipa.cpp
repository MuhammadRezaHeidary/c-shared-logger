#include "SharedMemoryLogger.h"
#include <thread>
#include <chrono>

using namespace mekatronik::logger;

int main() {
    std::string shared_memory_name = "/shm_json_car";
    std::string log_topic = "Car";
    std::string log_file_name = "log_car.json";
    SharedMemoryLogger logger(shared_memory_name, log_topic, log_file_name, SharedMemoryLogger::LogFormat::JSON);

    while(1) {
        logger.info << "Saipa is selling the Saipa Pride car!" << logger.endl;
        logger.warning << "Saipa Pride car is a low cost car..." << logger.endl;
        logger.error << "Saipa Pride car is very expensive" << logger.endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
