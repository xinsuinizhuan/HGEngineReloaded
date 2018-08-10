#pragma once

#include <Material.hpp>
#include "AbstractRenderer.hpp"
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Forward
{
    /**
     * @brief Class, that describes forward mesh
     * renderer.
     */
    class MeshRenderer : public AbstractRenderer
    {
    public:

        /**
         * @brief Constructor.
         */
        MeshRenderer();

        /**
         * @brief Destructor.
         */
        virtual ~MeshRenderer();

        void render(HG::Core::GameObject *gameObject,
                    HG::Rendering::Base::RenderBehaviour *renderBehaviour) override;

        size_t getTarget() override;

        void init() override;

    private:

        // Fallback mesh program
        HG::Rendering::Base::Material* m_meshFallbackMaterial;
    };
}


