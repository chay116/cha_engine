//
// Created by Chaeyoung Lim on 2022/02/22.
//

#ifndef CHA_ENGINE_SHADOW_MAP_H
#define CHA_ENGINE_SHADOW_MAP_H

#include "Texture.h"

CLASS_PTR(ShadowMap);
class ShadowMap {
public:
    static ShadowMapUPtr Create(int width, int height);
    ~ShadowMap();

    const uint32_t Get() const { return m_framebuffer; }
    void Bind() const;
    const TextureSPtr GetShadowMap() const { return m_shadowMap; }

private:
    ShadowMap() {}
    bool Init(int width, int height);

    uint32_t m_framebuffer { 0 };
    TextureSPtr m_shadowMap;
};



#endif //CHA_ENGINE_SHADOW_MAP_H
