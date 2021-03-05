cmake_minimum_required(VERSION 3.0)
project(irgLab)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(irgLab/dependencies/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(irgLab/dependencies/assimp)


add_subdirectory(irgLab/primjerGLM)
add_subdirectory(irgLab/primjerASSIMP)
add_subdirectory(irgLab/primjerOpenGL)


add_subdirectory(irgLab/vjezba1)
add_subdirectory(irgLab/vjezba2)
add_subdirectory(irgLab/vjezba3)
add_subdirectory(irgLab/vjezba4)
add_subdirectory(irgLab/vjezba5a)
add_subdirectory(irgLab/vjezba5b)
add_subdirectory(irgLab/vjezba6)
add_subdirectory(irgLab/vjezba7)
add_subdirectory(irgLab/vjezba8a)
add_subdirectory(irgLab/vjezba8b)
add_subdirectory(irgLab/vjezba8c)
add_subdirectory(irgLab/vjezba9)
add_subdirectory(irgLab/vjezba10)
