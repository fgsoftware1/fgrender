#include "include/mouse.hpp"
#include "include/keyboard.hpp"
#include "include/window.hpp"
#include <iostream>

using namespace std;

namespace fgengine{
    namespace input{
        Mouse::Mouse() :
            m_window(nullptr)
        {}

        Mouse::Mouse(GLFWwindow* window) :
            m_window(window)
        {}

        void Mouse::setCursorPos(double x, double y)
        {
            glfwSetCursorPos(m_window, x, y);
        }

        void Mouse::getCursorPos(double* x, double* y)
        {
            glfwGetCursorPos(m_window, x, y);
        }

        void Mouse::setShowCursor(cursorState cursor_state)
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, (int) cursor_state);
        }

        void Mouse::setMouseMoveCallback(std::function<void(double, double)> callback)
        {
            std::get<0>(m_callbacks) = std::move(callback);

            glfwSetCursorPosCallback(m_window, [] (GLFWwindow* window, double x, double y) {
                auto& data = std::get<1>(*static_cast<fgengine::gfx::Window::Callbacks_t*>(glfwGetWindowUserPointer(window)));
                std::get<0>(data)(x, y);
            });
        }

        void Mouse::addMouseClicCallback(button button, eventType type, std::function<void(double, double)> callback)
        {
            std::get<1>(m_callbacks).emplace_back(button, type, std::move(callback));

            glfwSetMouseButtonCallback(m_window, [] (GLFWwindow* window, int button, int action, int mods) {
                auto& callbacks = std::get<1>(*static_cast<fgengine::gfx::Window::Callbacks_t*>(glfwGetWindowUserPointer(window)));
                auto& clics = std::get<1>(callbacks);
                for (auto& f : clics)
                {
                    if ((int) std::get<0>(f) == button && (int) std::get<1>(f) == action)
                    {
                        double x{0.0}, y{0.0}; glfwGetCursorPos(window, &x, &y);
                        std::get<2>(f)(x, y);
                    }
                }
            });
        }

        const Mouse::Callbacks_t& Mouse::getUserData()
        {
            return m_callbacks;
        }
    }
}