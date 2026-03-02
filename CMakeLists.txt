cmake_minimum_required(VERSION 3.20)
project(eliott-tmx-parser VERSION 1.0.0 LANGUAGES CXX)

file(COPY tests/data DESTINATION ${CMAKE_BINARY_DIR}/tests)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(tinyxml2 CONFIG REQUIRED)


add_library(eliott-tmx-parser STATIC
    src/TmxParser.cpp
)

target_include_directories(eliott-tmx-parser PUBLIC
    include/
)
if(PROJECT_IS_TOP_LEVEL)
find_package(doctest CONFIG REQUIRED)

add_executable(tests
    tests/test_parser.cpp
)

target_link_libraries(tests PRIVATE
    eliott-tmx-parser
    doctest::doctest
    tinyxml2::tinyxml2
)
endif()

target_link_libraries(eliott-tmx-parser PRIVATE
   tinyxml2::tinyxml2
)