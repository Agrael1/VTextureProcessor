project(adsx)

if(NOT PLUGINS_LOADED)
    find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
    FetchContent_Declare(
      ads
      GIT_REPOSITORY https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System.git
    )


    set(QT_DIR "${Qt6_DIR}")
    set (BUILD_EXAMPLES OFF CACHE INTERNAL "Turn off examples")
    set (BUILD_STATIC ON CACHE INTERNAL "Static")
    FetchContent_MakeAvailable(ads)
endif()

