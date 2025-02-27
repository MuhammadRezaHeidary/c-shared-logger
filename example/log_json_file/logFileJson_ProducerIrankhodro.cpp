#include "SharedMemoryLogger.h"
#include <thread>
#include <chrono>

using namespace mekatronik::logger;

int main() {
    std::string shared_memory_name = "/shm_json_car";
    std::string log_topic = "Car::Irankhodro";
    std::string log_file_name = "log_car.json";
    SharedMemoryLogger logger(shared_memory_name, log_topic, log_file_name, SharedMemoryLogger::LogFormat::JSON);

    while(1) {
        logger.info << "Irankhordo company is a great company!" << logger.endl;
        logger.warning << "Irankhodro is selling its cars..." << logger.endl;
        logger.error << "Price of Irankhodro cars are rising." << logger.endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
