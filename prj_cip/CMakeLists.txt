set(CMAKE_CXX_STANDARD 17)

set(SRC_FILES
        main.cpp
        package_list.cpp
        ${CMAKE_SOURCE_DIR}/src/jsoncpp.cpp
        downloader.h)

add_executable(cip ${SRC_FILES})

target_include_directories(cip PUBLIC ${CMAKE_SOURCE_DIR}/include)


if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU AND ${CMAKE_SYSTEM_NAME} MATCHES Windows )
    target_link_libraries(cip shlwapi urlmon wininet)
endif()