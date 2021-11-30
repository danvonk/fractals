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
        /**
         * Returns an assoc. array of attributes and their binding
         * positions.
         * @return
         */
        auto get_attr_locs() -> std::vector<std::pair<std::string, GLint>>;

        static auto get_curr_prog() -> BoundShaderProgram*;

        /**
         * Statically create a new shader program, given a vector of shader file locations (relative)
         * @param paths vector of relative file paths
         * @return constructed ShaderProgram
         */
        static auto create(const std::vector<std::string>& paths) -> std::shared_ptr<ShaderProgram>;

    private:
        GLuint id_;
        std::vector<std::shared_ptr<Shader>> shaders_;
    };

    class BoundShaderProgram {
    public:
        explicit BoundShaderProgram(ShaderProgram* prog);

        auto set_uniform(const std::string& s, float v) -> void;
        auto set_uniform(const std::string& s, int v) -> void;
    private:
        ShaderProgram* shp_;
    };
}


#endif // FRACTALS_SHADER_PROG_H
