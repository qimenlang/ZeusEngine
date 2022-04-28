#include <iostream>

#include "editor.h"

namespace zeus{
    editor::editor(){
        m_zeus_engine = std::make_shared<engine>();
    }
    editor::~editor(){}
    void editor::run(){
        m_zeus_engine->run();
    }
}
