#pragma once

#include <Scene.hpp>
#include "RenderingPipeline.hpp"
#include "RenderBehaviour.hpp"
#include "Gizmos.hpp"
#include "MaterialCollection.hpp"

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes
     * layer between rendering pipeline and
     * user behaviour rendering system.
     */
    class Renderer
    {
    public:

        /**
         * @brief Default constructor.
         */
        explicit Renderer(::CORE_MODULE_NS::Application* application);

        /**
         * @brief Destructor.
         */
        ~Renderer();

        /**
         * @brief Method for setting pipeline
         * object.
         * @tparam PipelineType Type of pipeline object.
         */
        template<typename PipelineType>
        typename std::enable_if<
            std::is_base_of<RenderingPipeline, PipelineType>::value,
            PipelineType*
        >::type
        setPipeline()
        {
            m_pipeline = new PipelineType(m_parentApplication);
            return static_cast<PipelineType*>(m_pipeline);
        }

        /**
         * @brief Method for getting pipeline object.
         * @return Pointer to pipeline object.
         */
        RenderingPipeline* pipeline();

        /**
         * @brief Method for initializing pipeline.
         */
        bool init();

        /**
         * @brief Perform rendering finally.
         */
        void render(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& gameObjects);

        /**
         * @brief Method for getting gizmos object.
         * @return Pointer to gizmos.
         */
        ::RENDERING_BASE_MODULE_NS::Gizmos* gizmos();

        /**
         * @brief Method for getting renderer material collection.
         * @return Pointer to material collection.
         */
        ::RENDERING_BASE_MODULE_NS::MaterialCollection* materialCollection();

        /**
         * @brief Method for setting up objects
         * with specified rendering pipeline.
         * @param obj Object.
         */
        template<typename T>
        void setup(T obj)
        {
            if (m_pipeline == nullptr)
            {
                Error() << "Can't setup rendering behaviour without set pipeline.";
                return;
            }

            m_pipeline->setup(obj);
        }

    private:

        ::CORE_MODULE_NS::Application* m_parentApplication;

        ::RENDERING_BASE_MODULE_NS::RenderingPipeline* m_pipeline;

        ::RENDERING_BASE_MODULE_NS::Gizmos m_gizmos;

        ::RENDERING_BASE_MODULE_NS::MaterialCollection m_materialCollection;

    };
}

