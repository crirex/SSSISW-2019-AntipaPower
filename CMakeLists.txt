#--------------------------------------------------------------
# 
# Example of CMake configuration file to build an external 
# project depending on Chrono and on optional Chrono modules.
# 
# This minimal sample project can be used as a template for a
# user project.  Modify sections 1, 2, and 3 below as appropriate.
# 
#--------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)

#--------------------------------------------------------------
# === 1 === 
# Modify the project name if you want: 
#--------------------------------------------------------------

project(AMMO)

#--------------------------------------------------------------
# === 2 ===
# Find the Chrono package and any REQUIRED or OPTIONAL modules
# by invoking the find_package function in CONFIG mode:
#    find_package(Chrono
#	              COMPONENTS req_module1 req_module1 ...
#	              OPTIONAL opt_module1 opt_module2 ...
#                 CONFIG)
# The following Chrono modules can be requested (their names
# are case insensitive): Cascade, Cosimulation, FEA, Irrlicht,
# Matlab, Parallel, Postprocess, Python, Vehicle.
# 
# Note that you will have to set the variable Chrono_DIR to 
# specify the location of the ChronoConfig.cmake script, if
# it is not in its default install location.
# Chrono_DIR can be either a Chrono build tree or a Chrono install tree.
# 
# The following variables are set and can be used further down:
# CHRONO_FOUND
#   set to true if Chrono and all required components were found
# CHRONO_C_FLAGS
# CHRONO_CXX_FLAGS
#   C and C++ compilation flags
# CHRONO_INCLUDE_DIRS
#   additional paths for included headers
# CHRONO_LIBRARIES
#   list of required libraries (with full path)
# CHRONO_LINKER_FLAGS
#   additional linker flags
# CHRONO_DLLS
#   list of all DLL dependencies (with full path)
# CHRONO_DATA_DIR
#   path to the Chrono data make_directory
# 
# In addition, for each requested component [COMPONENT], the
# following variable is set to true (ON) or false (OFF):
# CHRONO_[COMPONENT]_FOUND
# 
# In this example, we only request the Irrlicht module (required)
#--------------------------------------------------------------

find_package(Chrono
             COMPONENTS Irrlicht
             CONFIG)

find_package(Chrono
             COMPONENTS Irrlicht Postprocessing
             CONFIG)

#--------------------------------------------------------------
# Return now if Chrono or a required component was not found.
#--------------------------------------------------------------

if (NOT Chrono_FOUND)
  message( FATAL_ERROR "Could not find Chrono or one of its required modules")
  SET(Chrono_DIR "${AMMO_SOURCE_DIR}/../chrono_build/cmake")
  LIST(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}/../Chrono/lib")
endif()

#--------------------------------------------------------------
# Enable creation of "application bundles" on MacOSX.
#--------------------------------------------------------------

# This is necessary for any Irrlicht-based project (like the example here).
# For OpenGL-based or non-graphics projects, this is optional and the block
# below can be removed (or else explcitly set CMAKE_MACOSX_BUNDLE to 'OFF').
#
# If creating application bundles, the build output will be named 'myexe.app'.
# Use the convenience script 'run_app.sh' available under 'contrib/appbundle-macosx/'
# to run:
#     start_demo.sh myexe.app

if(APPLE)
    set(CMAKE_MACOSX_BUNDLE ON)
endif()

#--------------------------------------------------------------
# Add path to Chrono headers and to headers of all dependencies
# of the requested modules.
#--------------------------------------------------------------

include_directories(${CHRONO_INCLUDE_DIRS})

#--------------------------------------------------------------
# Tweaks to disable some warnings with MSVC
#--------------------------------------------------------------
if(MSVC)
    add_definitions("-D_CRT_SECURE_NO_DEPRECATE")  # avoids deprecation warnings
    add_definitions("-D_SCL_SECURE_NO_DEPRECATE")  # avoids deprecation warnings
    add_definitions( "-DNOMINMAX" )                # do not use MSVC's min/max macros
    set(EXTRA_COMPILE_FLAGS "/wd4275")             # disable warnings triggered by Irrlicht
else()
    set(EXTRA_COMPILE_FLAGS "")
endif()

#--------------------------------------------------------------
# === 3 ===
# Add the executable from your project and specify all C++ 
# files in your project. 
#--------------------------------------------------------------

