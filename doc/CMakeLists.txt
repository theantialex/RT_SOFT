cmake_minimum_required(VERSION 3.10)
SET("OpenCV_DIR" "/opencv_build/opencv/")
set(nlohmann_json_FOUND 1)

project(open)
find_package( OpenCV REQUIRED )
find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto REQUIRED IMPORTED_TARGET libmosquitto)
find_package(nlohmann_json REQUIRED
    CONFIG
    PATHS "/usr/lib/cmake/"
)

add_executable(open opencv.cpp)
target_link_libraries(open ${OpenCV_LIBS})
target_link_libraries(open PkgConfig::Mosquitto)
target_link_libraries(open nlohmann_json)
