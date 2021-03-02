# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

project ("vut_fit_icp_proj")

# Включите подпроекты.
add_subdirectory ("TextureProcessor")
