cmake_minimum_required(VERSION 3.0.0)
project(trading_app VERSION 0.0.1)
configure_file(include/app.h include/app.h) #copy the header to binary directory
set(CMAKE_CXX_STANDARD 11)
add_executable(main src/main.cpp)
add_subdirectory(src libs)
find_package(SQLite3 REQUIRED)
target_link_libraries(main app ${SQLite3_LIBRARIES} -static)
target_include_directories(main PUBLIC "${PROJECT_BINARY_DIR}")
