cmake_minimum_required(VERSION 4.1)

project(sensor-lib LANGUAGES C)

add_library(sensor-lib STATIC
            src/sensor_data.c
            src/date_time.c
            src/gsm7.c
            include/sensor_data.h
            include/date_time.h
            include/gsm7_table.h
            include/gsm7_table.h)

target_include_directories(sensor-lib PUBLIC include)

# add_executable(test_meas src/test.c)
# target_link_libraries(test_meas PRIVATE sensor-lib)
# add_executable(test_dt src/test1.c)
# target_link_libraries(test_dt PRIVATE sensor-lib)
# add_executable(test_hex src/test_hex.c)
# target_link_libraries(test_hex PRIVATE sensor-lib)

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
