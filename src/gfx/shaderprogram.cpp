#include "shaderprogram.h"

#include <filesystem>
#include <fstream>

using namespace fr;
namespace fs = std::filesystem;

static BoundShaderProgram* currently_bound = nullptr;

ShaderProgram::ShaderProgram() noexcept {
    id_ = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id_);
}

//auto ShaderProgram::create(std::string_view name) -> std::shared_ptr<ShaderProgram> {
//    auto shp = std::make_shared<ShaderProgram>();
//
//    // look for shaders
//
//    shp->link();
//    return shp;
//}

auto ShaderProgram::link() -> void {
    glLinkProgram(id_);
    // check error log
    GLint logLength = 0;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1)
    {
        std::vector<GLchar> log;
        log.resize(unsigned(logLength) + 1);
        glGetProgramInfoLog(id_, logLength + 1, nullptr, log.data());

        spdlog::error("Shader linker returned:");
        spdlog::error(log.data());
    } else {
        spdlog::info("Linked shader.");
    }
}

auto ShaderProgram::use() -> BoundShaderProgram {
    glUseProgram(id_);
    return BoundShaderProgram(this);
}

auto ShaderProgram::create(const std::vector<std::string> &paths) -> std::shared_ptr<ShaderProgram> {
    auto shp = std::make_shared<ShaderProgram>();

    for (auto& path : paths) {
        fs::path fs_path(path);
        if (fs_path.has_extension()) {
            if (fs_path.extension() == ".fsh") {
                std::ifstream ifs(path);
                std::string str(std::istreambuf_iterator<char>{ifs}, {});
                auto fsh = Shader::create(GL_FRAGMENT_SHADER, str);
                fsh->set_filename(fs_path.stem());
                shp->attach_shader(fsh);
            } else if (fs_path.extension() == ".vsh") {
                std::ifstream ifs(path);
                std::string str(std::istreambuf_iterator<char>{ifs}, {});
                auto vsh = Shader::create(GL_VERTEX_SHADER, str);
                vsh->set_filename(fs_path.stem());
                shp->attach_shader(vsh);
            }
    }
    }
    shp->link();
    return shp;
}

auto ShaderProgram::get_attr_locs() -> std::vector<std::pair<std::string, GLint>> {
    std::vector<std::pair<std::string, int>> locs;

    // get attribute locations
    GLint maxAttrLength = 0;
    GLint attrCnt = 0;
    glGetProgramiv(id_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttrLength);
    glGetProgramiv(id_, GL_ACTIVE_ATTRIBUTES, &attrCnt);

    std::vector<char> nameBuffer;
    nameBuffer.resize(unsigned(maxAttrLength));
    for (auto i = 0; i < attrCnt; ++i)
    {
        GLint size;
        GLenum type;
        glGetActiveAttrib(id_, GLuint(i), GLsizei(nameBuffer.size()), nullptr, &size, &type, nameBuffer.data());

        std::string name{nameBuffer.data()};
        if (name.find("gl_") == 0)
            continue;

        auto shaderLoc = glGetAttribLocation(id_, name.c_str());
        locs.emplace_back(name, shaderLoc);
    }

    return locs;
}

auto ShaderProgram::attach_shader(std::shared_ptr<Shader> sh) -> void {
    shaders_.push_back(sh);
    glAttachShader(id_, sh->getID());
}

auto ShaderProgram::get_curr_prog() -> BoundShaderProgram * {
    return currently_bound;
}

//auto ShaderProgram::set_uniform(std::string_view name) -> void {
//
//}

BoundShaderProgram::BoundShaderProgram(ShaderProgram* prog)
    : shp_(prog)
{
   currently_bound = this;
}

auto BoundShaderProgram::set_uniform(const std::string& s, float v) -> void {
    auto loc = glGetUniformLocation(shp_->id_, s.c_str());
    glUniform1f(loc, v);
}
