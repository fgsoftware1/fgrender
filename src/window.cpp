#include "include/glad.h"
#include "include/window.hpp"
#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

namespace fgengine{
    namespace editor{
        inline void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
        {
            if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

            switch (source)
            {
                case GL_DEBUG_SOURCE_API:             std::cout << "[API] "; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "[Window System] "; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "[Shader Compiler] "; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "[Third Party] "; break;
                case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "[Application] "; break;
                case GL_DEBUG_SOURCE_OTHER:           std::cout << "[Other] "; break;
            }

            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR:               std::cout << "[Error] "; break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "[Deprecated Behaviour] "; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "[Undefined Behaviour] "; break; 
                case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "[Portability] "; break;
                case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "[Performance] "; break;
                case GL_DEBUG_TYPE_MARKER:              std::cout << "[Marker] "; break;
                case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "[Push Group] "; break;
                case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "[Pop Group] "; break;
                case GL_DEBUG_TYPE_OTHER:               std::cout << "[Other] "; break;
            }
            
            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:         std::cout << "[high] "; break;
                case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "[medium] "; break;
                case GL_DEBUG_SEVERITY_LOW:          std::cout << "[low] "; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "[notification] "; break;
            }
            std::cout << "(" << id << "): " <<  message << std::endl;
        }

        gfx::Window::Window(int width, int height, const std::string& name, flag_t flags, uint8_t aa_samples) : m_width(width), m_height(height), m_name(name), m_flags(flags)
        {
            if (!glfwInit())
                throw std::runtime_error("(fgengine::gfx::Window) Failed to initialize GLFW");

            glfwWindowHint(GLFW_SAMPLES, aa_samples);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            if (m_flags & Window::DEBUG)
                 glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            if (m_flags & Window::MAXSIZE)
            {
                const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                m_width = vidmode->width;
                m_height = vidmode->height;
            }

            m_window = glfwCreateWindow(m_width, m_height, name.c_str(), nullptr, nullptr);
            if (m_window == nullptr)
            {
                glfwTerminate();
                throw std::runtime_error("(fgengine::gfx::Window) Could not create window, OpenGL >= 3.3 required");
            }
            glfwMakeContextCurrent(m_window);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                throw std::runtime_error("(fgengine::gfx::Window) Failed to initialize GLAD");

            if (m_flags & Window::DEBUG)
            {
                GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
                if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
                {
                    glEnable(GL_DEBUG_OUTPUT);
                    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
                    glDebugMessageCallback(glDebugOutput, nullptr);
                    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
                }
                else
                    std::cerr << "(fgengine::gfx::Window) Couldn't initialize debug context" << std::endl;
            }

            m_keyboard = input::Keyboard(m_window);
            m_mouse = input::Mouse(m_window);
            
            glViewport(0, 0, m_width, m_height); 

            if (m_flags & Window::RESIZE)
                glfwSetFramebufferSizeCallback(m_window, [] (GLFWwindow* window, int w, int h) {
                    glViewport(0, 0, w, h);
                    auto& f = std::get<0>(std::get<2>(*static_cast<Window::Callbacks_t*>(glfwGetWindowUserPointer(window))));
                    auto& g = std::get<1>(std::get<2>(*static_cast<Window::Callbacks_t*>(glfwGetWindowUserPointer(window))));
                    f(w, h);
                    g(w, h);
                });
            
            setVSync(m_flags & Window::VSYNC);

            glEnable(GL_DEPTH_TEST);
        }

        gfx::Window::~Window()
        {
            glfwTerminate();
            m_window = nullptr;
        }

        bool gfx::Window::shouldClose() const
        {
            return glfwWindowShouldClose(m_window);
        }

        void gfx::Window::close()
        {
            glfwSetWindowShouldClose(m_window, true);
        }

        void gfx::Window::pollEvents()
        {
            glfwPollEvents();
        }

        void gfx::Window::display()
        {
            glfwSwapBuffers(m_window);
        }

        void gfx::Window::clear(gfx::Color color)
        {
            glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void gfx::Window::setVSync(bool val)
        {
            if (!val)
                glfwSwapInterval(0);
        }

        void gfx::Window::setTitle(const std::string& name)
        {
            glfwSetWindowTitle(m_window, name.c_str());
        }

        void gfx::Window::onResize(std::function<void(int, int)> callback)
        {
            std::get<0>(m_data) = [this] (int w, int h) {
                this->m_width = w;
                this->m_height = h;
            };
            std::get<1>(m_data) = std::move(callback);
        }

        float gfx::Window::getAspectRatio() const
        {
            return ((float) m_width) / m_height;
        }

        float gfx::Window::getWidth() const
        {
            return (float) m_width;
        }

        float gfx::Window::getHeight() const
        {
            return (float) m_height;
        }

        void gfx::Window::updateCallbacks()
        {
            std::get<0>(m_callbacks) = m_keyboard.getUserData();
            std::get<1>(m_callbacks) = m_mouse.getUserData();
            std::get<2>(m_callbacks) = m_data;

            glfwSetWindowUserPointer(m_window, &m_callbacks);
        }

        input::Keyboard& gfx::Window::getKeyboard()
        {
            return m_keyboard;
        }

        input::Mouse& gfx::Window::getMouse()
        {
            return m_mouse;
        }

        GLFWwindow* gfx::Window::getPtr()
        {
            return m_window;
        }
    }
}