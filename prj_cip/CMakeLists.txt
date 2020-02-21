set(CMAKE_CXX_STANDARD 17)

add_executable(cip
        main.cpp
        package_list.cpp
        )

target_include_directories(cip PUBLIC ${CMAKE_SOURCE_DIR}/include)
