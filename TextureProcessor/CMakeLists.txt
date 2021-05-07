# CMakeList.txt: проект CMake для TextureProcessor; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.18)

project(TextureProcessor CXX)

set(CXX_STANDARD 20)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
endif()

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
set(CMAKE_BUILD_TYPE "Debug")


conan_cmake_configure(REQUIRES fmt/7.1.3 GENERATORS cmake_find_package)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conan-center
                    SETTINGS ${settings})

find_package(fmt)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_compile_definitions(_HAS_DEPRECATED_RESULT_OF=1)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (TextureProcessor
	"src/EntryMain.cpp"

	"include/Logic/Sink.h"
	"include/Logic/Source.h"
	"src/Logic/Source.cpp"
	"src/Logic/Sink.cpp"
	"include/UI/UINode.h"
	"include/utils/Exception.h"
	"src/Logic/Node.cpp" "include/utils/Utils.h" "include/UI/UINode.h" "include/UI/App.h" "src/UI/App.cpp" "include/UI/Window.h" "src/UI/Window.cpp" "src/UI/FlowScene.cpp" "include/UI/FlowScene.h" "include/UI/FlowView.h" "src/UI/FlowView.cpp" "src/UI/UINode.cpp"   "include/UI/NodeStyle.h" "src/UI/NodeStyle.cpp" "include/UI/ContextMenu.h" "include/pch.h" "src/UI/ContextMenu.cpp" "include/UI/FlowCodex.h" "src/UI/FlowCodex.cpp"  "include/Logic/SinksT.h" "include/Logic/SourcesT.h" "include/Presenter/GLInterpret.h" "src/Presenter/GLInterpret.cpp" "include/Logic/ShaderNode.h" "src/Logic/ShaderNode.cpp" "include/UI/TextureNode.h" "src/UI/TextureNode.cpp" "include/Logic/PortType.h" "include/UI/Connection.h" "src/UI/Connection.cpp"     "include/Editor/Editor.h" "src/Editor/Editor.cpp" "include/Editor/SyntaxHighlight.h" "include/Editor/SyntaxStyle.h" "include/UI/Properties.h" "src/UI/Properties.cpp" "include/Logic/DynamicConstant.h"  "src/UI/Sliders.cpp" "include/UI/INode.h"  "include/UI/Updater.h" )

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt5Widgets_INCLUDE_DIRS}")
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_precompile_headers("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
target_link_libraries("${PROJECT_NAME}" fmt::fmt)
target_link_libraries("${PROJECT_NAME}" Qt5::Widgets)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/nodes $<TARGET_FILE_DIR:${PROJECT_NAME}>/nodes)

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

