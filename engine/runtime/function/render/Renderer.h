#include "View.h"

class Renderer {
   private:
    /* data */
   public:
    Renderer(/* args */) {};
    ~Renderer() {};

    void prerender();

    void render(View* view);
};
