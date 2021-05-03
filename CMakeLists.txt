cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the project name
project(svn_curses)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/dependecies/subversion/subversion/include)
include_directories(${PROJECT_SOURCE_DIR}/dependecies/apr/include)

file(GLOB SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        )

set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED COMPONENTS) 
find_package(Curses REQUIRED COMPONENTS) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    include_directories(${CURSES_INCLUDE_DIRS}) 

    # add the executable
    add_executable(svn_curses ${SOURCES})

    target_link_libraries(svn_curses ${Boost_LIBRARIES})
    target_link_libraries(svn_curses ${CURSES_LIBRARIES})
endif()


find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(APR "apr-1")
    if (APR_FOUND)
        target_link_libraries(svn_curses ${APR_LIBRARIES})
        add_definitions(${APR_CFLAGS} ${APR_CFLAGS_OTHER})
  endif()
endif()
