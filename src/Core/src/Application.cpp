// C++ STL
#include <stdexcept>

// HG::Physics::Base
#include <HG/Physics/Base/PhysicsController.hpp>

// HG::Core
#include <HG/Core/CountStatistics.hpp>
#include <HG/Core/ResourceManager.hpp>
#include <HG/Core/BuildProperties.hpp>
#include <HG/Core/TimeStatistics.hpp>
#include <HG/Core/Application.hpp>
#include <HG/Core/ThreadPool.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Core/Input.hpp>
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/ResourceCache.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/SystemController.hpp>


HG::Core::Application::Application(std::string name, int /* argc */, char** /* argv */) :
    m_applicationTitle(std::move(name)),
    m_renderer(nullptr),
    m_systemController(nullptr),
    m_physicsController(nullptr),
    m_threadPool(new HG::Core::ThreadPool()),
    m_input(new HG::Core::Input()),
    m_resourceManager(new HG::Core::ResourceManager(this)),
    m_timeStatistics(new HG::Core::TimeStatistics()),
    m_countStatistics(new HG::Core::CountStatistics()),
    m_benchmark(new HG::Core::Benchmark()),
    m_resourceCache(new HG::Core::ResourceCache()),
    m_currentScene(nullptr),
    m_cachedScene(nullptr)
{
    m_renderer = new HG::Rendering::Base::Renderer(this);
}

HG::Core::Application::~Application()
{
    delete m_cachedScene;
    delete m_currentScene;

    delete m_systemController;
    delete m_physicsController;

    delete m_renderer;
    delete m_resourceCache;
    delete m_benchmark;
    delete m_countStatistics;
    delete m_timeStatistics;
    delete m_resourceManager;
    delete m_input;
    delete m_threadPool;
}


void HG::Core::Application::setSystemController(HG::Rendering::Base::SystemController* systemController)
{
    m_systemController = systemController;
}

void HG::Core::Application::setPhysicsController(HG::Physics::Base::PhysicsController* physicsController)
{
    m_physicsController = physicsController;
}

HG::Physics::Base::PhysicsController* HG::Core::Application::physicsController()
{
    return m_physicsController;
}

void HG::Core::Application::setScene(HG::Core::Scene* scene)
{
    if (scene == nullptr)
    {
        throw std::runtime_error("New scene can't be nullptr.");
    }

    delete m_cachedScene;
    scene->setApplication(this);
    m_cachedScene = scene;
}

HG::Core::Scene* HG::Core::Application::scene() const
{
    if (m_cachedScene != nullptr)
    {
        return m_cachedScene;
    }

    return m_currentScene;
}

void HG::Core::Application::stop()
{
    const_cast<Input::Window*>(m_input->window())->setClosed(true);
}

bool HG::Core::Application::init()
{
    return m_renderer->init();
}

void HG::Core::Application::deinit()
{
    m_renderer->deinit();
}

std::string HG::Core::Application::title() const
{
    return m_applicationTitle;
}

bool HG::Core::Application::performCycle()
{
    // Saving last deltatime
    auto dt = m_timeStatistics->tickTimerAtomic(TimeStatistics::FrameTime);

    // Ticking benchmark
    m_benchmark->tick();

    // Tick counting frame time

    if (m_physicsController)
    {
        BENCH_D(this, "Physics tick");
        // Start counting physics time
        m_timeStatistics->tickTimerBegin(TimeStatistics::PhysicsTime);

        // Processing physics, if available
        m_physicsController->tick(dt);

        // Finish counting physics time
        m_timeStatistics->tickTimerEnd(TimeStatistics::PhysicsTime);
    }

    // Start counting update time (events are in update section)
    m_timeStatistics->tickTimerBegin(TimeStatistics::UpdateTime);

    // Polling events
    if (m_renderer->pipeline() != nullptr &&
        systemController() != nullptr)
    {
        BENCH_D(this, "Events polling");
        systemController()->pollEvents();
    }

    // Checking for new scene, etc
    proceedScene();

    {
        BENCH_D(this, "Updating");
        // Calling update on scene.
        if (m_currentScene)
        {
            m_currentScene->update();
        }

        // Finishing counting update time
        m_timeStatistics->tickTimerEnd(TimeStatistics::UpdateTime);
    }

    {
        BENCH_D(this, "Rendering");

        // Start counting rendering time
        m_timeStatistics->tickTimerBegin(TimeStatistics::RenderTime);

        // Executing rendering pipeline.
        if (m_currentScene)
        {
            m_currentScene->render(m_renderer);
        }

        // Finishing counting rendering time
        m_timeStatistics->tickTimerEnd(TimeStatistics::RenderTime);
    }

    m_countStatistics->frameChanged();

    return true;
}

int HG::Core::Application::exec()
{
    // Preparing deltaTime calculation

    while (!m_input->window()->isClosed())
    {
        performCycle();
    }

    delete m_currentScene;
    m_currentScene = nullptr;

    delete m_cachedScene;
    m_cachedScene = nullptr;

    deinit();

    return 0;
}

void HG::Core::Application::proceedScene()
{
    BENCH_D(this, "Scene processing");
    if (m_cachedScene != nullptr)
    {
        // Deleting current scene
        delete m_currentScene;
        m_currentScene = m_cachedScene;
        m_cachedScene = nullptr;

        // Calling start method
        m_currentScene->start();
    }
}

HG::Rendering::Base::Renderer* HG::Core::Application::renderer()
{
    return m_renderer;
}

HG::Core::ResourceManager* HG::Core::Application::resourceManager()
{
    return m_resourceManager;
}

HG::Core::TimeStatistics* HG::Core::Application::timeStatistics()
{
    return m_timeStatistics;
}

HG::Core::CountStatistics *HG::Core::Application::countStatistics()
{
    return m_countStatistics;
}

HG::Core::Benchmark *HG::Core::Application::benchmark()
{
    return m_benchmark;
}

HG::Core::ResourceCache *HG::Core::Application::resourceCache()
{
    return m_resourceCache;
}

HG::Core::ThreadPool* HG::Core::Application::threadPool()
{
    return m_threadPool;
}

const HG::Core::Input *HG::Core::Application::input() const
{
    return m_input;
}

HG::Rendering::Base::SystemController *HG::Core::Application::systemController() const
{
    return m_systemController;
}
