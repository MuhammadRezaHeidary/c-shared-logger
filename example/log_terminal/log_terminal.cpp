#include "SharedMemoryLogger.h"
#include <thread>
#include <chrono>

using namespace mekatronik::logger;

int main() {
    SharedMemoryLogger logger("Terminal Logger");

    while(true) {
        logger.info << "This is some information about logs!" << logger.endl;
        logger.warning << "There is no room!" << logger.endl;
        logger.error << "Memory is full!" << logger.endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
