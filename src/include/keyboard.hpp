#ifndef keyboard_HPP
#define keyboad_HPP

#include <GLFW/glfw3.h>
#include <tuple>
#include <vector>
#include <functional>

#include "events.hpp"

namespace fgengine{
    namespace input{
         class Keyboard
        {
        public:
            using Callbacks_t = std::vector<
                std::tuple<
                    int, fgengine::input::key, std::function<void()>
                >>;
        
        private:
            GLFWwindow* m_window{};
            Callbacks_t m_callbacks;

            void updateCallbacks();
        
        public:
            Keyboard();
            Keyboard(GLFWwindow* window);

            void addCallback(int eventtype, fgengine::input::key key, std::function<void()> callback);
        
            const Callbacks_t& getUserData();
        };
    }
}

#endif