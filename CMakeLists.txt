cmake_minimum_required(VERSION 3.1.0)
project(COMP2012H_Project VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "D:\\Qt\\Qt5.14.2\\5.14.2\\mingw73_64")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# set(CMAKE_BUILD_TYPE Debug)

find_package(Qt5 COMPONENTS Network Gui Core Multimedia Widgets REQUIRED)

aux_source_directory(./sources/utils UTILS_DIR_SRCS)
aux_source_directory(./sources/parser PARSER_DIR_SRCS)

add_subdirectory(./sources/math)

include_directories(./libraries/Armadillo/include
    .
    ./libraries
    # ./libraries/libgmp_6.2.1-1_msvc16/include
    ./libraries/gmp-6.2.1
    ./sources
    ./sources/math
    ./sources/math/double
    ./sources/math/fraction
    ./sources/math/linear
    ./sources/math/long
    ./sources/math/polynomial
    ./sources/utils
    ./sources/parser
)

# link_directories(./libraries ./libraries/libgmp_6.2.1-1_msvc16/lib/x64)
link_directories(./libraries)

add_executable(test_ocr
    ./sources/utils/ocr_api.cpp
    ./sources/utils/test_ocr.cpp
    # ./sources/parser/expr_ast.cpp
    # ./sources/parser/lexer.cpp
    # ./sources/parser/parser.cpp
    # ./sources/parser/test_parser.cpp
    # ./sources/utils/math_wrapper.cpp
)

# target_link_libraries(test_ocr
    # MathLib
    # ${CMAKE_CURRENT_SOURCE_DIR}/libraries/libgmp_6.2.1-1_msvc16/lib/x64/gmp.lib
    # ${CMAKE_CURRENT_SOURCE_DIR}/libraries/libgmp_6.2.1-1_msvc16/lib/x64/libgmp.lib
    # ${CMAKE_CURRENT_SOURCE_DIR}/libraries/libgmpxx.a
    # ${CMAKE_CURRENT_SOURCE_DIR}/libraries/libgmp.a
    # ${CMAKE_CURRENT_SOURCE_DIR}/libraries/libopenblas.lib
# )

target_link_libraries(test_ocr Qt5::Core Qt5::Gui Qt5::Network Qt5::Multimedia Qt5::Widgets)