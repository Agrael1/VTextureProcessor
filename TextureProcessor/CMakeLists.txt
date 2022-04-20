# CMakeLists.txt
# author: Ilya Doroshenko (xdoros01)

cmake_minimum_required (VERSION 3.16)

project(TextureProcessor CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")
endif()

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets OpenGL REQUIRED)
include(FetchContent)

set(QT_DIR "${Qt6_DIR}")

set (BUILD_EXAMPLES OFF CACHE INTERNAL "Turn off examples")
FetchContent_Declare(
  ads
  GIT_REPOSITORY https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System.git
)
FetchContent_MakeAvailable(ads)
FetchContent_Declare(
  optimizer
  GIT_REPOSITORY https://github.com/Agrael1/tiny-glsl-optimizer.git
)
FetchContent_MakeAvailable(optimizer)



file(GLOB_RECURSE vsrc "src/*.cpp")
file(GLOB_RECURSE vhead "include/*.h" "include/*.hpp")

add_executable (TextureProcessor "${vsrc}" "${vhead}" "resources/resources.qrc" "include/Logic/ParameterStorage.h" "include/Projects/CreatePage.h")

target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt6Widgets_INCLUDE_DIRS}")
target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt6Widgets_INCLUDE_DIRS}")
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories("${PROJECT_NAME}" PUBLIC "${ads_SOURCE_DIR}/src")
target_include_directories("${PROJECT_NAME}" PUBLIC "${optimizer_SOURCE_DIR}/include")
target_precompile_headers("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
target_link_libraries("${PROJECT_NAME}" Qt6::Widgets)
target_link_libraries("${PROJECT_NAME}" Qt6::OpenGL)
target_link_libraries("${PROJECT_NAME}" qtadvanceddocking)
target_link_libraries("${PROJECT_NAME}" GlslOptimizerV2)


add_custom_command(TARGET "${PROJECT_NAME}" POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       $<TARGET_FILE_DIR:qtadvanceddocking> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/nodes $<TARGET_FILE_DIR:${PROJECT_NAME}>/nodes)


if(DEFINED WINDEPLOYQT_EXECUTABLE)
    add_custom_command(TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}"         
                --verbose 1
                "$<$<CONFIG:DEBUG>:--debug>"
                "$<$<CONFIG:RELEASE>:--release>"
                --no-svg
                \"$<TARGET_FILE:${PROJECT_NAME}>\"
        COMMENT "Deploying Qt libraries using windeployqt for compilation target '${PROJECT_NAME}' ..."
    )
else()
    include(windeployqt.cmake)
    windeployqt("${PROJECT_NAME}")
endif()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/nodes" DESTINATION ${INSTALL_PATH}/nodes FILES_MATCHING PATTERN "*.json")
