#ifndef FRACTALS_SHADER_H
#define FRACTALS_SHADER_H

#include "common.h"

namespace fr {
    class Shader {
    public:
        explicit Shader(GLenum type) noexcept;

        static auto create(GLenum type, std::string src) -> std::shared_ptr<Shader>;

        auto compile() -> void;

        auto set_source(std::string src) -> void;
        auto se_source(std::string_view src) -> void;
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
