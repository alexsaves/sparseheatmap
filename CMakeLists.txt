cmake_minimum_required(VERSION 3.3)
project(node_heatmap)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        binding.gyp
        index.js
        nodeheatmap.cc
        sparsematrix.cc
        sparsematrix.h sparsearray.h sparsearray.cpp.cpp sparsearray.cpp.h sparsescroll.cpp sparsescroll.h cpp/mapsampler.cpp cpp/mapsampler.h cpp/colormodel.cpp cpp/colormodel.h cpp/colorengine.cpp cpp/colorengine.h cpp/lodepng.cpp cpp/lodepng.h)

add_executable(node_heatmap ${SOURCE_FILES})