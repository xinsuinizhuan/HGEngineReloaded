#pragma once

// C++ STL
#include <map>

// HG::Utils
#include <DoubleBufferContainer.hpp>

namespace HG::Core
{
    class Application;
    class GameObject;
}

namespace HG::Rendering::Base
{
    class RenderBehaviour;
    class AbstractRenderDataProcessor;
    class RenderData;

    /**
     * @brief Class, that describes
     * abstract rendering pipeline.
     */
    class RenderingPipeline
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit RenderingPipeline(HG::Core::Application* application);

        /**
         * @brief Desturctor.
         */
        virtual ~RenderingPipeline();

        /**
         * @brief Method for initializing pipeline.
         * By default it just initialize system controller
         * and creates window.
         */
        virtual bool init();

        /**
         * @brief Method for deinitialization pipeline.
         * By default it just deinitialize system controller
         * and creates window.
         */
        virtual void deinit();

        /**
         * @brief Rendering method.
         * @param objects Objects.
         */
        virtual void render(const HG::Utils::DoubleBufferContainer<HG::Core::GameObject*>& objects) = 0;

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Core::Application* application() const;

        /**
         * @brief Method for setting up render behaviour.
         * @param data Pointer to render behaviour.
         */
        virtual bool setup(HG::Rendering::Base::RenderData* data);

        /**
         * @brief Method for adding render data processor.
         * That will setup any render data.
         * @param processor Pointer to processor.
         */
        HG::Rendering::Base::RenderingPipeline* addRenderDataProcessor(HG::Rendering::Base::AbstractRenderDataProcessor* processor);

    protected:

        /**
         * @brief Method, that's called, when render behaviour has to be
         * set up.
         * @param behaviour Pointer to behaviour.
         */
        virtual bool setupRenderBehaviour(HG::Rendering::Base::RenderBehaviour* behaviour);

    private:

        HG::Core::Application* m_parentApplication;

        std::map<
            std::size_t,
            HG::Rendering::Base::AbstractRenderDataProcessor*
        > m_renderDataProcessor;
    };
}

