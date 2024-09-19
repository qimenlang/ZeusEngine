#include <chrono>

#include "core/base/singleton.h"
namespace zeus{
    class engine : public singleton<engine>
    {
        //父类为什么要作为友元？？？
        friend class singleton<engine>;
        protected:
            engine();
            void LogicTick(float delta_time);
            void RenderTick();
        public:
            virtual ~engine();
            engine(const engine&) = delete;
            engine& operator=(const engine&) = delete;
            void init();
            void run();
    private:
        unsigned int VBO, VAO, EBO;
        unsigned int diffuse;
        unsigned int specular;
        std::chrono::steady_clock::time_point time_last_frame = std::chrono::steady_clock::now();
    };
    
}