cmake_minimum_required(VERSION 3.16)
project(lmccode)

add_executable(lmccore lmccore.cpp yagcquaternion.cpp yagcquaternion.hpp)
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
   target_include_directories(lmccore PUBLIC ${OpenCV_INCLUDE_DIRS})
   target_link_libraries(lmccore ${OpenCV_LIBS})
endif()
