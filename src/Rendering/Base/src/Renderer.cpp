// HG::Core
#include <Application.hpp>

// HG::Rendering::Base
#include <RenderingPipeline.hpp>
#include <MaterialCollection.hpp>
#include <Renderer.hpp>
#include <Camera.hpp>
#include <Gizmos.hpp>
#include <RenderTarget.hpp>

// ALogger
#include <CurrentLogger.hpp>

HG::Rendering::Base::Renderer::Renderer(HG::Core::Application* application) :
    m_parentApplication(application),
    m_pipeline(nullptr),
    m_gizmos(new HG::Rendering::Base::Gizmos()),
    m_materialCollection(new HG::Rendering::Base::MaterialCollection(application->resourceManager(), this)),
    m_activeCamera(nullptr),
    m_defaultRenderTarget(HG::Rendering::Base::RenderTarget::createDefault())
{
    Debug() << "Creating renderer.";
}

HG::Rendering::Base::Renderer::~Renderer()
{
    Debug() << "Destroying renderer.";
    delete m_pipeline;
    delete m_gizmos;
    delete m_materialCollection;
    delete m_defaultRenderTarget;
}

HG::Rendering::Base::RenderTarget* HG::Rendering::Base::Renderer::defaultRenderTarget() const
{
    return m_defaultRenderTarget;
}

void HG::Rendering::Base::Renderer::setPipeline(HG::Rendering::Base::RenderingPipeline* pipeline)
{
    m_pipeline = pipeline;
}

bool HG::Rendering::Base::Renderer::init()
{
    if (m_pipeline == nullptr)
    {
        Error() << "No rendering pipeline set.";
        return false;
    }

    return m_pipeline->init();
}

void HG::Rendering::Base::Renderer::deinit()
{
    if (m_pipeline == nullptr)
    {
        Error() << "No rendering pipeline set.";
        return;
    }

    return m_pipeline->deinit();
}

void HG::Rendering::Base::Renderer::render(const HG::Utils::DoubleBufferContainer<HG::Core::GameObject*>& gameObjects)
{
    if (m_pipeline == nullptr)
    {
        Error() << "No rendering pipeline set.";
        return;
    }

    m_pipeline->render(gameObjects);

    m_gizmos->clear();
}

HG::Rendering::Base::Gizmos *HG::Rendering::Base::Renderer::gizmos()
{
    return m_gizmos;
}

HG::Rendering::Base::RenderingPipeline* HG::Rendering::Base::Renderer::pipeline()
{
    return m_pipeline;
}

HG::Rendering::Base::MaterialCollection* HG::Rendering::Base::Renderer::materialCollection()
{
    return m_materialCollection;
}

HG::Rendering::Base::Camera *HG::Rendering::Base::Renderer::activeCamera() const
{
    return m_activeCamera;
}

void HG::Rendering::Base::Renderer::setActiveCamera(HG::Rendering::Base::Camera *camera)
{
    if (camera->gameObject() == nullptr)
    {
        return;
    }

    m_activeCamera = camera;
}

bool HG::Rendering::Base::Renderer::setup(HG::Rendering::Base::RenderData* data)
{
    if (m_pipeline == nullptr)
    {
        Info() << "There is no rendering pipeline to setup.";
        return false;
    }

    return m_pipeline->setup(data);
}

bool HG::Rendering::Base::Renderer::needSetup(HG::Rendering::Base::RenderData* data)
{
    if (m_pipeline == nullptr)
    {
        Info() << "There is no rendering pipeline to check for setup requirement.";
        return false;
    }

    return m_pipeline->needSetup(data);
}
