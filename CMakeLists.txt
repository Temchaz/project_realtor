cmake_minimum_required(VERSION 3.4.3)

project(realtor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(InferenceEngine REQUIRED)
find_package(OpenCV REQUIRED)

add_library(DetectorLib STATIC Detector.cpp room.cpp embedding.cpp)
target_include_directories(DetectorLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(DetectorLib  PUBLIC ${OpenCV_LIBRARIES} ${InferenceEngine_LIBRARIES})

add_executable(realtor main.cpp)
target_link_libraries(realtor  PUBLIC DetectorLib)

add_executable(testRealtor test_rooms.cpp)
target_link_libraries(testRealtor PUBLIC DetectorLib)