#include "Behaviours/FPSCameraMovement.hpp"

STD_MODULE_NS::Behaviours::FPSCameraMovement::FPSCameraMovement() :
    m_enabled(false),
    m_first(false),
    m_lastMousePosition(),
    m_front(glm::vec3(0.0f, 0.0f, 1.0f)),
    m_yaw(90),
    m_pitch(0),
    m_camera(nullptr),
    m_sensitivity(3)
{

}

void STD_MODULE_NS::Behaviours::FPSCameraMovement::setSensitivity(float sens)
{
    m_sensitivity = sens;
}

float STD_MODULE_NS::Behaviours::FPSCameraMovement::sensitivity() const
{
    return m_sensitivity;
}

void STD_MODULE_NS::Behaviours::FPSCameraMovement::onStart()
{
    m_camera = gameObject()->findBehaviour<::RENDERING_BASE_MODULE_NS::Camera>();

    if (!m_camera)
    {
        Warning() << "Can't setup FPS camera movement, without camera.";
    }
}

void STD_MODULE_NS::Behaviours::FPSCameraMovement::onUpdate()
{
    if (m_camera == nullptr)
    {
        return;
    }

    if (input()->keyboard()->isPushed(
        ::CORE_MODULE_NS::Input::Keyboard::Key::R
    ))
    {
        m_enabled = !m_enabled;

        if (m_enabled)
        {
            m_lastMousePosition = input()->mouse()->getMousePosition();
        }

        input()->mouse()->setCursorHidden  (m_enabled);
        input()->mouse()->setCursorDisabled(m_enabled);
    }

    if (input()->keyboard()->isPushed(
        ::CORE_MODULE_NS::Input::Keyboard::Key::ESC
    ))
    {
        m_enabled = false;

        input()->mouse()->setCursorHidden  (false);
        input()->mouse()->setCursorDisabled(false);
    }

    if (m_enabled)
    {
        handleMouseRotation();

        handleKeyboardMovement();
    }
}

void HG::Standard::Behaviours::FPSCameraMovement::handleMouseRotation()
{
    // Frame sens
    float frameSensitivity =
        m_sensitivity / 10;

    // Mouse
    auto mousePosition = input()->mouse()->getMousePosition();
    mousePosition -= m_lastMousePosition;

    m_yaw += mousePosition.x * frameSensitivity;
    m_pitch += mousePosition.y * frameSensitivity;

    m_lastMousePosition = input()->mouse()->getMousePosition();

    if (m_pitch >= 89.0f)
    {
        m_pitch = 89.0f;
    }

    if (m_pitch <= -89.0f)
    {
        m_pitch = -89.0f;
    }

    m_front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    m_front.y = std::sin(glm::radians(m_pitch));
    m_front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);

    m_camera->lookAt(
        m_camera->gameObject()->transform()->localPosition() + m_front
    );
}

void HG::Standard::Behaviours::FPSCameraMovement::handleKeyboardMovement()
{
    auto dt = scene()
                  ->application()
                  ->timeStatistics()
                  ->lastFrameDeltaTime().count() / 1000000.0;

    auto input = scene()->application()->input()->keyboard();

    glm::vec3 inputDirection(0.0f, 0.0f, 0.0f);

    float speed = static_cast<float>(2.0f * dt);

    if (input->isPressed(CORE_MODULE_NS::Input::Keyboard::Key::Q))
    {
        inputDirection.y -= speed;
    }

    if (input->isPressed(CORE_MODULE_NS::Input::Keyboard::Key::E))
    {
        inputDirection.y += speed;
    }

    if (input->isPressed(CORE_MODULE_NS::Input::Keyboard::Key::W))
    {
        inputDirection.z -= speed;
    }

    if (input->isPressed(CORE_MODULE_NS::Input::Keyboard::Key::S))
    {
        inputDirection.z += speed;
    }

    if (input->isPressed(CORE_MODULE_NS::Input::Keyboard::Key::A))
    {
        inputDirection.x -= speed;
    }

    if (input->isPressed(CORE_MODULE_NS::Input::Keyboard::Key::D))
    {
        inputDirection.x += speed;
    }

    gameObject()->transform()->setGlobalPosition(
        gameObject()->transform()->globalPosition() +
            inputDirection * m_camera->gameObject()->transform()->globalRotation()
    );
}