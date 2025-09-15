cmake_minimum_required(VERSION 4.1)

project(sensor-lib LANGUAGES C)

add_library(sensor-lib STATIC
            src/sensor_data.c
            src/date_time.c
            include/sensor_data.h
            include/date_time.h)

target_include_directories(sensor-lib PUBLIC include)

add_executable(test src/test.c)
target_link_libraries(test PRIVATE sensor-lib)

if(CMAKE_SYSTEM_NAME MATCHES "Generic")
    message(STATUS "Building for embedded system")
else()
    add_library(sensor-lib-shared SHARED src/sensor_data.c)
    target_include_directories(sensor-lib-shared PUBLIC include)
    set_target_properties(sensor-lib-shared PROPERTIES OUTPUT_NAME "sensor_native")

    if(WIN32)
        set_target_properties(sensor-lib-shared PROPERTIES PREFIX "")
    endif()
    message(STATUS "Building for host system")
endif()
