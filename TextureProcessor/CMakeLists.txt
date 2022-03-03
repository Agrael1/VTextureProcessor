# CMakeLists.txt
# author: Ilya Doroshenko (xdoros01)

cmake_minimum_required (VERSION 3.16)

project(TextureProcessor CXX)

set(CXX_STANDARD 20)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")
endif()

set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_compile_definitions(_HAS_DEPRECATED_RESULT_OF=1)

add_executable (TextureProcessor
	"include/Logic/Sink.h"
	"include/Logic/Source.h"
    "include/UI/App.h"
    "include/UI/FlowScene.h"
    "include/UI/FlowView.h"
	"include/utils/Exception.h"
    "include/Windows/MainWindow.h"

	"src/Logic/Sink.cpp"
	"src/Logic/Source.cpp"
	"src/Logic/Node.cpp"  
    "src/UI/App.cpp"
    "src/UI/FlowScene.cpp"
    "src/UI/FlowView.cpp"
    "src/Windows/ProjectsWindow.cpp"
    "src/Windows/MainWindow.cpp"
	"src/EntryMain.cpp"

    "resources/resources.qrc"

    "include/UI/NodeStyle.h"
    "src/UI/NodeStyle.cpp"
    "include/UI/ContextMenu.h"
    "include/pch.h"
    "src/UI/ContextMenu.cpp"
    
    
    "include/Logic/SinksT.h"
    "include/Logic/SourcesT.h"
    "include/Logic/Engine.h"
    "src/Logic/Engine.cpp"
    "include/Logic/ShaderNode.h"
    "src/Logic/ShaderNode.cpp"
    
    
    "include/Logic/PortType.h"
    "include/UI/Connection.h"
    "src/UI/Connection.cpp"
    
    
    "include/Logic/DynamicConstant.h"
    "src/UI/Sliders.cpp"
    "include/Interfaces/INode.h"
    "include/UI/Updater.h"
    "include/Windows/ProjectsWindow.h"
    "include/UI/Frameless.h"
    "src/UI/Frameless.cpp"
    "include/Logic/ProjectsData.h"
    "src/Logic/ProjectsData.cpp"
    "include/Editor/Editor.h"
    "src/Editor/Editor.cpp"
    "include/Editor/Highlight.h"
    "src/Editor/Highlight.cpp"
    "include/Windows/TabRelay.h"
    "src/Windows/TabRelay.cpp"
    "include/Interfaces/IEditable.h"
    "include/utils/Macros.h"
    "include/windows/SceneTab.h"
    "src/Windows/SceneTab.cpp"
    "include/Windows/EditorTab.h"
    "include/Windows/Tab.h"
    "src/Windows/Tab.cpp"
    "src/Windows/EditorTab.cpp"
    "include/Logic/ApplicationConfig.h"
    "src/Logic/ApplicationConfig.cpp" "include/utils/utils.h" "src/utils/utils.cpp" "include/Logic/Constants.h" "include/UI/Node.h" "src/UI/Node.cpp" "include/UI/FlowCodex.h" "src/UI/FlowCodex.cpp" "include/UI/NodeModules.h" "include/Logic/PropertyView.h" "include/UI/PropertyGenerator.h" "src/UI/PropertyGenerator.cpp" "src/UI/NodeModules.cpp" "include/Windows/Properties.h" "src/Windows/Properties.cpp" "include/UI/SceneEvent.h" "include/UI/GraphicsLayout.h" "src/UI/Port.cpp" "include/Interfaces/ISerialize.h" "include/Interfaces/IConnection.h" "include/UI/NodeMenu.h" "include/Windows/TableProp.h" "src/Windows/TableProp.cpp" "include/Logic/DynamicNode.h" "include/Editor/CodeEditor.h" "src/Editor/CodeEditor.cpp" "include/Editor/NodeParser.h" "src/Editor/NodeParser.cpp" "include/Editor/Lexer.h" "src/Editor/Lexer.cpp" "include/utils/Promises.h" "include/utils/string_literal.h" "include/Editor/Language.h" "include/UI/PropertyHandler.h" "src/Logic/DynamicNode.cpp" "include/UI/DynamicNode.h" "src/UI/DynamicNode.cpp" "include/UI/NodeHeader.h")

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
