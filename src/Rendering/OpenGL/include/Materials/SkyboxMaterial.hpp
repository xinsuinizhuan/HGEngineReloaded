#pragma once

// HG::Rendering::Base
#include <Material.hpp> // Required for inheritance


namespace HG::Rendering::OpenGL::Materials
{
    class SkyboxMaterial : public HG::Rendering::Base::Material
    {
    public:
        static const char* rawShader;
    };
}

