#ifndef colors_HPP
#define colors_HPP

#include <cinttypes>

namespace fgengine{
    namespace gfx{
        struct Color
        {
            uint8_t r = 0, g = 0, b = 0, a = 1;

            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255) :
                r(r), g(g), b(b), a(a)
            {}

            Color(const Color& other) :
                r(other.r), g(other.g), b(other.b), a(other.a)
            {}
        
            static const Color Black;
            static const Color White;
            static const Color Red;
            static const Color Green;
            static const Color Blue;
            static const Color Yellow;
            static const Color Magenta;
            static const Color Cyan;
            static const Color Transparent;
        };
    }
}

#endif
