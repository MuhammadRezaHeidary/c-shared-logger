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
        logger.info << "Football is the best of all sports!" << logger.endl;
        logger.warning << "When american guys are chasing and hitting each other, they think they are playing football!" << logger.endl;
        logger.error << "Messi is the best player!!!" << logger.endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
