#ifndef FRACTAL_VERTEXARRAY_H
#define FRACTAL_VERTEXARRAY_H

//#include "SDL_opengl.h"
#include "common.h"
#include "vbo.h"

namespace fr {
    class BoundVertexArray;
    class VertexArray {
        friend class BoundVertexArray;
    public:
        explicit VertexArray(GLenum primitive_mode = GL_TRIANGLES) noexcept;
        ~VertexArray();

        auto bind() -> BoundVertexArray;

        auto attach(std::shared_ptr<VertexBuffer> vbo) -> void;

        auto attach_attr(VertexAttribute& v) -> void;

        auto getID() const -> GLuint {
            return id_;
        }

        static auto create(GLenum primitive_mode = GL_TRIANGLES) -> std::shared_ptr<VertexArray>;

    private:
        GLuint id_;
        GLenum primitive_mode_;

        std::vector<VertexAttribute> attrs_;
        std::vector<std::shared_ptr<VertexBuffer>> vbos_;
    };

    class BoundVertexArray {
        friend class VertexArray;
    public:
        explicit BoundVertexArray(VertexArray* v);

        auto draw() -> void;

    private:
        GLuint vao_id_;
        VertexArray* vao_;
    };
}


#endif // FRACTAL_VERTEXARRAY_H
