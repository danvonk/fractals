#include <chrono>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "common.h"
#include "window.h"

#include "util/imgui.h"
#include "util/backends/imgui_impl_sdl.h"
#include "util/backends/imgui_impl_opengl3.h"

using namespace fr::window;

auto init_logging() -> void {
    // create color multi threaded logger
    auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");
}

auto init_imgui(SDL_Window* w, SDL_GLContext* ctx) -> void {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(w, ctx);
    ImGui_ImplOpenGL3_Init("#version 430");
}

auto cleanup_imgui() -> void {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    init_logging();

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
        spdlog::error("Could not create OpenGL Context");
        const char* err_str = SDL_GetError();
        std::cerr << err_str;
        SDL_Quit();
        return 1;
    } else {
        std::string vendor((const char*)glGetString(GL_VENDOR));
        std::string version((const char*)glGetString(GL_VERSION));
        std::string glsl((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        spdlog::info("OpenGL vendor: {}", vendor);
        spdlog::info("OpenGL version: {}", version);
        spdlog::info("OpenGL GLSL version: {}", glsl);
    }

    glewExperimental = GL_TRUE;
    auto glew_init = glewInit();
    if (glew_init != GLEW_OK) {
        spdlog::error("Could not initialise GLEW");
        return 1;
    }

    bool running = true;
    WindowSettings ws = get_window_settings("../settings.cfg").expect("Could not open config file");
    Window app(*window, ws);

    init_imgui(window, &ctx);
    app.init();

    auto prev_time = std::chrono::system_clock::now();
    auto curr_time = std::chrono::system_clock::now();

    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_KEYDOWN) {
                switch (ev.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    default:
                        app.on_keydown(ev.key);
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

        prev_time = curr_time;
        curr_time = std::chrono::system_clock::now();

        app.update(std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - prev_time).count());
        app.gui();

        SDL_GL_SwapWindow(window);
    }

    cleanup_imgui();
    SDL_GL_DeleteContext(ctx);
    SDL_Quit();
    return 0;
}