set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/source)

file(GLOB_RECURSE INCS 
	"${INCROOT}/*.h"
)
file(GLOB_RECURSE SRCS 
	"${SRCROOT}/*.cpp"
)

#set(BOOST_ROOT E:/Projects/AMMO/boost_1_70_0)
#set(Boost_INCLUDE_DIR E:/Projects/AMMO/boost_1_70_0)
#set(BOOST_LIBRARYDIR E:/Projects/AMMO/boost_1_70_0/stage/lib)

#FIND_PACKAGE( Boost 1.70.0 REQUIRED COMPONENTS dynamic_bitset )
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

ADD_EXECUTABLE(AMMO 
${INCS} 
${SRCS}
)   

#TARGET_LINK_LIBRARIES( AMMO LINK_PUBLIC ${Boost_LIBRARIES} )

foreach(FILE ${SRCS}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

foreach(FILE ${INCS}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

#source_group("Source files\\Enviorment" FILES ${SRC_EnviormentS})
#source_group("Banana" FILES ${SRC_Banana})
#source_group("Source files\\Optimizer" FILES ${SRC_OptimizerS})
#source_group("Include files\\Enviorment" FILES ${SRC_EnviormentI})
#source_group("Include files\\Optimizer" FILES ${SRC_OptimizerI})


#--------------------------------------------------------------
# Set properties for your executable target
# 
# Note that here we define a macro CHRONO_DATA_DIR which will
# contain the path to the Chrono data directory, either in its
# source tree (if using a build version of Chrono), or in its
# install tree (if using an installed version of Chrono).
#--------------------------------------------------------------

set_target_properties(AMMO PROPERTIES 
	    COMPILE_FLAGS "${CHRONO_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}"
	    COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
	    LINK_FLAGS "${CHRONO_LINKER_FLAGS}"
		CXX_STANDARD 17)

#--------------------------------------------------------------
# Link to Chrono libraries and dependency libraries
#--------------------------------------------------------------

target_link_libraries(AMMO ${CHRONO_LIBRARIES})

#--------------------------------------------------------------
# === 4 (OPTIONAL) ===
# 
# Optionally, add a custom command for copying all Chrono and
# dependency DLLs to the appropriate binary output folder.
# This function has effect only on Windows.
# 
# Note that you must first set EXECUTABLE_OUTPUT_PATH
# (this can simply be ${CMAKE_BINARY_DIR}, like in this example)
#--------------------------------------------------------------
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#add_DLL_copy_command("${CHRONO_DLLS}")

set (DEBUG_PATH ${AMMO_SOURCE_DIR}/build/Debug)
set (CHRONO_DEBUG_PATH ${Chrono_DIR}/../bin/Debug)

set (RELEASE_PATH ${AMMO_SOURCE_DIR}/build/Release)
set (CHRONO_RELEASE_PATH ${Chrono_DIR}/../bin/Release)

file(COPY
   ${CHRONO_RELEASE_PATH}/ChronoEngine.dll
   DESTINATION ${RELEASE_PATH})
	
file(COPY
    ${CHRONO_RELEASE_PATH}/ChronoEngine_irrlicht.dll
    DESTINATION ${RELEASE_PATH})
	
file(COPY
    ${CHRONO_RELEASE_PATH}/ChronoEngine_postprocess.dll
    DESTINATION ${RELEASE_PATH})
	
file(COPY
    ${CHRONO_RELEASE_PATH}/Irrlicht.dll
    DESTINATION ${RELEASE_PATH})
	
	
file(COPY
   ${CHRONO_DEBUG_PATH}/ChronoEngine.dll
   DESTINATION ${DEBUG_PATH})
	
file(COPY
    ${CHRONO_DEBUG_PATH}/ChronoEngine_irrlicht.dll
    DESTINATION ${DEBUG_PATH})
	
file(COPY
    ${CHRONO_DEBUG_PATH}/ChronoEngine_postprocess.dll
    DESTINATION ${DEBUG_PATH})
	
file(COPY
    ${CHRONO_DEBUG_PATH}/Irrlicht.dll
    DESTINATION ${DEBUG_PATH})

include_directories(AMMO ${AMMO_SOURCE_DIR}/include)