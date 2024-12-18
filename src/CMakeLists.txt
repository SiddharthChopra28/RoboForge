cmake_minimum_required(VERSION 3.5)
project(RoboForge)

# Add module path in case this is project root
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../modules/" ${CMAKE_MODULE_PATH})
endif()



find_package(Corrade REQUIRED Main)
find_package(MagnumIntegration REQUIRED ImGui)
find_package(Magnum REQUIRED GL
    GL
    MeshTools
    Shaders
    SceneGraph
    Trade
    Sdl2Application)

if(CORRADE_TARGET_ANDROID)
    find_package(Magnum REQUIRED AndroidApplication)
elseif(CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED EmscriptenApplication)
else()
    find_package(Magnum REQUIRED Sdl2Application)
endif()


set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)
add_executable(RoboForge WIN32 main3.cpp)


target_link_libraries(RoboForge PRIVATE
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    MagnumIntegration::ImGui
    Magnum::MeshTools
    Magnum::SceneGraph
    Magnum::Shaders
    Magnum::Trade)


install(TARGETS RoboForge DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RoboForge)
