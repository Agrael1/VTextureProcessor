# CMakeList.txt: проект CMake для TextureProcessor; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.18)

project(TextureProcessor CXX)

set(CXX_STANDARD 20)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif()

# Добавьте источник в исполняемый файл этого проекта.
add_executable (TextureProcessor "TextureProcessor.cpp" "TextureProcessor.h" "Sink.h" "Source.h" "Source.cpp" "Sink.cpp" "Link.h" "Node.h" "Graph.h" "Node.cpp")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
