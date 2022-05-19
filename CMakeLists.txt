cmake_minimum_required(VERSION 3.20)
project(FlexNetLicenseSearch)

set(CMAKE_CXX_STANDARD 14)

add_executable(FlexNetLicenseSearch main.cpp search.cpp search.h pattern.cpp pattern.h)
