cmake_minimum_required(VERSION 3.20)  # 3.20 Recommended.
set(PROJECT_NAME A2AiGameProgramming)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

function(verify_path NAME PATH)
    message(CHECK_START "Verifying ${NAME} path (${PATH})")
    if(IS_DIRECTORY ${PATH})
        message(CHECK_PASS "Found")
    else()
        message(CHECK_FAIL "Not Found")
        message(FATAL_ERROR "Path (${PATH}) does not exist.")
    endif()
endfunction()


string(TOLOWER ${CMAKE_CXX_COMPILER_ID} COMP)
string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM)

if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL AMD64)
    set(ARCH "x64")
else()
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)
endif ()

set(COMPILER_TRIPLET "${COMP}-${PLATFORM}-${ARCH}")
message(STATUS "Compiling for: ${COMPILER_TRIPLET}")
message(STATUS "Using Cmake Version: ${CMAKE_VERSION}")

set(VENDOR_INCLUDE_DIR  ${CMAKE_SOURCE_DIR}/vendor/include)
set(VENDOR_LIB_DIR      ${CMAKE_SOURCE_DIR}/vendor/${COMPILER_TRIPLET}/lib)
set(VENDOR_SRC_DIR      ${CMAKE_SOURCE_DIR}/vendor/src)

verify_path("Vendor Include"    ${VENDOR_INCLUDE_DIR})
verify_path("Vendor Lib"        ${VENDOR_LIB_DIR})
verify_path("Vendor Source"     ${VENDOR_SRC_DIR})

add_executable(${PROJECT_NAME}
        src/Main.cpp

        src/core/Core.cpp
        src/core/Core.h
        src/core/DebugLogger.cpp
        src/core/Grid.cpp

        src/renderer/RendererSystem.cpp
        src/renderer/Shader.cpp

        include/core/DebugLogger.h
        include/core/Grid.h

        include/pathfinding/Pathfinding.h

        include/renderer/RendererSystem.h
        include/renderer/Shader.h

        ${VENDOR_SRC_DIR}/imgui/imgui.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_demo.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_draw.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_tables.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_widgets.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_impl_glfw.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_impl_opengl3.cpp


        src/core/Scene.cpp
        src/core/Scene.h
        src/renderer/GridMesh.cpp
        include/renderer/GridMesh.h
        src/file-io/MazeLoader.cpp
        include/file-io/MazeLoader.h
        src/q-learning/Agent.cpp
        include/q-learning/Agent.h
        include/q-learning/QlHelpers.h
        src/q-learning/Environment.cpp
        include/q-learning/Environment.h
        src/core/Common.cpp
        include/core/Common.h
        src/q-learning/QlPathFinder.cpp
        include/q-learning/QlPathFinder.h
        src/file-io/FileIoCommon.cpp
        include/file-io/FileIoCommon.h
        src/file-io/AiLoader.cpp
        include/file-io/AiLoader.h
        src/file-io/FileExplorer.cpp
        include/file-io/FileExplorer.h
        include/core/Pch.h)

if (${CMAKE_VERSION} VERSION_LESS 3.16)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NO_PCH)
else()
    target_precompile_headers(${PROJECT_NAME} PUBLIC
            <iostream> <vector> <unordered_map> <string> <string_view> <algorithm> <memory> <numeric> <cstdint>   # STL
            <glm.hpp> <gtx/quaternion.hpp> <glew.h>                                                     # Vendor
            [["DebugLogger.h"]]                                                                         # Project
            )
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC
        include
        include/core
        include/pathfinding
        include/renderer
        include/q-learning
        include/file-io

        ${VENDOR_INCLUDE_DIR}
        ${VENDOR_INCLUDE_DIR}/GL
        ${VENDOR_INCLUDE_DIR}/GLFW
        ${VENDOR_INCLUDE_DIR}/glm
        ${VENDOR_INCLUDE_DIR}/imgui
        ${VENDOR_INCLUDE_DIR}/stb-image
        )

target_compile_definitions(${PROJECT_NAME} PUBLIC
        GLEW_STATIC
        STB_IMAGE_IMPLEMENTATION
        LOG_TO_FILE
#        LOG_TO_CONSOLE
        LOG_TO_QUEUE
        )

find_package(OpenGL REQUIRED)
find_library(GLEW NAMES glew32s PATHS ${VENDOR_LIB_DIR} REQUIRED)
find_library(GLFW NAMES glfw3 PATHS ${VENDOR_LIB_DIR} REQUIRED)
target_link_libraries(${PROJECT_NAME}
        PUBLIC OpenGL::GL
        PUBLIC ${GLEW}
        PUBLIC ${GLFW}
        )

target_link_options(${PROJECT_NAME} PUBLIC -NODEFAULTLIB:glew32s)
