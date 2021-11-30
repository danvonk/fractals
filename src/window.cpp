#include "window.h"
#include "maths/vec3.h"
#include "gfx/vbo.h"
#include "gfx/vertexarray.h"

#include "stx/internal/option_result.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

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

GLfloat verts[] = {
        0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f
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
//    vbo->set_attribute({"col", GL_FLOAT, 12});
//    vbo_->bind().set_data(sizeof(verts), verts, GL_STATIC_DRAW);
    vbo_->bind().set_data(vertices);
//    vbo->bind().set_data(vertices);

    va_->attach(vbo_);

//    auto binds = shader_->get_attr_locs();
//
//    std::cout << "Bindings are:\n";
//    for (auto& [k,v] : binds) {
//        std::cout << k << " is " << v << '\n';
//    }
}

auto Window::update(float seconds) -> void {
    glClearColor(0.1f, 0.2f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    seconds *= 3;
    auto prog = shader_->use();
    prog.set_uniform("width", width_);
    va_->bind().draw(6);
}

auto Window::on_resize(int w, int h) -> void {
    glViewport(0,0, w, h);
    width_ = w;
    height_ = h;
    spdlog::info("Changed (w,h) to ({}, {})", w, h);
}
