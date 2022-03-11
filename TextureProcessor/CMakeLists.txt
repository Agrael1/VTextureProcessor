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

add_compile_definitions(_HAS_DEPRECATED_RESULT_OF=1)


file(GLOB_RECURSE vsrc "src/*.cpp")
file(GLOB_RECURSE vhead "include/*.h" "include/*.hpp")

add_executable (TextureProcessor "${vsrc}" "${vhead}" "resources/resources.qrc")

target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt5Widgets_INCLUDE_DIRS}")
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_precompile_headers("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
target_link_libraries("${PROJECT_NAME}" Qt5::Widgets)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/nodes $<TARGET_FILE_DIR:${PROJECT_NAME}>/nodes)

# This part is adapted from StackOverflow:
# https://stackoverflow.com/questions/40564443/copying-qt-dlls-to-executable-directory-on-windows-using-cmake
if(WIN32)
    if($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_DEBUG)
    else($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_RELEASE)
    endif($<CONFIG:Debug>)
    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Widgets>
        $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE:Qt5::Gui>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif(WIN32)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/nodes" DESTINATION ${INSTALL_PATH}/nodes FILES_MATCHING PATTERN "*.json")
