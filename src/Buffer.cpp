//
// Created by Chaeyoung Lim on 2022/02/10.
//

#include "Buffer.h"

BufferUPtr Buffer::CreateWithData(
        uint32_t bufferType, uint32_t usage,
        const void *data, size_t stride, size_t count) {
    auto buffer = BufferUPtr(new Buffer());
    if (!buffer->Init(bufferType, usage, data, stride, count))
        return nullptr;
    return std::move(buffer);
}

Buffer::~Buffer() {
    if (m_buffer) {
        glDeleteBuffers(1, &m_buffer);
    }
}

void Buffer::Update(const void* data) const {
    Bind();
    glBufferData(m_bufferType, m_stride * m_count, data, m_usage);
};

void Buffer::Bind() const {
    glBindBuffer(m_bufferType, m_buffer);
}

bool Buffer::Init(
        uint32_t bufferType, uint32_t usage,
        const void* data, size_t stride, size_t count) {
    m_bufferType = bufferType;
    m_usage = usage;
    m_stride = stride;
    m_count = count;
    glGenBuffers(1, &m_buffer);
    Bind();
    glBufferData(m_bufferType, m_stride * m_count, data, m_usage);
    return true;
}