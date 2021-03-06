//
// Created by Chaeyoung Lim on 2022/02/10.
//

#ifndef CHA_ENGINE_BUFFER_H
#define CHA_ENGINE_BUFFER_H

#include "common.h"

CLASS_PTR(Buffer)

class Buffer {
public:
    static BufferUPtr CreateWithData(
            uint32_t bufferType, uint32_t usage,
            const void *data, size_t stride, size_t count);

    ~Buffer();

    [[nodiscard]] uint32_t Get() const { return m_buffer; }
    [[nodiscard]] size_t GetStride() const { return m_stride; }
    [[nodiscard]] size_t GetCount() const { return m_count; }
    void Bind() const;
    void Update(const void* data) const;

private:
    Buffer() {}

    bool Init(
            uint32_t bufferType, uint32_t usage,
            const void *data, size_t stride, size_t count);

    uint32_t m_buffer{0};
    uint32_t m_bufferType{0};
    uint32_t m_usage{0};
    size_t m_stride{0};
    size_t m_count{0};
};


#endif //CHA_ENGINE_BUFFER_H
