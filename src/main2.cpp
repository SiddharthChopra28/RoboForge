#include<iostream>

#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/Pair.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Arguments.h>
#include <Corrade/Utility/DebugStl.h>


#include <Magnum/ImageView.h>
#include <Magnum/Mesh.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include "Magnum/GL/Buffer.h"

#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Math/Color.h>
#include "Magnum/Math/Time.h"
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/PhongGL.h>
#include "Magnum/Shaders/VertexColorGL.h"

#include "Magnum/ImGuiIntegration/Context.hpp"

#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Trade/PhongMaterialData.h>
#include <Magnum/Trade/SceneData.h>
#include <Magnum/Trade/TextureData.h>


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


typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

class ColoredDrawable: public SceneGraph::Drawable3D {
    public:
        explicit ColoredDrawable(Object3D& object, Shaders::PhongGL& shader, GL::Mesh& mesh, const Color4& color, SceneGraph::DrawableGroup3D& group): SceneGraph::Drawable3D{object, &group}, _shader(shader), _mesh(mesh), _color{color} {}

    private:
        void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

        Shaders::PhongGL& _shader;
        GL::Mesh& _mesh;
        Color4 _color;
};

class TexturedDrawable: public SceneGraph::Drawable3D {
    public:
        explicit TexturedDrawable(Object3D& object, Shaders::PhongGL& shader, GL::Mesh& mesh, GL::Texture2D& texture, SceneGraph::DrawableGroup3D& group): SceneGraph::Drawable3D{object, &group}, _shader(shader), _mesh(mesh), _texture(texture) {}

    private:
        void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

        Shaders::PhongGL& _shader;
        GL::Mesh& _mesh;
        GL::Texture2D& _texture;
};

class MainWindow: public Platform::Application{


    Vector3 positionOnSphere(const Vector2& position) const;
    Shaders::PhongGL _coloredShader;
    Shaders::PhongGL _texturedShader{Shaders::PhongGL::Configuration{}.setFlags(Shaders::PhongGL::Flag::DiffuseTexture)};

    ImGuiIntegration::Context _imgui{NoCreate};
    GL::Mesh _mesh;

    Shaders::VertexColorGL2D _shader;
    Containers::Array<Containers::Optional<GL::Texture2D>> _textures;
    Containers::Array<Containers::Optional<GL::Mesh>> _meshes;

    Scene3D _scene;
    Object3D _manipulator, _cameraObject;
    SceneGraph::Camera3D* _camera;
    SceneGraph::DrawableGroup3D _drawables;
    Vector3 _previousPosition;


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

    Utility::Arguments args;
    args.addArgument("file").setHelp("file", "file to load")
        .addOption("importer", "AnySceneImporter")
            .setHelp("importer", "importer plugin to use")
        .addSkippedPrefix("magnum", "engine-specific options")
        .setGlobalHelp("Displays a 3D scene file provided on command line.")
        .parse(arguments.argc, arguments.argv);

    _cameraObject
        .setParent(&_scene)
        .translate(Vector3::zAxis(5.0f));
    (*(_camera = new SceneGraph::Camera3D{_cameraObject}))
        .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setProjectionMatrix(
            Matrix4::perspectiveProjection(35.0_degf, 1.0f, 0.01f, 1000.0f)
        )
        .setViewport(GL::defaultFramebuffer.viewport().size());

    _manipulator.setParent(&_scene);

    PluginManager::Manager<Trade::AbstractImporter> manager;
    Containers::Pointer<Trade::AbstractImporter> importer = manager.loadAndInstantiate(args.value("importer"));

    if(!importer || !importer->openFile(args.value("file"))){
        std::exit(1);
    }

    _textures = Containers::Array<Containers::Optional<GL::Texture2D>>{
        importer->textureCount()};
    for(UnsignedInt i = 0; i != importer->textureCount(); ++i) {
        Containers::Optional<Trade::TextureData> textureData =
            importer->texture(i);
        if(!textureData || textureData->type() != Trade::TextureType::Texture2D) {
            Warning{} << "Cannot load texture" << i
                << importer->textureName(i);
            continue;
        }

        Containers::Optional<Trade::ImageData2D> imageData =
            importer->image2D(textureData->image());
        if(!imageData || imageData->isCompressed()) {
            Warning{} << "Cannot load image" << textureData->image()
                << importer->image2DName(textureData->image());
            continue;
        }

        (*(_textures[i] = GL::Texture2D{}))
            .setMagnificationFilter(textureData->magnificationFilter())
            .setMinificationFilter(textureData->minificationFilter(),
                                   textureData->mipmapFilter())
            .setWrapping(textureData->wrapping().xy())
            .setStorage(Math::log2(imageData->size().max()) + 1,
                GL::textureFormat(imageData->format()), imageData->size())
            .setSubImage(0, {}, *imageData)
            .generateMipmap();
    }

    Containers::Array<Containers::Optional<Trade::PhongMaterialData>> materials{
        importer->materialCount()};
    for(UnsignedInt i = 0; i != importer->materialCount(); ++i) {
        Containers::Optional<Trade::MaterialData> materialData;
        if(!(materialData = importer->material(i))) {
            Warning{} << "Cannot load material" << i
                << importer->materialName(i);
            continue;
        }

        materials[i] = std::move(*materialData).as<Trade::PhongMaterialData>();
    }
    
