//
// Created by Chaeyoung Lim on 2022/02/12.
//

#ifndef CHA_ENGINE_TEXTURE_H
#define CHA_ENGINE_TEXTURE_H

#include "Image.h"

CLASS_PTR(Texture)

class Texture {
public:
    static TextureUPtr Create(int width, int height,
                              uint32_t format, uint32_t type = GL_UNSIGNED_BYTE);

    static TextureUPtr CreateFromImage(const Image *image);

    ~Texture();

    void Bind() const;
    void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void SetWrap(uint32_t sWrap, uint32_t tWrap) const;

    [[nodiscard]] uint32_t Get() const { return m_texture; }
    [[nodiscard]] int GetWidth() const { return m_width; }
    [[nodiscard]] int GetHeight() const { return m_height; }
    [[nodiscard]] uint32_t GetFormat() const { return m_format; }
    [[nodiscard]] uint32_t GetType() const { return m_type; }

    void SetBorderColor(const glm::vec4& color) const;
private:
    Texture() = default;

    void CreateTexture();

    void SetTextureFromImage(const Image *image);

    void SetTextureFormat(int width, int height, uint32_t format, uint32_t type);

    uint32_t m_texture{0};
    int m_width{0};
    int m_height{0};
    uint32_t m_format{GL_RGBA};
    uint32_t m_type{GL_UNSIGNED_BYTE};
};

CLASS_PTR(CubeTexture)

class CubeTexture {
public:
    static CubeTextureUPtr CreateFromImages(
            const std::vector<Image *> &images);
    static CubeTextureUPtr Create(int width, int height,
                                  uint32_t format, uint32_t type = GL_UNSIGNED_BYTE);
    ~CubeTexture();

    [[nodiscard]] const uint32_t Get() const { return m_texture; }
    void Bind() const;

    [[nodiscard]] int GetWidth() const { return m_width; }
    [[nodiscard]] int GetHeight() const { return m_height; }
    [[nodiscard]] uint32_t GetFormat() const { return m_format; }
    [[nodiscard]] uint32_t GetType() const { return m_type; }

    void GenerateMipmap() const;

private:
    CubeTexture() = default;
    bool InitFromImages(const std::vector<Image *> images);
    void Init(int width, int height, int format, uint32_t type);

    uint32_t m_texture{0};
    int m_width { 0 };
    int m_height { 0 };
    uint32_t m_format { GL_RGBA };
    uint32_t m_type { GL_UNSIGNED_BYTE };
};

#endif //CHA_ENGINE_TEXTURE_H
