cmake_minimum_required (VERSION 2.8.11)

project (Gmsh2TIN)
SET(PROJECT_NAME gmsh2TIN)

IF (WIN32)
  IF (MSVC)
    ## For Visual Studio compiler
    MESSAGE (STATUS "Visual studio C++ compiler") 
    SET ( CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE TYPE INTERNAL FORCE )
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS
			-D_CRT_XNONSTDC_NO_WARNINGS)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS)
    ADD_DEFINITIONS(-DMSVC)
    # Sets warning level 3 and ignores some warnings
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /wd4290 /wd4267")  
  ENDIF (MSVC)
ENDIF (WIN32)

MESSAGE (STATUS "The system is \"${CMAKE_SYSTEM_NAME}\"")

IF (UNIX)
  string( LENGTH "$ENV{DESTDIR}" DESTDIRLEN )
  IF ( ${DESTDIRLEN} GREATER 0 )
    message( STATUS "DESTDIR specified ($ENV{DESTDIR}). Overriding 
        	CMAKE_INSTALL_PREFIX (${CMAKE_INSTALL_PREFIX})" )
    set( CMAKE_INSTALL_PREFIX "" CACHE PATH "Use DESTDIR instead of CMAKE_INSTALL_PREFIX" FORCE )
  ENDIF()	
ENDIF (UNIX)

MESSAGE (STATUS "Compiler is \"${CMAKE_CXX_COMPILER}\"")

### Compiler options.
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  ADD_DEFINITIONS( -std=c++11 )

  IF(CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
  ELSE(CMAKE_BUILD_TYPE STREQUAL "Debug" )
    MESSAGE(STATUS "Set GCC release flags")
    SET(CMAKE_CXX_FLAGS "-O3 -DNDEBUG")
  ENDIF()

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wdeprecated -Wextra -Woverloaded-virtual")
  
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
  IF(CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")
  ELSE(CMAKE_BUILD_TYPE STREQUAL "Debug" )
    MESSAGE(STATUS "Set GCC release flags")
    SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
  ENDIF()

  # -g
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wall -Wextra -Woverloaded-virtual -fno-nonansi-builtins")

  IF("${CMAKE_CXX_COMPILER}" MATCHES "mingw")
    IF(CMAKE_BUILD_TYPE STREQUAL "Debug" )
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static ")
    ELSE(CMAKE_BUILD_TYPE STREQUAL "Debug" )
      MESSAGE(STATUS "Set GCC release flags")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mtune=core2 -mthreads -mms-bitfields -O3 -static")
    ENDIF()
  ENDIF()

  ADD_DEFINITIONS( -std=gnu++11 )

  IF (NOT CYGWIN)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi")
  ENDIF()

ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
    
ENDIF()

# Set build directories
# Binaries are created in /bin and libraries in /lib
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )

set( SOURCES 
        Node.h
        Element.h
        Mesh.h
        Mesh.cpp
)

add_library(Mesh STATIC ${SOURCES} )

add_executable (${PROJECT_NAME}   main.cpp  ${SOURCES} )
target_link_libraries(
  ${PROJECT_NAME} 
  Mesh 
)
