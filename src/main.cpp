#include "iostream"
#include "include/window.hpp"
#include "include/time.hpp"
#include "glm/glm.hpp"

using namespace std;

template <typename T> void echo(T str)
{
    std::cout << str << std::endl;
}

constexpr unsigned int WIDTH = 800, HEIGHT = 600;

int main(int argc, char** argv)
{
    auto window = fgengine::gfx::Window(WIDTH, HEIGHT, "Fgeditor", fgengine::gfx::Window::RESIZE);
    auto& keyboard_ref = window.getKeyboard();
    auto& mouse_ref = window.getMouse();
    auto clock = fgengine::utils::Time();

    glm::vec3 lightPos = glm::vec3(0.0f, 4.0f, 0.0f);

    window.display();

    return 0;
}