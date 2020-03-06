set(CMAKE_CXX_STANDARD 17)

set(SRC_FILES
        main.cpp
        package_list.cpp
        ${CMAKE_SOURCE_DIR}/src/jsoncpp.cpp
        downloader.h parser.h parser.cpp argument.cpp argument.h ccpath.cpp)

add_executable(cip ${SRC_FILES})

target_include_directories(cip PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_definitions(-DNOMINMAX)
add_definitions(-DUSE_7Z)

if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU AND ${CMAKE_SYSTEM_NAME} MATCHES Windows )
    target_link_libraries(cip shlwapi urlmon wininet)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()
if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU AND ${CMAKE_SYSTEM_NAME} MATCHES Linux )
    target_link_libraries(cip pthread stdc++fs)
endif()