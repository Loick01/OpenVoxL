#include "core/window.hpp"

Window::Window(const std::string& title):
    m_title(title)
{
    InitGlfw();

    // Should not be here (glad must be initialized after glfw) ? 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");
}

Window::~Window()
{
    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

GLFWwindow* Window::GetGlfwWindow()
{
    return m_glfwWindow;
}

unsigned int Window::GetWidth() const
{
    return m_width;
}

unsigned int Window::GetHeight() const
{
    return m_height;
}

void Window::InitGlfw()
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor)
        throw std::runtime_error("glfwGetPrimaryMonitor failed");

    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
    if (!videoMode)
        throw std::runtime_error("glfwGetVideoMode failed");

    m_width = videoMode->width;
    m_height = videoMode->height;
    
    glfwWindowHint(GLFW_SAMPLES, 0); // For now I will not use MSAA 
    // glEnable(GL_MULTISAMPLE);

    // I wiil try to set 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), monitor, nullptr);
    if (m_glfwWindow == NULL)
        throw std::runtime_error("Failed to create GLFW window");

    glfwMakeContextCurrent(m_glfwWindow);
    // glfwSwapInterval(1); // ?
}

bool Window::EndFrame()
{
    glfwSwapBuffers(m_glfwWindow);
    return !glfwWindowShouldClose(m_glfwWindow);
}