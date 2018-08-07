#pragma once

#include <Behaviour.hpp>
#include <glm/vec2.hpp>

namespace STD_MODULE_NS::Behaviours
{
    /**
     * @brief Behaviour for controlling debug gameobjects
     * control.
     */
    class DebugControllerOverlay : public ::CORE_MODULE_NS::Behaviour
    {
    public:

        template<typename Behaviour>
        typename std::enable_if<
            std::is_base_of<CORE_MODULE_NS::Behaviour, Behaviour>::value,
            DebugControllerOverlay*
        >::type addViewBehaviour()
        {

        }

    protected:

        void onUpdate() override;

    private:

        template<typename T>
        struct Finder
        {
            
        };

        void displayMenu();

        void displayGameObjectsWindow();

        void displayInspectorWindow();

        void proceedParentedGameObjects(::CORE_MODULE_NS::Transform* parent);

        void proceedInspector();

        void displayPropertyWidget(const ::CORE_MODULE_NS::Behaviour::Property& property);

        std::vector<::CORE_MODULE_NS::GameObject*> m_gameObjects;
        std::vector<::CORE_MODULE_NS::Behaviour*> m_behaviours;
        std::vector<::CORE_MODULE_NS::Behaviour::Property> m_properties;

        ::CORE_MODULE_NS::GameObject* m_activeGameObject = nullptr;

        std::vector<
            std::pair<
                std::string,
                std::function<::CORE_MODULE_NS::Behaviour*(::CORE_MODULE_NS::Scene*)>
            >
        > m_viewBehaviours;

        HG_PROPERTY_DEFAULT(glm::vec2, OriginSize, glm::vec2(0.3f, 0.3f));
        HG_PROPERTY_DEFAULT(::UTILS_MODULE_NS::Color, OriginColor, ::UTILS_MODULE_NS::Color::fromRGB(150, 180, 0));
    };
}


