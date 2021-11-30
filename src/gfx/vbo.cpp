#include "vbo.h"

using namespace fr;

auto VertexBuffer::bind() -> BoundVertexBuffer {
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    return BoundVertexBuffer(this);
}

VertexBuffer::VertexBuffer() noexcept
    : id_(0)
    , stride_(0)
    , no_elements_(0)
{
    glGenBuffers(1, &id_);
}

auto VertexBuffer::set_attribute(const VertexAttribute &attr) -> void {
    attributes_.push_back(attr); // copy
    stride_ += attr.size;
}

auto VertexBuffer::create(const std::vector<VertexAttribute> &attrs) -> std::shared_ptr<VertexBuffer> {
    auto vbo = std::make_shared<VertexBuffer>();
    for (const auto& a : attrs) {
       vbo->set_attribute(a);
    }
    return vbo;
}

BoundVertexBuffer::BoundVertexBuffer(VertexBuffer* buf)
   : buf_(buf)
{
    glBindBuffer(GL_ARRAY_BUFFER, buf->getID());
    std::cout << "Bind vbo.\n";
}

template<typename T>
auto BoundVertexBuffer::set_data(const std::vector<T> &data) -> void {
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), (void*)data[0], GL_STATIC_DRAW);
}

auto BoundVertexBuffer::set_data(size_t bytes_sz, const void* data, GLenum usage) -> void {
    glBufferData(GL_ARRAY_BUFFER, bytes_sz, data, usage);
    buf_->no_elements_ = bytes_sz / buf_->stride_;
}
