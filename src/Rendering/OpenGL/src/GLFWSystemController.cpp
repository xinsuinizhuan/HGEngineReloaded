#include <CurrentLogger.hpp>
#include <Input.hpp>
#include <Application.hpp>
#include "GLFWSystemController.hpp"

#ifdef GRAPHICS_USE_GLFW

OGL_RENDERING_MODULE_NS::GLFWSystemController::GLFWSystemController(::CORE_MODULE_NS::Application* application) :
    SystemController(application),
    m_window(nullptr)
{

}

OGL_RENDERING_MODULE_NS::GLFWSystemController::~GLFWSystemController()
{
    glfwTerminate();
    m_window = nullptr;
}

bool OGL_RENDERING_MODULE_NS::GLFWSystemController::init()
{
    Info() << "Initializing GLFW";

    // Setting error callback
    glfwSetErrorCallback(
        [](int code, const char* description)
        {
            ErrorF() << "GLFW Received error #" << code << ", " << description;
        }
    );

    // Initializing GLFW
    if (!glfwInit())
    {
        return false;
    }

    // Setting OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

// Incognito
namespace
{
    OGL_RENDERING_MODULE_NS::GLFWSystemController* controller = nullptr;
}

bool OGL_RENDERING_MODULE_NS::GLFWSystemController::createWindow(uint32_t width, uint32_t height, std::string title)
{
    Info() << "Creating window " << width << "x" << height << " with title \"" << title << "\"";

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        glfwTerminate();
        return false;
    }

    controller = this;

    // Setting event callbacks
    glfwSetKeyCallback        (m_window, &GLFWSystemController::keyPressCallback);
    glfwSetCursorPosCallback  (m_window, &GLFWSystemController::cursorPosCallback);
    glfwSetMouseButtonCallback(m_window, &GLFWSystemController::mouseButtonCallback);
    glfwSetJoystickCallback   (          &GLFWSystemController::joystickCallback);

    // todo: Add char callback
//    glfwSetCharCallback(m_window, []());

    glfwMakeContextCurrent(m_window);

    return true;
}

void OGL_RENDERING_MODULE_NS::GLFWSystemController::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

void OGL_RENDERING_MODULE_NS::GLFWSystemController::pollEvents()
{
    glfwPollEvents();
    handleGamepadsEvents();
    handleWindowEvents();
}

void OGL_RENDERING_MODULE_NS::GLFWSystemController::handleGamepadsEvents()
{
    for (auto index = GLFW_JOYSTICK_1; index <= GLFW_JOYSTICK_LAST; ++index)
    {
        int present = glfwJoystickPresent(index);

        const_cast<::CORE_MODULE_NS::Input::Gamepads*>(
            controller->application()->input()->gamepads()
        )->setIsConnectedGamepad(
            static_cast<uint8_t>(index),
            static_cast<bool>(present)
        );

        if (!present)
        {
            continue;
        }

        int count;

        // Axes
        const float* axes = glfwGetJoystickAxes(index, &count);

        for (int i = 0; i < count; ++i)
        {
            const_cast<::CORE_MODULE_NS::Input::Gamepads*>(
                controller->application()->input()->gamepads()
            )->setGamepadAxisValue(
                static_cast<uint8_t>(index),
                static_cast<uint8_t>(i),
                axes[i]
            );
        }

        // Buttons
        const auto buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

        for (int i = 0; i < count; ++i)
        {
            if (controller->application()
                          ->input()
                          ->gamepads()
                          ->isButtonPressed(
                              static_cast<uint8_t>(index),
                              static_cast<uint8_t>(i)
                          ))
            {
                if (buttons[i] == GLFW_RELEASE)
                {
                    const_cast<::CORE_MODULE_NS::Input::Gamepads*>(
                        controller->application()->input()->gamepads()
                    )->setGamepadButtonValue(
                        static_cast<uint8_t>(index),
                        static_cast<uint8_t>(i),
                        false
                    );
                }
            }
            else
            {
                if (buttons[i] == GLFW_PRESS)
                {
                    const_cast<::CORE_MODULE_NS::Input::Gamepads*>(
                        controller->application()->input()->gamepads()
                    )->setGamepadButtonValue(
                        static_cast<uint8_t>(index),
                        static_cast<uint8_t>(i),
                        true
                    );
                }
            }
        }
    }
}

