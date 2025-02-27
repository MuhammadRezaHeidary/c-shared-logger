add_library(SharedMemoryLogger STATIC src/SharedMemoryLogger.cpp)

target_include_directories(SharedMemoryLogger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
