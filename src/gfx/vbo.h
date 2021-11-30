#ifndef FRACTALS_VBO_H
#define FRACTALS_VBO_H

#include "common.h"


namespace fr {
    class BoundVertexBuffer;
    struct VertexAttribute {
        VertexAttribute(std::string n, GLenum t, int comps, size_t stride)
            : name(std::move(n))
            , type(t)
            , size(stride)
            , components(comps)
        {
        }

        std::string name;
        GLenum type;
        size_t size;
        int components;
    };

    class VertexBuffer {
        friend class BoundVertexBuffer;
    public:
        VertexBuffer() noexcept;

        auto bind() -> BoundVertexBuffer;

        static auto create() -> std::shared_ptr<VertexBuffer> {
           return std::make_shared<VertexBuffer>();
        }

        static auto create(const std::vector<VertexAttribute>& attrs) -> std::shared_ptr<VertexBuffer>;

       [[nodiscard]]
       auto getID() const -> GLuint {
          return id_;
       }

       [[nodiscard]]
       auto get_stride() const {
           return stride_;
       }

       [[nodiscard]]
       auto get_no_elements() const {
           return no_elements_;
       }

       [[nodiscard]]
       auto get_attrs() const -> const std::vector<VertexAttribute>& {
          return attributes_;
       }

       auto set_attribute(const VertexAttribute& attr) -> void;

    private:
        GLuint id_;
        std::vector<VertexAttribute> attributes_;
        size_t stride_;
        size_t no_elements_;
    };

    class BoundVertexBuffer {
        friend class VertexBuffer;
    public:
        explicit BoundVertexBuffer(VertexBuffer* buf);

        ~BoundVertexBuffer() {
//            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        template <typename T>
        auto set_data(const std::vector<T>& data) -> void;

        auto set_data(size_t bytes_sz, const void* data, GLenum usage) -> void;

    private:
        VertexBuffer* buf_;
    };

}


#endif // FRACTALS_VBO_H