    if(importer->defaultScene() == -1) {
        if(!_meshes.isEmpty() && _meshes[0])
            new ColoredDrawable{_manipulator, _coloredShader, *_meshes[0],
                0xffffff_rgbf, _drawables};
        return;
    }

    Containers::Optional<Trade::SceneData> scene;
    if(!(scene = importer->scene(importer->defaultScene())) ||
       !scene->is3D() ||
       !scene->hasField(Trade::SceneField::Parent) ||
       !scene->hasField(Trade::SceneField::Mesh))
    {
        Fatal{} << "Cannot load scene" << importer->defaultScene()
            << importer->sceneName(importer->defaultScene());
    }

    Containers::Array<Object3D*> objects{std::size_t(scene->mappingBound())};
    Containers::Array<Containers::Pair<UnsignedInt, Int>> parents
        = scene->parentsAsArray();
    for(const Containers::Pair<UnsignedInt, Int>& parent: parents)
        objects[parent.first()] = new Object3D{};

    for(const Containers::Pair<UnsignedInt, Int>& parent: parents)
        objects[parent.first()]->setParent(parent.second() == -1 ?
            &_manipulator : objects[parent.second()]);

    for(const Containers::Pair<UnsignedInt, Matrix4>& transformation:
        scene->transformations3DAsArray())
    {
        if(Object3D* object = objects[transformation.first()])
            object->setTransformation(transformation.second());
    }

        for(const Containers::Pair<UnsignedInt, Containers::Pair<UnsignedInt, Int>>&
        meshMaterial: scene->meshesMaterialsAsArray())
    {
        Object3D* object = objects[meshMaterial.first()];
        Containers::Optional<GL::Mesh>& mesh =
            _meshes[meshMaterial.second().first()];
        if(!object || !mesh) continue;

        Int materialId = meshMaterial.second().second();

        /* Material not available / not loaded, use a default material */
        if(materialId == -1 || !materials[materialId]) {
            new ColoredDrawable{*object, _coloredShader, *mesh, 0xffffff_rgbf,
                _drawables};

        /* Textured material, if the texture loaded correctly */
        } else if(materials[materialId]->hasAttribute(
                Trade::MaterialAttribute::DiffuseTexture
            ) && _textures[materials[materialId]->diffuseTexture()])
        {
            new TexturedDrawable{*object, _texturedShader, *mesh,
                *_textures[materials[materialId]->diffuseTexture()],
                _drawables};

        /* Color-only material */
        } else {
            new ColoredDrawable{*object, _coloredShader, *mesh,
                materials[materialId]->diffuseColor(), _drawables};
        }
    }
}


void ColoredDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    _shader
        .setDiffuseColor(_color)
        .setLightPositions({
            {camera.cameraMatrix().transformPoint({-3.0f, 10.0f, 10.0f}), 0.0f}
        })
        .setTransformationMatrix(transformationMatrix)
        .setNormalMatrix(transformationMatrix.normalMatrix())
        .setProjectionMatrix(camera.projectionMatrix())
        .draw(_mesh);
}

void TexturedDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    _shader
        .setLightPositions({
            {camera.cameraMatrix().transformPoint({-3.0f, 10.0f, 10.0f}), 0.0f}
        });
}

void MainWindow::drawEvent(){
    GL::Renderer::setClearColor(_clearcolor);
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

    
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

    _camera->draw(_drawables);

    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::enable(GL::Renderer::Feature::Blending);

    swapBuffers();
    redraw();
}






void MainWindow::pointerPressEvent(PointerEvent& event) {
    if(!event.isPrimary() ||
       !(event.pointer() & (Pointer::MouseLeft|Pointer::Finger)))
        return;

    _previousPosition = positionOnSphere(event.position());
}

void MainWindow::pointerReleaseEvent(PointerEvent& event) {
    if(!event.isPrimary() ||
       !(event.pointer() & (Pointer::MouseLeft|Pointer::Finger)))
        return;

    _previousPosition = {};
}

void MainWindow::scrollEvent(ScrollEvent& event) {
    if(!event.offset().y()) return;

    /* Distance to origin */
    const Float distance = _cameraObject.transformation().translation().z();

    /* Move 15% of the distance back or forward */
    _cameraObject.translate(Vector3::zAxis(
        distance*(1.0f - (event.offset().y() > 0 ? 1/0.85f : 0.85f))));

    redraw();
}

Vector3 MainWindow::positionOnSphere(const Vector2& position) const {
    const Vector2 positionNormalized =
        position/Vector2{_camera->viewport()} - Vector2{0.5f};
    const Float length = positionNormalized.length();
    const Vector3 result = length > 1.0f ?
        Vector3{positionNormalized, 0.0f} : 
                Vector3{positionNormalized, 1.0f - length};
    return (result*Vector3::yScale(-1.0f)).normalized();

    
}

void MainWindow::pointerMoveEvent(PointerMoveEvent& event) {
    if(!event.isPrimary() ||
       !(event.pointers() & (Pointer::MouseLeft|Pointer::Finger)))
        return;

    const Vector3 currentPosition = positionOnSphere(event.position());
    const Vector3 axis = Math::cross(_previousPosition, currentPosition);

    if(_previousPosition.isZero() || axis.isZero())
        return;

    _manipulator.rotate(Math::angle(_previousPosition, currentPosition), axis.normalized());
    _previousPosition = currentPosition;

    redraw();
}


MAGNUM_APPLICATION_MAIN(MainWindow)

