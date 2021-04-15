# Install script for directory: D:/uet/202/graphics/p2-meshedit-current-master/CGL/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ColladaViewer")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/uet/202/graphics/p2-meshedit-current-master/cmake-build-debug/CGL/src/libCGL.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CGL" TYPE FILE FILES
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/CGL.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/vector2D.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/vector3D.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/vector4D.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/matrix3x3.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/matrix4x4.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/quaternion.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/complex.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/color.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/osdtext.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/viewer.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/base64.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/tinyxml2.h"
    "D:/uet/202/graphics/p2-meshedit-current-master/CGL/src/renderer.h"
    )
endif()

