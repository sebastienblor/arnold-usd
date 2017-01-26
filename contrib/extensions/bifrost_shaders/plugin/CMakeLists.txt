set(LIB_NAME bifrost_shaders_translators)
project(${LIB_NAME})

cmake_minimum_required(VERSION 2.8.8)

add_definitions(-std=c++11)
add_definitions(-DDLLEXPORT=)

file(GLOB_RECURSE SRC *.cpp)
file(GLOB_RECURSE HEADERS *.h)

set(allfiles ${HEADERS} ${SRC} ${HEADERS})
source_group(src FILES ${allfiles})
add_library(${LIB_NAME} SHARED ${allfiles})

set(MAYA_LOCATION /home/beauchc/Perforce/beauchc_maya_2/build/Release/runTime)
set(MTOA_LOCATION /solidangle/mtoa)
set(ARNOLD_LOCATION /solidangle/arnold)

INCLUDE_DIRECTORIES(${MTOA_LOCATION}/include)
INCLUDE_DIRECTORIES(${ARNOLD_LOCATION}/include)
INCLUDE_DIRECTORIES(${MAYA_LOCATION}/include)

target_link_libraries(${LIB_NAME}
    ${MAYA_LOCATION}/lib/libOpenMaya.so
    ${MTOA_LOCATION}/bin/libmtoa_api.so
    ${MTOA_LOCATION}/bin/libai.so
)

