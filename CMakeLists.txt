cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
set(name 3dvr)

project(${name})

add_definitions(-DSHADER_DIR="${CMAKE_SOURCE_DIR}/Src/Graphics/Shaders" -DMODEL_DIR="${CMAKE_SOURCE_DIR}/Src/Geometry/Models")

function(check_env var)
if (NOT DEFINED ${var})
  if (DEFINED ENV{${var}})
    set(${var} $ENV{${var}} CACHE PATH "Location of ${var}")
  else()
    message(SEND_ERROR "${var} is NOT defined")
    set(${var} ""           CACHE PATH "Location of ${var}")
  endif()
endif()
endfunction(check_env)



#Main Files
add_executable(${name} Src/Application.h Src/Application.cpp Src/Main.cpp Deps/glad/src/gl.c
						#Geometry Files
					   Src/Geometry/Mesh.cpp Src/Geometry/Mesh.h Src/Geometry/Model.cpp Src/Geometry/Model.h Src/Geometry/ModelManager.cpp Src/Geometry/ModelManager.h Src/Geometry/Shape.cpp Src/Geometry/Shape.h
					   
					   #Debug Files
					  #${name} Debug/
					  
					   #Graphics Files
					   Src/Graphics/FrameBuffer.cpp Src/Graphics/FrameBuffer.h Src/Graphics/GUI.cpp Src/Graphics/GUI.h Src/Graphics/Renderer.cpp Src/Graphics/Renderer.h Src/Graphics/Shader.cpp Src/Graphics/Shader.h Src/Graphics/ShaderManager.cpp 
					   Src/Graphics/ShaderManager.h	
					   
					   #Input Files
					   Src/Input/Input.cpp Src/Input/Input.h	
					   
					   #Math Files
					   Src/Maths/Math.cpp Src/Maths/Math.h		
					   
					   #System Files
					   Src/System/Timer.cpp Src/System/Timer.h Src/System/VRhandler.cpp Src/System/VRhandler.h)
					   
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Src/Bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Src/Bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Src/Bin)

if (MSVC)
  if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(SEND_ERROR "Please build for 64-bit MSVS.")
  endif ()
  check_env(VCPKG_INCLUDE_PATH)
  check_env(VCPKG_LIB_PATH)
  #check_env(CMAKE_MODULE_PATH)
  if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	check_env(VCPKG_CMAKE_FILE)
	set(CMAKE_TOOLCHAIN_FILE ${VCPKG_CMAKE_FILE})
	endif()
  
  #include_directories($ENV{VCPKG_INCLUDE_PATH})
  #include_directories($ENV{VCPKG_LIB_PATH})
  
  #find_package(GLM REQUIRED)
  #find_package(GLM QUIET)
  #find_package(glm QUIET)
  #find_package(imgui REQUIRED)
  #find_package(assimp CONFIG REQUIRED)
  #find_package(glfw3 CONFIG REQUIRED)
  #find_package(glad CONFIG REQUIRED)
  #find_package(SDL2 CONFIG REQUIRED)
  #find_package(openvr CONFIG REQUIRED)
  #
  #target_link_libraries(${name} PRIVATE SDL2::SDL2 SDL2::SDL2main glfw assimp::assimp openvr::openvr glm::glm glad::glad imgui::imgui )
  
  
  #set(Lib_Path ${CMAKE_SOURCE_DIR}/Deps)
  #include_directories(${Lib_Path}/Assimp/include ${Lib_Path}/glad/include ${Lib_Path}/glfw-3.3.2/include ${Lib_Path}/glm/glm ${Lib_Path}/imgui ${Lib_Path}/openvr/include ${Lib_Path}/SDL/include ${CMAKE_SOURCE_DIR}/Src)
  #
  #target_link_directories(${name} PRIVATE ${Lib_Path}/Assimp/lib64 ${Lib_Path}/glfw-3.3.2/lib-vc2019 ${Lib_Path}/openvr/lib/win64 ${Lib_Path}/SDL/bin/win64)
  #message( "Lib Path is"  ${Lib_Path})
  #
  #set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${name})
  #
  #target_link_libraries(${name} debug assimp.lib glfw3.lib openvr_api.lib SDL2.lib SDL2main.lib)
  #target_link_libraries(${name} optimized assimp.lib glfw3.lib openvr_api.lib SDL2.lib SDL2main.lib)
endif ()



