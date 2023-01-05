cmake_minimum_required(VERSION 3.0.0)
project(Data_crawler VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
add_executable(main 
        main.cpp
        util/helper.h
        util/helper.cc
        util/parser.h
        util/parser.cc)
target_link_libraries(main ${CURL_LIBRARIES})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
