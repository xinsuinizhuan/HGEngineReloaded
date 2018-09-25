#pragma once

// HG::Rendering::Base
#include <RenderSpecificData.hpp> // Required for inheritance

// gl
#include <gl/all.hpp>

// GLM
#include <glm/vec2.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for render target behaviour.
     */
    class RenderTargetData : public HG::Rendering::Base::RenderSpecificData
    {
    public:

        RenderTargetData() :
            Framebuffer(),
            Renderbuffer(),
            Size()
        {}

        RenderTargetData(gl::framebuffer f,
                         gl::renderbuffer r,
                         glm::ivec2 s) :
             Framebuffer(std::move(f)),
             Renderbuffer(std::move(r)),
             Size(s)
        {}

        gl::framebuffer Framebuffer;
        gl::renderbuffer Renderbuffer;
        glm::ivec2 Size;
    };
}

