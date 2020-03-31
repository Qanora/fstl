include_directories(${PROJECT_SOURCE_DIR}/include)
set(APP_SRC vector.cpp)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_executable(stltest ${APP_SRC})