#include "Magnum/Math/Color.h"
#include "Magnum/Math/Time.h"

#include "Magnum/GL/DefaultFramebuffer.h"
#include "Magnum/GL/Renderer.h"
#include "Magnum/GL/Buffer.h"
#include "Magnum/GL/Mesh.h"

#include "Magnum/Shaders/VertexColorGL.h"

#include "Magnum/ImGuiIntegration/Context.hpp"

#include <Magnum/Platform/Sdl2Application.h> 

#include<iostream>



using namespace Magnum;
using namespace Math::Literals;
using namespace std;

struct TriangleVertex{
    Vector2 position;
    Color3 color;
};

const TriangleVertex vertices[3] = {
    {{-0.5, -0.5}, 0xff0000_rgbf}, 
    {{0.5, -0.5}, 0x00ff00_rgbf}, 
    {{0.0, 0.5}, 0x0000ff_rgbf}
};




class MainWindow: public Platform::Application{

    ImGuiIntegration::Context _imgui{NoCreate};
    GL::Mesh _mesh;
    Shaders::VertexColorGL2D _shader;


    bool abool = true;
    Color4 _clearcolor = 0x72909aff_rgbaf;
    float x = 0;

    public:
        MainWindow(const Arguments& arguments);
        void drawEvent() override;
        void viewportEvent(ViewportEvent& event) override;

        void keyPressEvent(KeyEvent& event) override;
        void keyReleaseEvent(KeyEvent& event) override;

        void pointerPressEvent(PointerEvent& event) override;
        void pointerReleaseEvent(PointerEvent& event) override;
        void pointerMoveEvent(PointerMoveEvent& event) override;
        void scrollEvent(ScrollEvent& event) override;
        void textInputEvent(TextInputEvent& event) override;

};

MainWindow::MainWindow(const Arguments& arguments)
: Platform::Application(arguments, Configuration().setTitle("RoboForge").setWindowFlags(Configuration::WindowFlag::Resizable))
{
    _imgui = ImGuiIntegration::Context(Vector2(windowSize())/dpiScaling(), windowSize(), framebufferSize());
    GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);

    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha, GL::Renderer::BlendFunction::OneMinusSourceAlpha);

    _mesh.setCount(Containers::arraySize(vertices)).addVertexBuffer(GL::Buffer(vertices), 0, Shaders::VertexColorGL2D::Position(), Shaders::VertexColorGL2D::Color3());

    #if !defined(MAGNUM_TARGET_WEBGL) && !defined(CORRADE_TARGET_ANDROID)
        setMinimalLoopPeriod(16.0_msec);
    #endif

}

void MainWindow::drawEvent(){
    GL::Renderer::setClearColor(_clearcolor);
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    
    _imgui.newFrame();

    {
        int flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        ImGui::Begin("this is a window", nullptr, flags);
        ImGui::Text("Hello World");
        if (ImGui::ColorEdit3("Clear Color", _clearcolor.data())){
            GL::Renderer::setClearColor(_clearcolor);
        }
        if (ImGui::Button("Press me")){
            cout<<"Button Pressed"<<endl;
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0/Double(ImGui::GetIO().Framerate), Double(ImGui::GetIO().Framerate));

        ImGui::End();

    }

    _imgui.updateApplicationCursor(*this);

    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

    _imgui.drawFrame();
    _shader.draw(_mesh);

    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::enable(GL::Renderer::Feature::Blending);

    swapBuffers();
    redraw();
}


void MainWindow::viewportEvent(ViewportEvent& event){
    GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
    _imgui.relayout(Vector2{event.windowSize()}/event.dpiScaling(), event.windowSize(), event.framebufferSize());
    cout<<"viewportEvent"<<endl;
}

void MainWindow::keyPressEvent(KeyEvent& event) {
    if(_imgui.handleKeyPressEvent(event)) return;
}

void MainWindow::keyReleaseEvent(KeyEvent& event) {
    if(_imgui.handleKeyReleaseEvent(event)) return;
}

void MainWindow::pointerPressEvent(PointerEvent& event) {
    if(_imgui.handlePointerPressEvent(event)) return;
}

void MainWindow::pointerReleaseEvent(PointerEvent& event) {
    if(_imgui.handlePointerReleaseEvent(event)) return;
}

void MainWindow::pointerMoveEvent(PointerMoveEvent& event) {
    if(_imgui.handlePointerMoveEvent(event)) return;
}

void MainWindow::scrollEvent(ScrollEvent& event) {
    if(_imgui.handleScrollEvent(event)) {
        /* Prevent scrolling the page */
        event.setAccepted();
        return;
    }
}

void MainWindow::textInputEvent(TextInputEvent& event) {
    if(_imgui.handleTextInputEvent(event)) return;
}

MAGNUM_APPLICATION_MAIN(MainWindow)

//command to run:
