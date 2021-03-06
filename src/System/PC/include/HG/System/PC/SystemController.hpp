#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/SystemController.hpp>

// Forward declaration
struct GLFWwindow;

namespace HG::System::PC
{
/**
 * @brief Class, that describes system controller, using GLFW library.
 */
class SystemController : public HG::Rendering::Base::SystemController
{
public:
    /**
     * @brief Constructor.
     */
    explicit SystemController(HG::Core::Application* application);

    /**
     * @brief Method, that's closing window on destruction.
     */
    ~SystemController() override;

    void changeTitle(std::string title) override;

    /**
     * @brief Method for swapping buffers in window.
     */
    void swapBuffers() override;

    Utils::Rect viewport() const override;

    void closeWindow() override;

    bool isWindowFocused() override;

protected:
    /**
     * @brief Method, that's calling `glfwPollEvents`.
     */
    void onPollEvents() override;

    /**
     * @brief Method for creating window with defines width, height and title.
     * @param width Initial width.
     * @param height Initial height.
     * @param title Window title.
     * @return Success.
     */
    bool onCreateWindow(std::uint32_t width, std::uint32_t height, std::string title) override;

    /**
     * @brief Method for initializing GLFW, setting GLFW settings and setting callbacks.
     * @return Success.
     */
    bool onInit() override;

    /**
     * @brief Method for deinitializing. Does nothing for now...
     * @todo Do something...
     */
    void onDeinit() override;

private:
    /**
     * @brief Method for handling gamepad events.
     */
    void handleGamepadsEvents();

    /**
     * @brief Method for handling window events.
     */
    void handleWindowEvents();

    /**
     * @brief GLFW callback for input.
     * @param c Scancode.
     */
    static void charCallback(GLFWwindow*, unsigned int c);

    /**
     * @brief GLFW callback for pressing keys.
     * @param key Key code.
     * @param action Action (Press/release)
     */
    static void keyPressCallback(GLFWwindow*, int key, int, int action, int);

    /**
     * @brief GLFW callback for moving cursor.
     * @param x X coordinate, locally to window.
     * @param y Y coordinate, locally to window.
     */
    static void cursorPosCallback(GLFWwindow*, double x, double y);

    /**
     * @brief GLFW callback for connecting/disconnecting
     * gamepads.
     * @param gamepad Gamepad index.
     * @param event Event.
     */
    static void joystickCallback(int gamepad, int event);

    /**
     * @brief GLFW callback for mouse buttons.
     * @param button Mouse button index.
     * @param action Action code.
     */
    static void mouseButtonCallback(GLFWwindow*, int button, int action, int);

    /**
     * @brief GLFW callback for mouse wheel.
     * @param xDelta X delta.
     * @param yDelta Y delta.
     */
    static void mouseWheelCallback(GLFWwindow*, double xDelta, double yDelta);

    /**
     * @brief GLFW callback for framebuffer size changed.
     * @param width New width.
     * @param height New height.
     */
    static void framebufferSizeCallback(GLFWwindow*, int width, int height);

    GLFWwindow* m_window;
};
} // namespace HG::System::PC
