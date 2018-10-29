// Orbital example
#include <Assets/Scenes/OrbitalScene.hpp>

// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/ResourceManager.hpp>

// HG::Standard
#include <HG/Utils/FilesystemResourceAccessor.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/GLFWSystemController.hpp>
#include <Forward/RenderingPipeline.hpp>
#include <Forward/MeshRenderer.hpp>
#include <Common/MeshDataProcessor.hpp>
#include <Common/Texture2DDataProcessor.hpp>
#include <Common/ShaderDataProcessor.hpp>
#include <Common/CubeMapTextureDataProcessor.hpp>
#include <Common/RenderTargetDataProcessor.hpp>
#include <Forward/CubeMapRenderer.hpp>

// ALogger
#include <CurrentLogger.hpp>
#include <Loggers/BasicLogger.hpp>

int main(int argc, char** argv)
{
    CurrentLogger::setCurrentLogger(std::make_shared<Loggers::BasicLogger>());

    InfoF() << "Creating application";
    HG::Core::Application application("HEEngine Orbital Example", argc, argv);

    // Setting resource accessor implementation
    application.resourceManager()
        ->setResourceAccessor(new HG::Standard::FilesystemResourceAccessor());

    // Setting system controller
    application.setSystemController(new HG::Rendering::OpenGL::GLFWSystemController(&application));

    auto pipeline = new HG::Rendering::OpenGL::Forward::RenderingPipeline(&application);
    pipeline->addRenderer(new HG::Rendering::OpenGL::Forward::MeshRenderer());
    pipeline->addRenderer(new HG::Rendering::OpenGL::Forward::CubeMapRenderer());
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::MeshDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::Texture2DDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::ShaderDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::CubeMapTextureDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::RenderTargetDataProcessor);

    // Setting rendering to forward
    application.renderer()->setPipeline(pipeline);

    if (!application.init())
    {
        ErrorF() << "Can't init application.";
        return -1;
    }

    application.setScene(new OrbitalScene());

    return application.exec();
}