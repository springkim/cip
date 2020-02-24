cmake_minimum_required(VERSION 3.4)
project(cip VERSION 0.1 LANGUAGES CXX)

message(STATUS "OS")
message(STATUS " - System       \t: ${CMAKE_SYSTEM_NAME}")
message(STATUS " - Architecture \t: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Compiler")
message(STATUS " - ID       \t: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS " - Version  \t: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " - Path     \t: ${CMAKE_CXX_COMPILER}")




add_subdirectory(prj_cip)