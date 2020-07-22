cmake_minimum_required(VERSION 3.4.3)

project(realtor CXX)

find_package(InferenceEngine REQUIRED)
find_package(OpenCV REQUIRED)

add_library(Detector STATIC Detector.cpp room.cpp)
target_include_directories(Detector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Detector  PUBLIC ${OpenCV_LIBRARIES} ${InferenceEngine_LIBRARIES})

add_executable(realtor main.cpp)
target_link_libraries(realtor  PUBLIC Detector)

add_executable(testRealtor test_rooms.cpp)
target_link_libraries(testRealtor PUBLIC Detector)