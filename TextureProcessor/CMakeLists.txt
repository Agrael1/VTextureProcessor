# CMakeLists.txt
# author: Ilya Doroshenko (xdoros01)

cmake_minimum_required (VERSION 3.16)

project(TextureProcessor CXX)

set(CXX_STANDARD 23)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")
endif()

set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
# Include Qt deployment helper function
include(windeployqt.cmake)
include(FetchContent)

set(QT_DIR "${Qt5_DIR}")

set (BUILD_EXAMPLES OFF CACHE INTERNAL "Turn off examples")
FetchContent_Declare(
  ads
  GIT_REPOSITORY https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System.git
)
FetchContent_MakeAvailable(ads)




file(GLOB_RECURSE vsrc "src/*.cpp")
file(GLOB_RECURSE vhead "include/*.h" "include/*.hpp")

add_executable (TextureProcessor "${vsrc}" "${vhead}" "resources/resources.qrc" "include/Logic/ParameterStorage.h")

target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt5Widgets_INCLUDE_DIRS}")
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories("${PROJECT_NAME}" PUBLIC "${ads_SOURCE_DIR}/src")
target_precompile_headers("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
target_link_libraries("${PROJECT_NAME}" Qt5::Widgets)
target_link_libraries("${PROJECT_NAME}" qtadvanceddocking)


add_custom_command(TARGET "${PROJECT_NAME}" POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       $<TARGET_FILE_DIR:qtadvanceddocking> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/nodes $<TARGET_FILE_DIR:${PROJECT_NAME}>/nodes)



windeployqt("${PROJECT_NAME}")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/nodes" DESTINATION ${INSTALL_PATH}/nodes FILES_MATCHING PATTERN "*.json")
