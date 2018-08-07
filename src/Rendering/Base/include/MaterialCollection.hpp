#pragma once

#include "Material.hpp"
#include <utility>
#include <ResourceManager.hpp>
#include <Loaders/StringLoader.hpp>

namespace RENDERING_BASE_MODULE_NS
{
    class Renderer;

    /**
     * @brief Class, that describes
     * material management system. It caches
     * shader information and generates materials
     * with cached shaders.
     */
    class MaterialCollection
    {
    public:

        /**
         * @brief Constructor.
         * @param Root application.
         */
        explicit MaterialCollection(CORE_MODULE_NS::ResourceManager* resourceManager,
                                    RENDERING_BASE_MODULE_NS::Renderer* pipeline);

        /**
         * @brief Destructor.
         */
        ~MaterialCollection();

        /**
         * @brief Method for getting material
         * (and loading shader if required)
         * @tparam MaterialType Type of material.
         * @return Material.
         */
        template<typename MaterialType>
        typename std::enable_if<
            std::is_base_of<Material, MaterialType>::value,
            MaterialType*
        >::type getMaterial()
        {
            auto typeHash = typeid(MaterialType).hash_code();

            auto shader = m_shaders.find(typeHash);

            if (shader == m_shaders.end())
            {
                prepareMaterial<MaterialType>();
            }

            shader = m_shaders.find(typeHash);

            auto newMaterial = new MaterialType;

            newMaterial->setShader(shader->second);

            return newMaterial;
        }

        /**
         * @brief Method for preparing material's shader.
         * @tparam MaterialType Type of material.
         * @return Material.
         */
        template< typename MaterialType >
        typename std::enable_if<
            std::is_base_of<Material, MaterialType>::value
        >::type prepareMaterial()
        {
            static_assert(has_raw_text<MaterialType>::value != sizeof(typename has_raw_text<MaterialType>::hasNothing),
                          "Material does not contains `rawShader` or `shaderPath`, or it cannot be accessed.");

            auto typeHash = typeid(MaterialType).hash_code();

            if (m_shaders.find(typeHash) != m_shaders.end())
            {
                return;
            }

            // Creating shader
            auto newShader = new Shader();

            if constexpr (has_raw_text<MaterialType>::value ==
                          sizeof(typename has_raw_text<MaterialType>::hasRawData))
            {
                newShader->setShaderText(MaterialType::rawShader);

                m_shaders[typeHash] = newShader;
            }
            else if constexpr (has_raw_text<MaterialType>::value ==
                               sizeof(typename has_raw_text<MaterialType>::hasPaths))
            {
                auto shaderData = m_resourceManager->load<UTILS_MODULE_NS::StringLoader>(MaterialType::shaderPath);

                newShader->setShaderText(shaderData);
            }

            setup(newShader);
        }

    private:

        // RAW shader text test
        template<typename T>
        class has_raw_text
        {

        public:

            typedef uint8_t hasRawData;
            typedef uint16_t hasPaths;
            typedef uint32_t hasNothing;

        private:

            template<typename C> static hasRawData test(typeof(C::rawShader));

            template<typename C> static hasPaths test(typeof(C::shaderPath));

            template<typename C> static hasNothing test(...);

        public:
            enum { value = sizeof(test<T>(0)) };
        };

        /**
         * @brief Method for setting up shader. (Method
         * implemented to decouple Renderer.hpp and MaterialCollection.hpp.
         * @param shader Pointer to shader.
         */
        void setup(Shader* shader);

        CORE_MODULE_NS::ResourceManager* m_resourceManager;

        std::map<std::size_t, Shader*> m_shaders;
        RENDERING_BASE_MODULE_NS::Renderer* m_renderer;
    };
}
