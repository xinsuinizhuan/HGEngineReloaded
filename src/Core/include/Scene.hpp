#pragma once

#include <chrono>
#include <UniqueMergeContainer.hpp>
#include "Application.hpp"
#include "GameObject.hpp"

namespace CORE_MODULE_NS
{
    /**
     * @brief Class, that describes
     * application scene.
     */
    class Scene
    {
    public:

        /**
         * @brief Constructor.
         */
        Scene();

        /**
         * @brief Destructor.
         */
        virtual ~Scene() = default;

        /**
         * @brief Method for setting application
         * instance.
         * @param application Pointer to application.
         */
        void setApplication(Application* application);

        /**
         * @brief Method for getting current application
         * instance.
         * @return Pointer to application.
         */
        Application* application() const;

        /**
         * @brief Method, that's called every frame.
         */
        void update();

        /**
         * @brief Method, that's called on scene start.
         */
        void start();

        /**
         * @brief Method for removing gameobject
         * from scene.
         * @param gameObject GameObject.
         */
        void removeGameObject(GameObject* gameObject);

        /**
         * @brief Method for adding new gameObject.
         * If gameObject will be `nullptr`,
         * `std::runtime_error` exception will
         * be thrown.
         * @param gameObject Pointer to GameObject.
         */
        void addGameObject(GameObject* gameObject);

    private:
        Application* m_mainApplication;

        UTILS_MODULE_NS::UniqueMergeContainer<GameObject*> m_gameObjects;
    };
}

