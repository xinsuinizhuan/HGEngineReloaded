#pragma once

#include <glm/vec2.hpp>
#include <type_traits>
#include <memory>
#include "RenderData.hpp"

namespace HG::Utils
{
    class Surface;

    using SurfacePtr = std::shared_ptr<Surface>;
}

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes texture
     * for rendering (in GPU / prepared for GPU).
     */
    class Texture : public RenderData
    {
    public:

        static constexpr std::size_t DataId = 3;

        /**
         * @brief Texture filtering methods.
         */
        enum Filtering
        {
            Nearest,
            Linear
        };

        /**
         * @brief Texture wrapping methods.
         */
        enum Wrapping
        {
            Repeat,
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder
        };

        /**
         * @brief Constructor.
         */
        Texture();

        /**
         * @brief Constructor from surface.
         */
        explicit Texture(HG::Utils::SurfacePtr ptr);

        /**
         * @brief Method for getting binded surface to
         * texture.
         * @return Surface.
         */
        HG::Utils::SurfacePtr surface() const;

        /**
         * @brief Method for setting surface to texture.
         * @param ptr Pointer to surface.
         */
        void setSurface(HG::Utils::SurfacePtr ptr);

        /**
         * @brief Method for getting texture size in pixels.
         * @return Texture size in pixels.
         */
        glm::ivec2 size() const;

        /**
         * @brief Method for changing current magnification
         * method. This value change requires setup in renderer.
         * @param value Method value.
         */
        void setMagnificationMethod(Filtering value);

        /**
         * @brief Method for getting current magnification
         * method.
         * @return Method value.
         */
        Filtering magnificationMethod() const;

        /**
         * @brief Method for changing current minification
         * method. This value change requires setup in renderer.
         * @param value Method value.
         */
        void setMinificationMethod(Filtering value);

        /**
         * @brief Method for getting current minification
         * method.
         * @return Method value.
         */
        Filtering minificationMethod() const;

        /**
         * @brief Method for setting s coordinate wrapping
         * method.
         * @param method Method value.
         */
        void setSWrapping(Wrapping method);

        /**
         * @brief Method for getting s coordinate wrapping
         * method.
         * @return Method value.
         */
        Wrapping sWrapping() const;

        /**
         * @brief Method for setting t coordinate wrapping
         * method.
         * @param method Method value.
         */
        void setTWrapping(Wrapping method);

        /**
         * @brief Method for getting t coordinate wrapping
         * method.
         * @return Method value.
         */
        Wrapping tWrapping() const;

    private:
        HG::Utils::SurfacePtr m_surface;

        Filtering m_minFiltering;
        Filtering m_magFiltering;
        Wrapping m_sWrapping;
        Wrapping m_tWrapping;

        glm::ivec2 m_size;

    };
}

