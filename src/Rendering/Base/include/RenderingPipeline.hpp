#pragma once

#include "Scene.hpp"

namespace HG::Core
{
    class Application;
}

namespace HG::Rendering::Base
{
    class RenderBehaviour;

    class Texture;
    class Shader;
    class CubeMapTexture;

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
        virtual ~RenderingPipeline() = default;

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
        virtual void render(const HG::Core::Scene::GameObjectsContainer& objects) = 0;

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Core::Application* application() const;

        /**
         * @brief Method for setting up render behaviour.
         * @param behaviour Pointer to render behaviour.
         */
        virtual void setup(RenderBehaviour* behaviour) = 0;

        /**
         * @brief Method for setting up textures for rendering.
         * @param texture Pointer to texture.
         */
        virtual void setup(Texture* texture) = 0;

        /**
         * @brief Method for setting up shaders for rendering.
         * @param shader Pointer to shader.
         */
        virtual void setup(Shader* shader) = 0;

        /**
         * @brief Method for setting up cube map textures for rendering.
         * @param texture Pointer to cube map.
         */
        virtual void setup(CubeMapTexture* texture) = 0;

    private:

        HG::Core::Application* m_parentApplication;
    };
}

