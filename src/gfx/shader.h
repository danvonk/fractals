#ifndef FRACTALS_SHADER_H
#define FRACTALS_SHADER_H

#include "common.h"

namespace fr {
    class Shader {
    public:
        explicit Shader(GLenum type) noexcept;

        /**
         * Create a new shader
         * @param type GL_FRAGMENT etc.
         * @param src string containing entire source file of shader
         * @return constructed shader
         */
        static auto create(GLenum type, std::string src) -> std::shared_ptr<Shader>;

        /**
         * Compile the shader. Check log for errors
         */
        auto compile() -> void;

        /**
         * Set the source code and recompile
         * @param src string containing entire source file of shader
         */
        auto set_source(std::string src) -> void;
        /**
         * Set the filename. Useful for debugging
         * @param fname name of shader for debugging
         */
        auto set_filename(std::string fname) -> void;

        [[nodiscard]]
        auto getID() const -> GLuint {
            return id_;
        }

        [[nodiscard]]
        auto get_type() const -> GLenum {
            return type_;
        }

        auto is_compiled() const -> bool {
            return compiled_;
        }
    private:
        GLuint id_;
        GLenum type_;
        bool compiled_;

        std::string filename_;
        std::vector<std::string> src_;
    };
}


#endif // FRACTALS_SHADER_H
