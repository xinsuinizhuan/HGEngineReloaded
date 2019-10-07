// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/AbstractRenderDataProcessor.hpp>
#include <HG/Rendering/Base/RenderBehaviour.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/SystemController.hpp>

// ALogger
#include <CurrentLogger.hpp>

HG::Rendering::Base::RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
    m_parentApplication(application),
    m_currentRenderTarget(nullptr),
    m_renderDataProcessor(),
    m_renderOverride(nullptr)
{
}

HG::Rendering::Base::RenderingPipeline::~RenderingPipeline()
{
    for (auto&& processor : m_renderDataProcessor)
    {
        delete processor.second;
    }
}

bool HG::Rendering::Base::RenderingPipeline::init()
{
    if (m_parentApplication == nullptr)
    {
        Error() << "No parent application set.";
        return false;
    }

    if (m_parentApplication->systemController() == nullptr)
    {
        Error() << "No SystemController set in rendering pipeline.";
        return false;
    }

    if (!m_parentApplication->systemController()->init())
    {
        Error() << "Can't init rendering pipeline by system controller. See errors above.";
        return false;
    }

    if (!m_parentApplication->systemController()->createWindow(800, 600, m_parentApplication->title()))
    {
        Error() << "Can't create window. See errors above.";
        return false;
    }

    if (!m_parentApplication->renderer())
    {
        Error() << "No parent renderer. Can't set default render target.";
        return false;
    }

    setRenderTarget(m_parentApplication->renderer()->defaultRenderTarget());

    return true;
}

void HG::Rendering::Base::RenderingPipeline::deinit()
{
    if (m_parentApplication == nullptr)
    {
        Error() << "No parent application set.";
    }

    if (m_parentApplication->systemController() == nullptr)
    {
        Error() << "No SystemController set in rendering pipeline.";
    }

    m_parentApplication->systemController()->deinit();

    m_parentApplication->systemController()->closeWindow();
}

HG::Core::Application* HG::Rendering::Base::RenderingPipeline::application() const
{
    return m_parentApplication;
}

bool HG::Rendering::Base::RenderingPipeline::setup(HG::Rendering::Base::RenderData* data, bool guarantee)
{
    BENCH("Setup of resource " + std::to_string(data->dataType()));

    if (data->dataType() == HG::Rendering::Base::RenderBehaviour::RenderDataId)
    {
        return setupRenderBehaviour(dynamic_cast<HG::Rendering::Base::RenderBehaviour*>(data), guarantee);
    }

    // Search for data processor.
    auto processorIterator = m_renderDataProcessor.find(data->dataType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        Error() << "Can't setup render data with " << data->dataType() << " type. No processor.";
        return false;
    }

    return processorIterator->second->setup(data, guarantee);
}

bool HG::Rendering::Base::RenderingPipeline::needSetup(HG::Rendering::Base::RenderData* data)
{
    BENCH("Checking is setup required for " + std::to_string(data->dataType()));
    if (data->dataType() == HG::Rendering::Base::RenderBehaviour::RenderDataId)
    {
        return needSetupRenderBehaviour(dynamic_cast<HG::Rendering::Base::RenderBehaviour*>(data));
    }

    // Search for data processor.
    auto processorIterator = m_renderDataProcessor.find(data->dataType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        Error() << "Can't check is render data with type " << data->dataType()
                << " requires to be set up. No processor.";
        return false;
    }

    return processorIterator->second->needSetup(data);
}

bool HG::Rendering::Base::RenderingPipeline::setupRenderBehaviour(HG::Rendering::Base::RenderBehaviour* behaviour,
                                                                  bool guarantee)
{
    BENCH("Setup of rendering behaviour " + std::to_string(behaviour->renderBehaviourType()));
    auto processorIterator = m_renderDataProcessor.find(behaviour->renderBehaviourType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        Error() << "Can't setup rendering behaviour's render data with " << behaviour->renderBehaviourType()
                << " type. No processor.";
        return false;
    }

    return processorIterator->second->setup(behaviour, guarantee);
}

bool HG::Rendering::Base::RenderingPipeline::needSetupRenderBehaviour(HG::Rendering::Base::RenderBehaviour* behaviour)
{
    BENCH("Is setup required for rendering behaviour " + std::to_string(behaviour->renderBehaviourType()));
    auto processorIterator = m_renderDataProcessor.find(behaviour->renderBehaviourType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        Error() << "Can't check rendering behaviour's setup requirement with " << behaviour->renderBehaviourType()
                << " type. No processor.";
        return false;
    }

    return processorIterator->second->needSetup(behaviour);
}

HG::Rendering::Base::RenderingPipeline* HG::Rendering::Base::RenderingPipeline::addRenderDataProcessor(
    HG::Rendering::Base::AbstractRenderDataProcessor* processor)
{
    processor->setRenderingPipeline(this);
    m_renderDataProcessor[processor->getTarget()] = processor;

    return this;
}

void HG::Rendering::Base::RenderingPipeline::setRenderTarget(HG::Rendering::Base::RenderTarget* target)
{
    m_currentRenderTarget = target;
}

HG::Rendering::Base::RenderTarget* HG::Rendering::Base::RenderingPipeline::renderTarget() const
{
    return m_currentRenderTarget;
}

void HG::Rendering::Base::RenderingPipeline::setRenderOverride(HG::Rendering::Base::RenderOverride* renderOverride)
{
    m_renderOverride = renderOverride;
}

HG::Rendering::Base::RenderOverride* HG::Rendering::Base::RenderingPipeline::renderOverride() const
{
    return m_renderOverride;
}
