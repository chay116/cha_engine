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
            const void* data, size_t dataSize);
    ~Buffer();
    uint32_t Get() const { return m_buffer; }
    void Bind() const;

private:
    Buffer() {}
    bool Init(
            uint32_t bufferType, uint32_t usage,
            const void* data, size_t dataSize);
    uint32_t m_buffer { 0 };
    uint32_t m_bufferType { 0 };
    uint32_t m_usage { 0 };
};


#endif //CHA_ENGINE_BUFFER_H
