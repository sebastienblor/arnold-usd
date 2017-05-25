set(LIBNAME bifrost_procedural)

project(${LIBNAME})

cmake_minimum_required(VERSION 3.8.0)

file(GLOB SRC "*/*.cpp" "*.cpp")
file(GLOB HEADERS "*.h" "*/*.h" "*/*/*.h" "*/*/*/*.h")

set(ARNOLD_INCLUDE_PATH /solidangle/arnold/include)
set(ARNOLD_LIBRARY_PATH /solidangle/arnold/bin)

set(BIFROST_CORE_INCLUDE_PATH /Users/cynthia/dev/bifrostfluids/modules/bifrostcore/api)
set(BIFROST_API_INCLUDE_PATH /Users/cynthia/dev/bifrostfluids/modules/bifrostapi/api)
set(BIFROST_PROCESSING_INCLUDE_PATH /Users/cynthia/dev/mtoa/contrib/extensions/bifrost/procedural/bifrostprocessing/api)
set(AMINOMATH_INCLUDE_PATH /Users/cynthia/dev/bifrostfluids/modules/aminomath/api)
set(TBB_INCLUDE_PATH /Users/cynthia/dev/bifrostfluids/Artifactory/OSX/tbb/4.4u5/ca47f1a/include)

set(BIFROST_LIBS_PATH /Users/cynthia/dev/bifrostfluids/build/master/build/RelWithDebInfo/lib)
set(TBB_LIBS_PATH /Users/cynthia/dev/bifrostfluids/Artifactory/OSX/tbb/4.4u5/ca47f1a/lib)

INCLUDE_DIRECTORIES(
    include
    ${BIFROST_CORE_INCLUDE_PATH}
    ${BIFROST_API_INCLUDE_PATH}
    ${BIFROST_PROCESSING_INCLUDE_PATH}
    ${AMINOMATH_INCLUDE_PATH}
    ${ARNOLD_INCLUDE_PATH}
    ${TBB_INCLUDE_PATH}
)

#link_directories(
#    ${ARNOLD_LIBRARY_PATH}
#    ${BIFROST_LIBS_PATH}
#    ${TBB_LIBS_PATH}
#)

add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-comment")

set(allfiles ${HEADERS} ${SRC})
source_group(src FILES ${allfiles})
add_library(${LIBNAME} SHARED ${allfiles})

target_link_libraries(${LIBNAME}
    ${BIFROST_LIBS_PATH}/libbifrostapi_1_0.dylib
    ${ARNOLD_LIBRARY_PATH}/libai.dylib
    ${TBB_LIBS_PATH}/libtbb.dylib
)
