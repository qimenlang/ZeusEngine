#include "core/base/singleton.h"
namespace zeus{
    class engine : public singleton<engine>
    {
        //父类为什么要作为友元？？？
        friend class singleton<engine>;
        protected:
            engine();
        public:
            virtual ~engine();
            engine(const engine&) = delete;
            engine& operator=(const engine&) = delete;
            void run();
    };
    
}