void OGL_RENDERING_MODULE_NS::GLFWSystemController::joystickCallback(int gamepad, int event)
{
    if (event == GLFW_CONNECTED)
    {
        InfoEx("OpenGL::RendererController") << "Gamepad #" << gamepad << " connected.";

        const_cast<::CORE_MODULE_NS::Input::Gamepads*>(
            controller->application()->input()->gamepads()
        )->setIsConnectedGamepad(static_cast<uint8_t>(gamepad), true);
    }
    else if (event == GLFW_DISCONNECTED)
    {
        InfoEx("OpenGL::RendererController") << "Gamepad #" << gamepad << " disconnected.";

        const_cast<::CORE_MODULE_NS::Input::Gamepads*>(
            controller->application()->input()->gamepads()
        )->setIsConnectedGamepad(static_cast<uint8_t>(gamepad), false);
    }
}

void OGL_RENDERING_MODULE_NS::GLFWSystemController::cursorPosCallback(GLFWwindow*, double x, double y)
{
    const_cast<::CORE_MODULE_NS::Input::Mouse*>(
        controller->application()->input()->mouse()
    )->setMousePosition(
        static_cast<int>(x),
        static_cast<int>(y)
    );
}

void OGL_RENDERING_MODULE_NS::GLFWSystemController::keyPressCallback(GLFWwindow*, int key, int, int action, int)
{
    static std::map<int, ::CORE_MODULE_NS::Input::Keyboard::Key> keys = {
        {GLFW_KEY_0,                ::CORE_MODULE_NS::Input::Keyboard::Key::N0},
        {GLFW_KEY_1,                ::CORE_MODULE_NS::Input::Keyboard::Key::N1},
        {GLFW_KEY_2,                ::CORE_MODULE_NS::Input::Keyboard::Key::N2},
        {GLFW_KEY_3,                ::CORE_MODULE_NS::Input::Keyboard::Key::N3},
        {GLFW_KEY_4,                ::CORE_MODULE_NS::Input::Keyboard::Key::N4},
        {GLFW_KEY_5,                ::CORE_MODULE_NS::Input::Keyboard::Key::N5},
        {GLFW_KEY_6,                ::CORE_MODULE_NS::Input::Keyboard::Key::N6},
        {GLFW_KEY_7,                ::CORE_MODULE_NS::Input::Keyboard::Key::N7},
        {GLFW_KEY_8,                ::CORE_MODULE_NS::Input::Keyboard::Key::N8},
        {GLFW_KEY_9,                ::CORE_MODULE_NS::Input::Keyboard::Key::N9},
        {GLFW_KEY_SPACE,            ::CORE_MODULE_NS::Input::Keyboard::Key::Space},
        {GLFW_KEY_APOSTROPHE,       ::CORE_MODULE_NS::Input::Keyboard::Key::SingleQuotation},
        {GLFW_KEY_COMMA,            ::CORE_MODULE_NS::Input::Keyboard::Key::Comma},
        {GLFW_KEY_MINUS,            ::CORE_MODULE_NS::Input::Keyboard::Key::Minus},
        {GLFW_KEY_PERIOD,           ::CORE_MODULE_NS::Input::Keyboard::Key::Dot},
        {GLFW_KEY_SLASH,            ::CORE_MODULE_NS::Input::Keyboard::Key::Slash},
        {GLFW_KEY_SEMICOLON,        ::CORE_MODULE_NS::Input::Keyboard::Key::Semicolon},
        {GLFW_KEY_EQUAL,            ::CORE_MODULE_NS::Input::Keyboard::Key::Equal},
        {GLFW_KEY_A,                ::CORE_MODULE_NS::Input::Keyboard::Key::A},
        {GLFW_KEY_B,                ::CORE_MODULE_NS::Input::Keyboard::Key::B},
        {GLFW_KEY_C,                ::CORE_MODULE_NS::Input::Keyboard::Key::C},
        {GLFW_KEY_D,                ::CORE_MODULE_NS::Input::Keyboard::Key::D},
        {GLFW_KEY_E,                ::CORE_MODULE_NS::Input::Keyboard::Key::E},
        {GLFW_KEY_F,                ::CORE_MODULE_NS::Input::Keyboard::Key::F},
        {GLFW_KEY_G,                ::CORE_MODULE_NS::Input::Keyboard::Key::G},
        {GLFW_KEY_H,                ::CORE_MODULE_NS::Input::Keyboard::Key::H},
        {GLFW_KEY_I,                ::CORE_MODULE_NS::Input::Keyboard::Key::I},
        {GLFW_KEY_J,                ::CORE_MODULE_NS::Input::Keyboard::Key::J},
        {GLFW_KEY_K,                ::CORE_MODULE_NS::Input::Keyboard::Key::K},
        {GLFW_KEY_L,                ::CORE_MODULE_NS::Input::Keyboard::Key::L},
        {GLFW_KEY_M,                ::CORE_MODULE_NS::Input::Keyboard::Key::M},
        {GLFW_KEY_N,                ::CORE_MODULE_NS::Input::Keyboard::Key::N},
        {GLFW_KEY_O,                ::CORE_MODULE_NS::Input::Keyboard::Key::O},
        {GLFW_KEY_P,                ::CORE_MODULE_NS::Input::Keyboard::Key::P},
        {GLFW_KEY_Q,                ::CORE_MODULE_NS::Input::Keyboard::Key::Q},
        {GLFW_KEY_R,                ::CORE_MODULE_NS::Input::Keyboard::Key::R},
        {GLFW_KEY_S,                ::CORE_MODULE_NS::Input::Keyboard::Key::S},
        {GLFW_KEY_T,                ::CORE_MODULE_NS::Input::Keyboard::Key::T},
        {GLFW_KEY_U,                ::CORE_MODULE_NS::Input::Keyboard::Key::U},
        {GLFW_KEY_V,                ::CORE_MODULE_NS::Input::Keyboard::Key::V},
        {GLFW_KEY_W,                ::CORE_MODULE_NS::Input::Keyboard::Key::W},
        {GLFW_KEY_X,                ::CORE_MODULE_NS::Input::Keyboard::Key::X},
        {GLFW_KEY_Y,                ::CORE_MODULE_NS::Input::Keyboard::Key::Y},
        {GLFW_KEY_Z,                ::CORE_MODULE_NS::Input::Keyboard::Key::Z},
        {GLFW_KEY_LEFT_BRACKET,     ::CORE_MODULE_NS::Input::Keyboard::Key::SquareBracketOpen},
        {GLFW_KEY_BACKSLASH,        ::CORE_MODULE_NS::Input::Keyboard::Key::Backslash},
        {GLFW_KEY_RIGHT_BRACKET,    ::CORE_MODULE_NS::Input::Keyboard::Key::SquareBracketClose},
        {GLFW_KEY_GRAVE_ACCENT,     ::CORE_MODULE_NS::Input::Keyboard::Key::Tilda}, // todo: Add world keys here
        {GLFW_KEY_ESCAPE,           ::CORE_MODULE_NS::Input::Keyboard::Key::ESC},
        {GLFW_KEY_ENTER,            ::CORE_MODULE_NS::Input::Keyboard::Key::Return},
        {GLFW_KEY_TAB,              ::CORE_MODULE_NS::Input::Keyboard::Key::Tab},
        {GLFW_KEY_BACKSPACE,        ::CORE_MODULE_NS::Input::Keyboard::Key::Backspace},
        {GLFW_KEY_INSERT,           ::CORE_MODULE_NS::Input::Keyboard::Key::Insert},
        {GLFW_KEY_DELETE,           ::CORE_MODULE_NS::Input::Keyboard::Key::Delete},
        {GLFW_KEY_RIGHT,            ::CORE_MODULE_NS::Input::Keyboard::Key::ArrowRight},
        {GLFW_KEY_LEFT,             ::CORE_MODULE_NS::Input::Keyboard::Key::ArrowLeft},
        {GLFW_KEY_DOWN,             ::CORE_MODULE_NS::Input::Keyboard::Key::ArrowDown},
        {GLFW_KEY_UP,               ::CORE_MODULE_NS::Input::Keyboard::Key::ArrowUp},
        {GLFW_KEY_PAGE_UP,          ::CORE_MODULE_NS::Input::Keyboard::Key::PageUp},
        {GLFW_KEY_PAGE_DOWN,        ::CORE_MODULE_NS::Input::Keyboard::Key::PageDown},
        {GLFW_KEY_HOME,             ::CORE_MODULE_NS::Input::Keyboard::Key::Home},
        {GLFW_KEY_END,              ::CORE_MODULE_NS::Input::Keyboard::Key::End},
        {GLFW_KEY_CAPS_LOCK,        ::CORE_MODULE_NS::Input::Keyboard::Key::CapsLock},
        {GLFW_KEY_NUM_LOCK,         ::CORE_MODULE_NS::Input::Keyboard::Key::NumLock},
        {GLFW_KEY_PRINT_SCREEN,     ::CORE_MODULE_NS::Input::Keyboard::Key::PrintScreen},
        {GLFW_KEY_PAUSE,            ::CORE_MODULE_NS::Input::Keyboard::Key::Pause},
        {GLFW_KEY_F1,               ::CORE_MODULE_NS::Input::Keyboard::Key::F1},
        {GLFW_KEY_F2,               ::CORE_MODULE_NS::Input::Keyboard::Key::F2},
        {GLFW_KEY_F3,               ::CORE_MODULE_NS::Input::Keyboard::Key::F3},
        {GLFW_KEY_F4,               ::CORE_MODULE_NS::Input::Keyboard::Key::F4},
        {GLFW_KEY_F5,               ::CORE_MODULE_NS::Input::Keyboard::Key::F5},
        {GLFW_KEY_F6,               ::CORE_MODULE_NS::Input::Keyboard::Key::F6},
        {GLFW_KEY_F7,               ::CORE_MODULE_NS::Input::Keyboard::Key::F7},
        {GLFW_KEY_F8,               ::CORE_MODULE_NS::Input::Keyboard::Key::F8},
        {GLFW_KEY_F9,               ::CORE_MODULE_NS::Input::Keyboard::Key::F9},
        {GLFW_KEY_F10,              ::CORE_MODULE_NS::Input::Keyboard::Key::F10},
        {GLFW_KEY_F11,              ::CORE_MODULE_NS::Input::Keyboard::Key::F11},
        {GLFW_KEY_F12,              ::CORE_MODULE_NS::Input::Keyboard::Key::F12},
        {GLFW_KEY_F13,              ::CORE_MODULE_NS::Input::Keyboard::Key::F13},
        {GLFW_KEY_F14,              ::CORE_MODULE_NS::Input::Keyboard::Key::F14},
        {GLFW_KEY_F15,              ::CORE_MODULE_NS::Input::Keyboard::Key::F15},
        {GLFW_KEY_F16,              ::CORE_MODULE_NS::Input::Keyboard::Key::F16},
        {GLFW_KEY_F17,              ::CORE_MODULE_NS::Input::Keyboard::Key::F17},
        {GLFW_KEY_F18,              ::CORE_MODULE_NS::Input::Keyboard::Key::F18},
        {GLFW_KEY_F19,              ::CORE_MODULE_NS::Input::Keyboard::Key::F19},
        {GLFW_KEY_F20,              ::CORE_MODULE_NS::Input::Keyboard::Key::F20},
        {GLFW_KEY_F21,              ::CORE_MODULE_NS::Input::Keyboard::Key::F21},
        {GLFW_KEY_F22,              ::CORE_MODULE_NS::Input::Keyboard::Key::F22},
        {GLFW_KEY_F23,              ::CORE_MODULE_NS::Input::Keyboard::Key::F23},
        {GLFW_KEY_F24,              ::CORE_MODULE_NS::Input::Keyboard::Key::F24},
        {GLFW_KEY_F25,              ::CORE_MODULE_NS::Input::Keyboard::Key::F25},
        {GLFW_KEY_KP_0,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num0},
        {GLFW_KEY_KP_1,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num1},
        {GLFW_KEY_KP_2,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num2},
        {GLFW_KEY_KP_3,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num3},
        {GLFW_KEY_KP_4,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num4},
        {GLFW_KEY_KP_5,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num5},
        {GLFW_KEY_KP_6,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num6},
        {GLFW_KEY_KP_7,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num7},
        {GLFW_KEY_KP_8,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num8},
        {GLFW_KEY_KP_9,             ::CORE_MODULE_NS::Input::Keyboard::Key::Num9},
        {GLFW_KEY_KP_DECIMAL,       ::CORE_MODULE_NS::Input::Keyboard::Key::NumDot},
        {GLFW_KEY_KP_DIVIDE,        ::CORE_MODULE_NS::Input::Keyboard::Key::NumDivide},
        {GLFW_KEY_KP_MULTIPLY,      ::CORE_MODULE_NS::Input::Keyboard::Key::NumMultiply},
        {GLFW_KEY_KP_SUBTRACT,      ::CORE_MODULE_NS::Input::Keyboard::Key::NumSubstract},
        {GLFW_KEY_KP_ADD,           ::CORE_MODULE_NS::Input::Keyboard::Key::NumAdd},
        {GLFW_KEY_KP_ENTER,         ::CORE_MODULE_NS::Input::Keyboard::Key::NumReturn},
        {GLFW_KEY_KP_EQUAL,         ::CORE_MODULE_NS::Input::Keyboard::Key::NumEqual},
        {GLFW_KEY_LEFT_SHIFT,       ::CORE_MODULE_NS::Input::Keyboard::Key::LeftShift},
        {GLFW_KEY_LEFT_CONTROL,     ::CORE_MODULE_NS::Input::Keyboard::Key::LeftCtrl},
        {GLFW_KEY_LEFT_ALT,         ::CORE_MODULE_NS::Input::Keyboard::Key::LeftAlt},
        {GLFW_KEY_LEFT_SUPER,       ::CORE_MODULE_NS::Input::Keyboard::Key::Super},
        {GLFW_KEY_RIGHT_SHIFT,      ::CORE_MODULE_NS::Input::Keyboard::Key::RightShift},
        {GLFW_KEY_RIGHT_CONTROL,    ::CORE_MODULE_NS::Input::Keyboard::Key::RightCtrl},
        {GLFW_KEY_RIGHT_ALT,        ::CORE_MODULE_NS::Input::Keyboard::Key::RightAlt},
        {GLFW_KEY_MENU,             ::CORE_MODULE_NS::Input::Keyboard::Key::Menu}
    };

    static std::map<int, ::CORE_MODULE_NS::Input::Keyboard::Modifiers> modifiers = {
        {GLFW_KEY_LEFT_ALT,         ::CORE_MODULE_NS::Input::Keyboard::Modifiers::Alt},
        {GLFW_KEY_RIGHT_ALT,        ::CORE_MODULE_NS::Input::Keyboard::Modifiers::Alt},
        {GLFW_KEY_LEFT_SHIFT,       ::CORE_MODULE_NS::Input::Keyboard::Modifiers::Shift},
        {GLFW_KEY_RIGHT_SHIFT,      ::CORE_MODULE_NS::Input::Keyboard::Modifiers::Shift},
        {GLFW_KEY_LEFT_CONTROL,     ::CORE_MODULE_NS::Input::Keyboard::Modifiers::Ctrl},
        {GLFW_KEY_RIGHT_CONTROL,    ::CORE_MODULE_NS::Input::Keyboard::Modifiers::Ctrl},
    };

    {
        auto iter = keys.find(key);

        if (iter != keys.end())
        {
            if (action == GLFW_PRESS)
            {
                const_cast<::CORE_MODULE_NS::Input::Keyboard*>(
                    controller->application()->input()->keyboard()
                )->setPressed(iter->second, true);
            }
            else if (action == GLFW_RELEASE)
            {
                const_cast<::CORE_MODULE_NS::Input::Keyboard*>(
                    controller->application()->input()->keyboard()
                )->setPressed(iter->second, false);
            }

        }
    }

    {
        auto iter = modifiers.find(key);

        if (iter != modifiers.end())
        {
            if (action == GLFW_PRESS)
            {
                const_cast<::CORE_MODULE_NS::Input::Keyboard*>(
                    controller->application()->input()->keyboard()
                )->setPressed(iter->second, true);
            }
            else if (action == GLFW_RELEASE)
            {
                const_cast<::CORE_MODULE_NS::Input::Keyboard*>(
                    controller->application()->input()->keyboard()
                )->setPressed(iter->second, false);
            }
        }
    }
}

void OGL_RENDERING_MODULE_NS::GLFWSystemController::mouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    const_cast<::CORE_MODULE_NS::Input::Mouse*>(
        controller->application()->input()->mouse()
    )->setPressedButton(
        static_cast<uint8_t>(button),
        static_cast<bool>(action)
    );
}

void OGL_RENDERING_MODULE_NS::GLFWSystemController::handleWindowEvents()
{
    const_cast<::CORE_MODULE_NS::Input::Window*>(
        controller->application()->input()->window()
    )->setClosed(static_cast<bool>(glfwWindowShouldClose(m_window)));
}

#endif