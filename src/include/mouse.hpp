#ifndef mouse_HPP
#define mouse_HPP

namespace fgengine{
    namespace input{
        class Mouse
        {
        public:
            using MouseMoveCallback = std::function<void(double, double)>;
            using MouseClickCallback = std::vector<
                std::tuple<
                    button,
                    eventType,
                    std::function<void(double, double)>
                >
            >;
            using Callbacks_t = std::tuple<
                MouseMoveCallback,
                MouseClickCallback
                >;
        
        private:
            GLFWwindow* m_window{};
            Callbacks_t m_callbacks;
        
        public:
            Mouse();
            Mouse(GLFWwindow* window);

            void setCursorPos(double x, double y);
            void getCursorPos(double* x, double* y);
            void setShowCursor(cursorState cursor_state);

            void setMouseMoveCallback(std::function<void(double, double)> callback);
            void addMouseClicCallback(button button, eventType type, std::function<void(double, double)> callback);

            const Callbacks_t& getUserData();
        };
    }
}

#endif