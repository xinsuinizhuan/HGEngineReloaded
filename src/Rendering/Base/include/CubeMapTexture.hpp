#pragma once

#include <Surface.hpp>
#include <CurrentLogger.hpp>

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes cube map
     * texture for rendering (in GPU / prepared for GPU)
     */
    class CubeMapTexture
    {
    public:

        /**
         * @brief Cube side.
         */
        enum Side
        {
            Right,
            Left,
            Top,
            Bottom,
            Front,
            Back
        };

        /**
         * @brief Class, that describes abstract
         * external data for textures.
         */
        class CubeMapTextureExternalData
        {
        public:

            /**
             * @brief Defualt virtual constructor.
             */
            virtual ~CubeMapTextureExternalData() = default;
        };

        /**
         * @brief Constructor.
         */
        CubeMapTexture();

        /**
         * @brief Initialize constructor.
         * @param right Right side surface.
         * @param left Left side surface.
         * @param top Top side surface.
         * @param bottom Bottom side surface.
         * @param front Front side surface.
         * @param back Back side surface.
         */
        CubeMapTexture(::UTILS_MODULE_NS::SurfacePtr right,
                       ::UTILS_MODULE_NS::SurfacePtr left,
                       ::UTILS_MODULE_NS::SurfacePtr top,
                       ::UTILS_MODULE_NS::SurfacePtr bottom,
                       ::UTILS_MODULE_NS::SurfacePtr front,
                       ::UTILS_MODULE_NS::SurfacePtr back);

        /**
         * @brief Destructor. Clears external data.
         */
        ~CubeMapTexture();

        /**
         * @brief Method for getting external data, casted
         * to some type.
         * @tparam T Type to cast.
         * @return Pointer to external data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<CubeMapTextureExternalData, T>::value,
            T*
        >::type externalData() const
        {
            return static_cast<T*>(m_externalData);
        };

        /**
         * @brief Method for setting external data.
         * @tparam T Type of actual external data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<CubeMapTextureExternalData, T>::value
        >::type setExternalData()
        {
#ifndef NDEBUG
            if (m_externalData)
            {
                Info() << "Recreating existing external data for texture.";
            }
#endif

            delete m_externalData;

            m_externalData = new T();
        };

        /**
         * @brief Method for clearing external data.
         */
        void clearExternalData();

        /**
         * @brief Method for getting side surface.
         * @param side Side.
         * @return Smart pointer to surface.
         */
        ::UTILS_MODULE_NS::SurfacePtr getSideSurface(Side side) const;

        /**
         * @brief Method for setting side surface.
         * Can throw `std::invalid_argument` if side is
         * wrong.
         * @param side Side.
         * @param surface Smart pointer to surface.
         */
        void setSideSurface(Side side, ::UTILS_MODULE_NS::SurfacePtr surface);

    private:

        CubeMapTextureExternalData* m_externalData;

        // Surfaces
        ::UTILS_MODULE_NS::SurfacePtr m_right;
        ::UTILS_MODULE_NS::SurfacePtr m_left;
        ::UTILS_MODULE_NS::SurfacePtr m_top;
        ::UTILS_MODULE_NS::SurfacePtr m_bottom;
        ::UTILS_MODULE_NS::SurfacePtr m_front;
        ::UTILS_MODULE_NS::SurfacePtr m_back;
    };
}
