cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

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

if (DEFINED ENV{VCPKG_ROOT})
  set(VCPKG_CMAKE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
  list(APPEND CMAKE_TOOLCHAIN_FILE ${VCPKG_CMAKE})
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  set(LIBROOT "$ENV{VCPKG_ROOT}/buildtrees/glad/x64-windows-dbg/src/glad.c")
  string(REGEX REPLACE "\\\\" "/" LIBROOT ${LIBROOT})
else()
  check_env(EXTERNAL_ROOT)
  set(VCPKG_CMAKE "${EXTERNAL_ROOT}/scripts/buildsystems/vcpkg.cmake")
  list(APPEND CMAKE_TOOLCHAIN_FILE ${VCPKG_CMAKE})
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  set(LIBROOT "${EXTERNAL_ROOT}/buildtrees/glad/x64-windows-dbg/src/glad.c")
  string(REGEX REPLACE "\\\\" "/" LIBROOT ${LIBROOT})
endif()

message(${LIBROOT})
message(${VCPKG_CMAKE})
set(name 3dvr)
project(${name})
add_definitions(-DSHADER_DIR="${CMAKE_SOURCE_DIR}//Src//Graphics//Shaders" -DMODEL_DIR="${CMAKE_SOURCE_DIR}//Src//Geometry//Models" -DSRC_DIR="${CMAKE_SOURCE_DIR}//Src")

#Main Files
add_executable(${name} Src/Application.h Src/Application.cpp Src/Main.cpp
						#Geometry Files
					   Src/Geometry/Model.cpp Src/Geometry/Model.h Src/Geometry/ModelManager.cpp Src/Geometry/ModelManager.h
					   
					   #Debug Files
					   #${LIBROOT}
					  
					   #Graphics Files
					   Src/Graphics/GUI.cpp Src/Graphics/GUI.h Src/Graphics/Shader.cpp Src/Graphics/Shader.h
					   
					   #Math Files
					   Src/Maths/Math.cpp Src/Maths/Math.h		
					   
					   #System Files
					   Src/System/VRhandler.cpp Src/System/VRhandler.h)

find_package(glm CONFIG REQUIRED) #Throws Error
find_package(imgui CONFIG REQUIRED) #Throws Error
find_package(assimp CONFIG REQUIRED) #Throws Error
find_package(glfw3 CONFIG REQUIRED) #Assumed Error Thrown not yet tested
find_package(glad CONFIG REQUIRED) #Assumed Error Thrown not yet tested
find_package(SDL2 CONFIG REQUIRED) #Assumed Error Thrown not yet tested
find_package(OpenVR REQUIRED) #Assumed Error Thrown not yet tested
  
# Had to use vcpkg x-package-info --x-json --x-installed [lib]:x86-windows in cmd to find the usage info
#target_link_libraries(${name} PRIVATE SDL2::SDL2 SDL2::SDL2main glfw assimp::assimp openvr::openvr glm::glm glad::glad imgui::imgui )
target_include_directories(${name} PRIVATE Src)
target_link_libraries(${name} PRIVATE glm::glm imgui::imgui assimp::assimp glfw glad::glad SDL2::SDL2 SDL2::SDL2main OpenVR::OpenVR) 




