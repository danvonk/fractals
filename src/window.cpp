#include "window.h"
#include "maths/vec3.h"
#include "gfx/vbo.h"
#include "gfx/vertexarray.h"

#include "stx/internal/option_result.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "util/imgui.h"
#include "util/backends/imgui_impl_opengl3.h"
#include "util/backends/imgui_impl_sdl.h"

using stx::Ok;
using stx::Err;
using std::string_view;
using std::string;
using fr::window::Window;
using fr::window::Error;
using fr::window::WindowSettings;
namespace pt = boost::property_tree;

template<typename T>
using Result = stx::Result<T, Error>;

struct Vertex {
    fr::Vec3f pos;
    fr::Vec3f colour;
};

auto fr::window::get_window_settings(const string &path) -> Result<WindowSettings> {
        pt::ptree props;
        pt::read_ini(path, props);

        auto& window_props = props.get_child("window");
        WindowSettings ws{};

        ws.fullscreen = window_props.get("fullscreen", false);
        ws.width = window_props.get("width", 640);
        ws.height = window_props.get("height", 480);

        return Ok(std::move(ws));
}

Window::Window(SDL_Window& wnd, const WindowSettings& ws) noexcept {
        wnd_ = &(wnd);
        width_ = ws.width;
        height_ = ws.height;
        fullscreen_ = ws.fullscreen;
        runtime_ = 0.0f;

        zoom_ = 1.0f;
}

auto Window::init() -> void {
    glViewport(0,0, width_, height_);
    va_ = VertexArray::create();

    std::vector<Vertex> vertices;
    vertices.push_back({fr::Vec3(-0.8f, -0.8f, 0.0f), fr::Vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back({fr::Vec3(+0.8f, -0.8f, 0.0f), fr::Vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back({fr::Vec3(+0.0f, +0.8f, 0.0f), fr::Vec3(1.0f, 1.0f, 1.0f)});

    shader_ = ShaderProgram::create({"../shaders/default.vsh", "../shaders/default.fsh"});
    vbo_ = VertexBuffer::create();
    vbo_->set_attribute({"pos", GL_FLOAT, 3, 12});
    vbo_->set_attribute({"col", GL_FLOAT, 3, 12});
    vbo_->bind().set_data(vertices);
    va_->attach(vbo_);
}

auto Window::update(float ms) -> void {
    runtime_ += ms;

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto prog = shader_->use();
    prog.set_uniform("width", width_);
    prog.set_uniform("height", height_);
    prog.set_uniform("zoom", zoom_);
    prog.set_uniform("offsetX", offsetX_);
    prog.set_uniform("offsetY", offsetY_);
    prog.set_uniform("max_iters", iterations_);
    va_->bind().draw(6);
}

auto Window::on_resize(int w, int h) -> void {
    glViewport(0,0, w, h);
    width_ = w;
    height_ = h;
    spdlog::info("Changed (w,h) to ({}, {})", w, h);
}

auto Window::on_keydown(const SDL_KeyboardEvent& key_ev) -> void {
    switch (key_ev.keysym.sym) {
        case SDLK_PLUS:
            zoom_ += 0.01f;
            break;
        case SDLK_MINUS:
            zoom_ = std::max(zoom_ - 0.01f, 0.000001f);
            break;
        case SDLK_LEFT:
            offsetX_ -= 0.01f;
            break;
        case SDLK_RIGHT:
            offsetX_ += 0.01f;
            break;
        case SDLK_UP:
            offsetY_ += 0.01f;
            break;
        case SDLK_DOWN:
            offsetY_ -= 0.01f;
            break;
        default:
            break;
    }
}

auto Window::gui() -> void {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Fractals");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::Text("Zoom is %fx", zoom_);
    ImGui::Text("Mandelbrot Iterations:");
    ImGui::InputInt("iterations", &iterations_, 10);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
