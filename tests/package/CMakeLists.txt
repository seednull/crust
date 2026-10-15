cmake_minimum_required(VERSION 3.10)
project(crust_package LANGUAGES C)

find_package(crust CONFIG REQUIRED)

add_library(crust_package_shared SHARED shared.c)
target_link_libraries(crust_package_shared PRIVATE crust::crust)

add_executable(crust_package main.c)
target_link_libraries(crust_package PRIVATE crust_package_shared)
