add_library(app STATIC app.cpp)
target_include_directories(app PRIVATE "${PROJECT_BINARY_DIR}")
