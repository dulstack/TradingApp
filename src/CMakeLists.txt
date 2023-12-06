add_library(app STATIC app.cpp)
add_library(db STATIC db.cpp)
target_include_directories(app PRIVATE "${PROJECT_BINARY_DIR}")
