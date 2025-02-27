#include "SharedMemoryLogger.h"
#include <thread>
#include <chrono>

using namespace mekatronik::logger;

int main() {
    std::string shared_memory_name = "/shm_text_sports";
    std::string log_topic = "Sports";
    std::string log_file_name = "log_sports.txt";
    SharedMemoryLogger logger(shared_memory_name, log_topic, log_file_name);

    while(1) {
        logger.info << "Kung-Fu is a great chinese martial art." << logger.endl;
        logger.warning << "Kung-Fu might be dangerous for new trainees." << logger.endl;
        logger.error << "Kung-Fu is not in olympics!" << logger.endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
