#ifndef time_HPP
#define time_HPP

namespace fgengine{
    namespace utils{
        class Time
        {
        private:
            double m_time{};
            double m_last{};

        public:
            Time();
            ~Time();

            void tick();
            double timeSinceLastTick() const;
            double time() const;
        };
    }
}

#endif