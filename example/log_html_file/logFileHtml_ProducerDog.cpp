#include "SharedMemoryLogger.h"
#include <thread>
#include <chrono>

using namespace mekatronik::logger;

int main() {
    std::string shared_memory_name = "/shm_html_animal";
    std::string log_topic = "Animal::Dog";
    std::string log_file_name = "log_animal.html";
    SharedMemoryLogger logger(shared_memory_name, log_topic, log_file_name, SharedMemoryLogger::LogFormat::HTML);

    while(1) {
        logger.info << "The dog is saying bow-wow!" << logger.endl;
        logger.warning << "The dog is eating. Go away!" << logger.endl;
        logger.error << "The dog has saw a thief. Call the police!" << logger.endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
