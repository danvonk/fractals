#ifndef FRACTALS_SHADER_PROG_H
#define FRACTALS_SHADER_PROG_H

#include "common.h"
#include "maths/vec3.h"
#include "shader.h"

namespace fr {
    class BoundShaderProgram;

    class ShaderProgram {
        friend class BoundShaderProgram;
    public:
        explicit ShaderProgram() noexcept;
        ~ShaderProgram();

        auto link() -> void;

        auto use() -> BoundShaderProgram;

        auto attach_shader(std::shared_ptr<Shader> sh) -> void;
        auto get_attr_locs() -> std::vector<std::pair<std::string, GLint>>;

        static auto get_curr_prog() -> BoundShaderProgram*;

        //static auto create(std::string_view name) -> std::shared_ptr<ShaderProgram>;
        static auto create(const std::vector<std::string>& paths) -> std::shared_ptr<ShaderProgram>;

    private:
        GLuint id_;
        std::vector<std::shared_ptr<Shader>> shaders_;
    };

    class BoundShaderProgram {
    public:
        explicit BoundShaderProgram(ShaderProgram* prog);

        auto set_uniform(const std::string& s, float v) -> void;
    private:
        ShaderProgram* shp_;
    };
}


#endif // FRACTALS_SHADER_PROG_H
