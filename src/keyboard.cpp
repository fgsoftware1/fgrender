#include "include/keyboard.hpp"
#include "include/window.hpp"
#include "iostream"

using namespace std;

namespace fgengine{
    namespace input{
        static const int EventIndex = 0, KeyIndex = 1, CallbackIndex = 2;

        void Keyboard::updateCallbacks()
        {
            glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
                auto& callbacks = std::get<0>(*static_cast<fgengine::gfx::Window::Callbacks_t*>(glfwGetWindowUserPointer(window)));
                
                for (auto& f : callbacks)
                {
                    int ev_idx = std::get<EventIndex>(f);

                    if (((ev_idx & (int) eventType::Released) != 0 && action == GLFW_RELEASE) ||
                        ((ev_idx & (int) eventType::Pressed) != 0  && action == GLFW_PRESS)   ||
                        ((ev_idx & (int) eventType::Repeat) != 0   && action == GLFW_REPEAT))
                        if ((int) std::get<KeyIndex>(f) == key)
                            std::get<CallbackIndex>(f)();
                }
            });
        }

        Keyboard::Keyboard()
        {}

        Keyboard::Keyboard(GLFWwindow* window) :
            m_window(window)
        {
            glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 1);
        }

        void Keyboard::addCallback(int eventtype, key key, std::function<void()> callback)
        {
            m_callbacks.emplace_back(eventtype, key, std::move(callback));
            updateCallbacks();
        }

        const Keyboard::Callbacks_t& Keyboard::getUserData()
        {
            return m_callbacks;
        }
    }
}