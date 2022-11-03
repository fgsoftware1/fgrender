#include "include/time.hpp"
#include "GLFW/glfw3.h"

namespace fgengine
{
    namespace utils
    {
        Time::Time() :
            m_time(0.0), m_last(0.0)
        {}

        Time::~Time()
        {}

        void Time::tick()
        {
            m_last = m_time;
            m_time = glfwGetTime();
        }

        double Time::timeSinceLastTick() const
        {
            return m_time - m_last;
        }

        double Time::time() const
        {
            return m_time;
        }   
    }
}