#include <iostream>
#include <memory>

// 这里如何换成非相对路径的形式？？？
#include "../../runtime/engine.h"
namespace zeus{
    class editor{
        private:
            std::shared_ptr<engine> m_zeus_engine;
        public:
            editor();
            virtual ~editor();//虚析构
            void run();
            

        protected:
    };
}//namespace zeus'