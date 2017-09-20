cmake_minimum_required(VERSION 2.8.0)
project(wcsvrd)

set(CMAKE_VERBOSE_MAKEFILE ON)


############# VERSION ###################
set(MY_PRODUCT_NUMBER 1)
set(MY_PRODUCT_VERSION 0)
set(MY_BUILD_NUMBER 1)
set(MY_INTERNAL_BUILD_NUMBER 1)

configure_file(
  ${PROJECT_SOURCE_DIR}/version.h.in
  ${PROJECT_SOURCE_DIR}/version.h
  @ONLY)

IF (MSVC)
 	set(VERSION_FILE ${PROJECT_SOURCE_DIR}/version.h ${PROJECT_SOURCE_DIR}/version.rc)
ELSEIF (UNIX)
 	set(VERSION_FILE ${PROJECT_SOURCE_DIR}/version.h)
ENDIF()


############# BASIC SETTINGS #############

if (UNIX)
    add_definitions(-std=c++11)
	add_definitions(-Wwrite-strings)
	set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
	set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
 


############# BASIC DIRECTORIES #############


	
IF (MSVC)
	SET(PLATFORM win)
elseif (UNIX)
	SET(PLATFORM linux)
endif()



############# INCLUDE DIRECTORIES #############
include_directories(${PROJECT_SOURCE_DIR})


############# LINK DIRECTORIES #############



if (UNIX)
	set(UNIX_LIB pthread rt crypto ssl)
elseif (MSVC)
    set(WIN_LIB ws2_32 dbghelp)
endif()

############# BOOST SETTINGS #############


############# ADD SOURCES #############

file(GLOB SOURCES
    "*.h"
    "*.cpp"
)

################################


############# MAIN #############
add_executable(${PROJECT_NAME}  ${SOURCES}  ${VERSION_FILE})
target_link_libraries(${PROJECT_NAME} ${WIN_LIB} ${UNIX_LIB} )
   