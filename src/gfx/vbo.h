#ifndef FRACTALS_VBO_H
#define FRACTALS_VBO_H

#include "common.h"


namespace fr {
    class BoundVertexBuffer;
    struct VertexAttribute {
        /**
         * Construct a new vertex attribute
         * @param n name of attribute
         * @param t OpenGL type
         * @param comps no. of components (e.g. 3 for Vec3)
         * @param stride size in bytes of the attribute
         */
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

        /**
         * Static constructor for VBO
         * @param attrs list of attrbitues
         * @return constructed VBO
         */
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

        template <typename T, typename A>
        auto set_data(const std::vector<T, A>& data) -> void {
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), (void*)data.data(), GL_STATIC_DRAW);
            buf_->no_elements_ = data.size() * sizeof(T) / buf_->stride_;
        }

        auto set_data(size_t bytes_sz, const void* data, GLenum usage) -> void;

    private:
        VertexBuffer* buf_;
    };

}


#endif // FRACTALS_VBO_H
