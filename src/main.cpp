#include <GL/glew.h>

#include "common.h"
#include "window.h"

using namespace fr::window;

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    auto* window = SDL_CreateWindow("Fractals",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    640,
                                    480,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    auto ctx = SDL_GL_CreateContext(window);
    if (!ctx) {
        std::cerr << "Could not create OpenGL Context.\n";
        const char* err_str = SDL_GetError();
        std::cerr << err_str;
        SDL_Quit();
        return 1;
    }

    glewExperimental = GL_TRUE;
    auto glew_init = glewInit();
    if (glew_init != GLEW_OK) {
        std::cerr << "Could not initialise GLEW.\n";
        return 1;
    }

    bool running = true;
    WindowSettings ws = get_window_settings("../settings.cfg").expect("Could not open config file");
    Window app(*window, ws);

    app.init();

    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_KEYDOWN) {
                switch (ev.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    default:
                        break;
                }
            } else if (ev.type == SDL_QUIT) {
                running = false;
            } else if (ev.type == SDL_WINDOWEVENT) {
                if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
                    app.on_resize(ev.window.data1, ev.window.data2);
                }
            }
        }

        app.update(0.0f);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(ctx);
    SDL_Quit();
    return 0;
}
