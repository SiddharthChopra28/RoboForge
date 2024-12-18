cmake_minimum_required(VERSION 3.5)
project(RoboForge)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

# Add Corrade as a subproject
add_subdirectory(corrade EXCLUDE_FROM_ALL)

# Add Magnum as a subproject, enable Sdl2Application
set(MAGNUM_WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_ANYIMAGEIMPORTER ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_ANYSCENEIMPORTER ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_OBJIMPORTER ON CACHE BOOL "" FORCE)

add_subdirectory(magnum EXCLUDE_FROM_ALL)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(MAGNUM_WITH_IMGUI ON CACHE BOOL "" FORCE)
add_subdirectory(magnum-integration EXCLUDE_FROM_ALL)

add_subdirectory(src)

# If you want to load just basic OBJs

# If you want to load (textured) glTF as well
set(MAGNUM_WITH_STBIMAGEIMPORTER ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_GLTFIMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(magnum-plugins EXCLUDE_FROM_ALL)

add_dependencies(RoboForge
    Magnum::AnyImageImporter
    Magnum::AnySceneImporter
    Magnum::ObjImporter
    MagnumPlugins::StbImageImporter
    MagnumPlugins::GltfImporter)
