#include "vertexarray.h"

#include "common.h"

using fr::VertexArray;
using fr::BoundVertexArray;

VertexArray::VertexArray(GLenum primitive_mode) noexcept
    : id_(0)
    , primitive_mode_(primitive_mode)
{
    glGenVertexArrays(1, &id_);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id_);
}

auto VertexArray::create(GLenum primitive_mode) -> std::shared_ptr<VertexArray> {
    return std::make_shared<VertexArray>(primitive_mode);
}

auto VertexArray::bind() -> BoundVertexArray {
    return BoundVertexArray(this);
}

auto VertexArray::attach(std::shared_ptr<VertexBuffer> vbo) -> void {
    vbos_.push_back(vbo);
    vbo->bind();
    bind();

    const auto& attrs = vbo->get_attrs();
    glBindVertexBuffer(0, vbo->getID(), 0, vbo->get_stride());
    size_t offset = 0;
    for (auto i = 0u; i < attrs.size(); i++) {
        const auto& a = attrs[i];
        glEnableVertexAttribArray(i);
        glVertexAttribFormat(i, a.components, a.type, GL_FALSE, offset);
        offset += a.size;
        glVertexAttribBinding(i, 0);
    }
}

auto VertexArray::attach_attr(fr::VertexAttribute &v) -> void {
    attrs_.push_back(v);
}

BoundVertexArray::BoundVertexArray(VertexArray *v)
    : vao_id_(v->getID())
    , vao_(v)
{
    glBindVertexArray(v->getID());
}

auto BoundVertexArray::draw() -> void {
    for (auto v : vao_->vbos_) {
        v->bind();
        glDrawArrays(vao_->primitive_mode_, 0, v->get_no_elements());
    }
}

auto BoundVertexArray::draw(int no_elements) -> void {
    for (auto v : vao_->vbos_) {
        v->bind();
        glDrawArrays(vao_->primitive_mode_, 0, no_elements);
    }
}
