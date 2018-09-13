#pragma once

// HG::Rendering::Base
#include <RenderSpecificData.hpp> // Required for inheritance

// GLM
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for cube map.
     */
    class CubeMapTextureData : public HG::Rendering::Base::RenderSpecificData
    {
    public:

        gl::cubemap_texture Texture;
    };
}
