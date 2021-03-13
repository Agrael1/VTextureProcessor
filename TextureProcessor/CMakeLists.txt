# CMakeList.txt: проект CMake для TextureProcessor; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.18)

project(TextureProcessor CXX)

set(CXX_STANDARD 20)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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
set(CMAKE_BUILD_TYPE "Release")


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
	 
	"include/Sink.h" 
	"include/Source.h" 
	"src/Source.cpp" 
	"src/Sink.cpp" 
	"include/Link.h" 
	"src/Link.cpp"
	"include/UI/UINode.h" 
	"include/Graph.h"
	"include/utils/Exception.h"
	"src/Node.cpp"   "include/GraphCodex.h"  "include/utils/Utils.h" "include/UI/UINode.h" "include/UI/App.h" "src/UI/App.cpp" "include/UI/Window.h" "src/UI/Window.cpp" "src/UI/FlowScene.cpp" "include/UI/FlowScene.h" "include/UI/FlowView.h" "src/UI/FlowView.cpp" "src/UI/UINode.cpp"   "include/UI/NodeStyle.h" "src/UI/NodeStyle.cpp")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt5Widgets_INCLUDE_DIRS}")
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries("${PROJECT_NAME}" fmt::fmt)
target_link_libraries("${PROJECT_NAME}" Qt5::Widgets)
