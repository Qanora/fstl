include_directories(${PROJECT_SOURCE_DIR}/include)
set(APP_SRC shared_ptr.cpp)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_executable(stltest ${APP_SRC})