cmake_minimum_required(VERSION 3.0.0)
project(trading_app VERSION 0.1.1.1)
configure_file(include/app.h include/app.h) #copy the header to binary directory
configure_file(include/version.h.in include/version.h)
configure_file(include/db.h include/db.h)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 1)
add_executable(main src/main.cpp)
add_subdirectory(src libs)
find_package(SQLite3 REQUIRED)
target_link_libraries(main app db ${SQLite3_LIBRARIES} -static)
target_include_directories(main PUBLIC "${PROJECT_BINARY_DIR}")
