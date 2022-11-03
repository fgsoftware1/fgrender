#ifndef windows_HPP
#define window_HPP

#include <GLFW/glfw3.h>
#include <string>
#include <cinttypes>
#include <tuple>
#include <vector>
#include <functional>

#include "keyboard.hpp"
#include "mouse.hpp"
#include "colors.hpp"

namespace fgengine
{
    namespace gfx
    {
        class Window
        {
        public:
            using Data_t = std::tuple<
                std::function<void(int, int)>, std::function<void(int, int)>
            >;

            using Callbacks_t = std::tuple<
                input::Keyboard::Callbacks_t,
                input::Mouse::Callbacks_t,
                Window::Data_t
            >;

        private:
            using flag_t = unsigned int;

            GLFWwindow* m_window{};
            int m_width{}, m_height{};
            std::string m_name{};
            flag_t m_flags{};

            input::Keyboard m_keyboard;
            input::Mouse m_mouse;
            Callbacks_t m_callbacks;
            Data_t m_data;
        
        public:
            Window(int width, int height, const std::string& name, flag_t flags=0, uint8_t aa_samples=4);
            ~Window();

            bool shouldClose() const;
            void close();
            void pollEvents();
            void display();
            void clear(gfx::Color color=gfx::Color::Black);

            void setVSync(bool val);
            void setTitle(const std::string& name);
            void onResize(std::function<void(int, int)> callback);

            float getAspectRatio() const;
            float getWidth() const;
            float getHeight() const;
            
            input::Keyboard& getKeyboard();
            input::Mouse& getMouse();
            GLFWwindow* getPtr();

            void updateCallbacks();

            static const flag_t RESIZE = 1 << 1,
                                VSYNC = 1 << 2,
                                MAXSIZE = 1 << 3,
                                DEBUG = 1 << 4;
        };
    }
}

#endif