cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
set(name 3dvr)
project(${name})

if(DEFINED ENV{VCPKG_ROOT})
	set(VCPKG_CMAKE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
	list(APPEND CMAKE_TOOLCHAIN_FILE${VCPKG_CMAKE})
	set(-DCMAKE_PREFIX_PATH="$ENV{VCPKG_ROOT}//installed//x86-windows//share")
endif()


add_definitions(-DSHADER_DIR="${CMAKE_SOURCE_DIR}//Src//Graphics//Shaders" -DMODEL_DIR="${CMAKE_SOURCE_DIR}//Src//Geometry//Models")

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

check_env(VCPKG_INCLUDE_PATH)
check_env(VCPKG_LIB_PATH)

include_directories($ENV{VCPKG_INCLUDE_PATH})
include_directories($ENV{VCPKG_LIB_PATH})

find_package(glm REQUIRED) #Throws Error
#find_package(imgui REQUIRED) #Throws Error
#find_package(assimp CONFIG REQUIRED) #Throws Error
#find_package(glfw3 CONFIG REQUIRED) #Assumed Error Thrown not yet tested
#find_package(glad CONFIG REQUIRED) #Assumed Error Thrown not yet tested
#find_package(SDL2 CONFIG REQUIRED) #Assumed Error Thrown not yet tested
#find_package(openvr CONFIG REQUIRED) #Assumed Error Thrown not yet tested

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
  
  
  # Had to use vcpkg x-package-info --x-json --x-installed [lib]:x86-windows in cmd to find the usage info
  #target_link_libraries(${name} PRIVATE SDL2::SDL2 SDL2::SDL2main glfw assimp::assimp openvr::openvr glm::glm glad::glad imgui::imgui ) 
 
endif ()



