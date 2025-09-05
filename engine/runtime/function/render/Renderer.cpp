#include "Renderer.h"

#include "include/glad/glad.h"

void Renderer::prerender() {
    // todo : move to opengl backend
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::render(View* view) { view->update(); }