include(FetchContent)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory(ads)

FetchContent_Declare(
  optimizer
  GIT_REPOSITORY https://github.com/Agrael1/tiny-glsl-optimizer.git
)
FetchContent_MakeAvailable(optimizer)