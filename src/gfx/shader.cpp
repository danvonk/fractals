#include "shader.h"

#include <filesystem>
#include <utility>

using namespace fr;

Shader::Shader(GLenum type) noexcept
: id_(0)
, type_(type)
, compiled_(false)
{
    id_ = glCreateShader(type);
}

auto Shader::create(GLenum type, std::string src) -> std::shared_ptr<Shader> {
    auto sh = std::make_shared<Shader>(type);
    sh->set_source(std::move(src));
    sh->compile();
    return sh;
}

auto Shader::compile() -> void {
    glCompileShader(id_);
    GLint logLength = 0;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        std::string logOutput;
        {
            std::vector<GLchar> log(static_cast<unsigned>(logLength) + 1u);
            glGetShaderInfoLog(id_, logLength + 1, nullptr, log.data());
            logOutput = std::string(log.data());
        }

        spdlog::error("Shader was not compiled:");
        spdlog::error(logOutput);
        compiled_ = false;
    } else {
        compiled_ = true;
    }
}

auto Shader::set_filename(std::string fname) -> void {
    filename_ = std::move(fname);
}

auto Shader::set_source(std::string src) -> void {
    src_.push_back(src);
    const char* d = src.c_str();
    glShaderSource(id_, 1, &d, nullptr);
}
