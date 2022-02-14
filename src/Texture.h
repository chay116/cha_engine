//
// Created by Chaeyoung Lim on 2022/02/12.
//

#ifndef CHA_ENGINE_TEXTURE_H
#define CHA_ENGINE_TEXTURE_H

#include "Image.h"

CLASS_PTR(Texture)
class Texture {
public:
    static TextureUPtr CreateFromImage(const Image* image);
    ~Texture();

    const uint32_t Get() const { return m_texture; }
    void Bind() const;
    void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void SetWrap(uint32_t sWrap, uint32_t tWrap) const;

private:
    Texture() {}
    void CreateTexture();
    void SetTextureFromImage(const Image* image);

    uint32_t m_texture { 0 };
};



#endif //CHA_ENGINE_TEXTURE_H
