#include <iostream>

#include "editor.h"

namespace zeus{
    editor::editor(){
    }
    editor::~editor(){}
    void editor::run(){
        zeus::engine::instance().init();
        zeus::engine::instance().run();
    }
}
