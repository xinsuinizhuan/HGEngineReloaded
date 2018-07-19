#pragma once

#include <RenderBehaviour.hpp>
#include <Texture.hpp>

namespace RENDERING_BASE_MODULE_NS::Behaviours
{
    /**
     * @brief Class, that describes sprite render behaviour.
     */
    class Sprite : public RenderBehaviour
    {
    public:

        static constexpr std::size_t Id = __COUNTER__;

        /**
         * @brief Constructor.
         */
        Sprite();

        /**
         * @brief Destructor.
         */
        ~Sprite() override;

        /**
         * @brief Method for setting texture for sprite.
         * @param texture Pointer to texture.
         */
        void setTexture(Texture* texture);

        /**
         * @brief Method for getting sprite texture.
         * @return Pointer to texture.
         */
        Texture* texture() const;

    private:
        Texture* m_texture;
    };
}

