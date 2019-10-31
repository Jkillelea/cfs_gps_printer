cmake_minimum_required(VERSION 2.6.4)
project(GPS_PRINTER C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)
include_directories(${gps_reader_MISSION_DIR}/fsw/src/libnmea/include)
include_directories(${gps_reader_MISSION_DIR}/fsw/platform_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module
add_cfe_app(gps_printer ${APP_SRC_FILES})
