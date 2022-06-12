//
// Created by Chaeyoung Lim on 2022/02/21.
//

#ifndef CHA_ENGINE_FRAMEBUFFER_H
#define CHA_ENGINE_FRAMEBUFFER_H

#include "Texture.h"

CLASS_PTR(Framebuffer);
class Framebuffer {
public:
    static FramebufferUPtr Create(const std::vector<TextureSPtr>& colorAttachments);
    static void BindToDefault();
    ~Framebuffer();

    [[nodiscard]] uint32_t Get() const { return m_framebuffer; }
    void Bind() const;
    [[nodiscard]] int GetColorAttachmentCount() const { return (int)m_colorAttachments.size(); }
    [[nodiscard]] TextureSPtr GetColorAttachment(int index = 0) const { return m_colorAttachments[index]; }

private:
    Framebuffer() = default;
    bool InitWithColorAttachments(const std::vector<TextureSPtr>& colorAttachments);

    uint32_t m_framebuffer { 0 };
    uint32_t m_depthStencilBuffer { 0 };
    std::vector<TextureSPtr> m_colorAttachments;
};

CLASS_PTR(CubeFramebuffer);
class CubeFramebuffer {
public:
    static CubeFramebufferUPtr Create(CubeTextureSPtr colorAttachment, uint32_t mipLevel = 0);
    ~CubeFramebuffer();

    [[nodiscard]] uint32_t Get() const { return m_framebuffer; }
    void Bind(int cubeIndex = 0) const;
    [[nodiscard]] CubeTextureSPtr GetColorAttachment() const { return m_colorAttachment; }

private:
    CubeFramebuffer() = default;
    bool InitWithColorAttachment(const CubeTextureSPtr& colorAttachment, uint32_t mipLevel);

    uint32_t m_framebuffer { 0 };
    uint32_t m_depthStencilBuffer { 0 };
    CubeTextureSPtr m_colorAttachment;
    uint32_t m_mipLevel { 0 };
};

#endif //CHA_ENGINE_FRAMEBUFFER_H
