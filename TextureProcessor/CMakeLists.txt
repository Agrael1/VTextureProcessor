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
	"src/EntryMain.cpp"
    "src/Windows/ProjectsWindow.cpp"
    "resources/resources.qrc"
	"include/Logic/Sink.h"
	"include/Logic/Source.h"
	"src/Logic/Source.cpp"
	"src/Logic/Sink.cpp"
	"include/UI/UINode.h"
	"include/utils/Exception.h"
	"src/Logic/Node.cpp"  "include/UI/UINode.h" "include/UI/App.h" "src/UI/App.cpp" "include/Windows/MainWindow.h" "src/Windows/MainWindow.cpp" "src/UI/FlowScene.cpp" "include/UI/FlowScene.h" "include/UI/FlowView.h" "src/UI/FlowView.cpp" "src/UI/UINode.cpp"   "include/UI/NodeStyle.h" "src/UI/NodeStyle.cpp" "include/UI/ContextMenu.h" "include/pch.h" "src/UI/ContextMenu.cpp" "include/UI/FlowCodex.h" "src/UI/FlowCodex.cpp"  "include/Logic/SinksT.h" "include/Logic/SourcesT.h" "include/Logic/Engine.h" "src/Logic/Engine.cpp" "include/Logic/ShaderNode.h" "src/Logic/ShaderNode.cpp" "include/UI/TextureNode.h" "src/UI/TextureNode.cpp" "include/Logic/PortType.h" "include/UI/Connection.h" "src/UI/Connection.cpp" "include/Windows/Properties.h" "src/Windows/Properties.cpp" "include/Logic/DynamicConstant.h"  "src/UI/Sliders.cpp" "include/UI/INode.h"  "include/UI/Updater.h"  "include/Windows/ProjectsWindow.h" "include/UI/Frameless.h" "src/UI/Frameless.cpp"  "include/Logic/ProjectsData.h" "src/Logic/ProjectsData.cpp" "include/Editor/Editor.h" "src/Editor/Editor.cpp" "include/Editor/Highlight.h" "src/Editor/Highlight.cpp" "include/UI/TabRelay.h" "src/UI/TabRelay.cpp" "include/UI/IEditable.h" "include/utils/Macros.h" "include/windows/SceneTab.h" "src/Windows/SceneTab.cpp"  "include/Windows/EditorTab.h" "include/Windows/Tab.h" "src/Windows/Tab.cpp"  "src/Windows/EditorTab.cpp"  "include/Logic/ApplicationConfig.h" "src/Logic/ApplicationConfig.cpp" "include/utils/utils.h" "src/utils/utils.cpp" "include/Logic/Constants.h" "include/UI/Node.h" "src/UI/Node.cpp" "include/UI/REFlowCodex.h" "src/UI/REFlowCodex.cpp" "include/UI/NodeModules.h" "include/Logic/PropertyView.h" "include/UI/PropertyGenerator.h" "src/UI/PropertyGenerator.cpp")

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
