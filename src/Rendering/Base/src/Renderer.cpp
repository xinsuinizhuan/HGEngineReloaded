#include <CurrentLogger.hpp>
#include "Renderer.hpp"
#include <Application.hpp>
#include <Renderer.hpp>


HG::Rendering::Base::Renderer::Renderer(HG::Core::Application* application) :
    m_parentApplication(application),
    m_pipeline(nullptr),
    m_materialCollection(application->resourceManager(), this)
{
    Debug() << "Creating renderer.";
}

HG::Rendering::Base::Renderer::~Renderer()
{
    delete m_pipeline;

    Debug() << "Destroying renderer.";
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

void HG::Rendering::Base::Renderer::render(const HG::Core::Scene::GameObjectsContainer& gameObjects)
{
    if (m_pipeline == nullptr)
    {
        Error() << "No rendering pipeline set.";
        return;
    }

    m_pipeline->render(gameObjects);

    m_gizmos.clear();
}

HG::Rendering::Base::Gizmos *HG::Rendering::Base::Renderer::gizmos()
{
    return &m_gizmos;
}

HG::Rendering::Base::RenderingPipeline* HG::Rendering::Base::Renderer::pipeline()
{
    return m_pipeline;
}

HG::Rendering::Base::MaterialCollection* HG::Rendering::Base::Renderer::materialCollection()
{
    return &m_materialCollection;
}
