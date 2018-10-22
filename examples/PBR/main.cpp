// Assets
#include <Assets/Scenes/TestScene.hpp>

// HG::Core
#include <Application.hpp>
#include <ResourceManager.hpp>

// HG::Standard
#include <FilesystemResourceAccessor.hpp>

// HG::Rendering::Base
#include <Renderer.hpp>

// HG::Rendering::OpenGL
#include <GLFWSystemController.hpp>
#include <Forward/RenderingPipeline.hpp>
#include <Forward/MeshRenderer.hpp>
#include <Common/MeshDataProcessor.hpp>
#include <Common/Texture2DDataProcessor.hpp>
#include <Common/ShaderDataProcessor.hpp>

// ALogger
#include <CurrentLogger.hpp>
#include <Loggers/BasicLogger.hpp>
#include <Common/RenderTargetDataProcessor.hpp>

int main(int argc, char** argv)
{
    CurrentLogger::setCurrentLogger(std::make_shared<Loggers::BasicLogger>());

    InfoF() << "Creating application";
    HG::Core::Application application("HEEngine PBR Example", argc, argv);

    // Setting resource accessor implementation
    application.resourceManager()
            ->setResourceAccessor(new HG::Standard::FilesystemResourceAccessor());

    // Setting system controller
    application.setSystemController(new HG::Rendering::OpenGL::GLFWSystemController(&application));

    auto pipeline = new HG::Rendering::OpenGL::Forward::RenderingPipeline(&application);
    pipeline->addRenderer(new HG::Rendering::OpenGL::Forward::MeshRenderer());
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::MeshDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::Texture2DDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::ShaderDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::RenderTargetDataProcessor);

    // Setting rendering to forward
    application.renderer()->setPipeline(pipeline);

    if (!application.init())
    {
        ErrorF() << "Can't init application.";
        return -1;
    }

    application.setScene(new TestScene());

    return application.exec();
}