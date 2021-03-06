//
// Created by Chaeyoung Lim on 2022/02/10.
//

#ifndef CHA_ENGINE_CONTEXT_H
#define CHA_ENGINE_CONTEXT_H

#include "common.h"
#include "Shader.h"
#include "Program.h"
#include "Buffer.h"
#include "Texture.h"
#include "Vertex_layout.h"
#include "Mesh/Cloth.h"
#include "Model.h"
#include "Framebuffer.h"
#include "Shadow_map.h"

CLASS_PTR(Context)


class Context {
public:
    static ContextUPtr Create();
    void Render();
    void ProcessInput(GLFWwindow *window);
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);
    void DrawSphere(const glm::mat4& view,
                    const glm::mat4& projection,
                    const Program* program);

private:
    Context() = default;
    bool Init();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    ProgramUPtr m_pbrProgram;
    ProgramUPtr m_simplePbrProgram;

    MeshUPtr m_box;
    MeshUPtr m_plane;
    MeshUPtr m_sphere;
    ClothUPtr m_cloth;
    struct Light {
        glm::vec3 position { glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 color { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    std::vector<Light> m_lights;


    struct Material {
        glm::vec3 albedo { glm::vec3(1.0f, 1.0f, 1.0f) };
        float roughness { 0.5f };
        float metallic { 0.5f };
        float ao { 0.1f };
        bool useIBL { false };
    };
    Material m_sphereMaterial;
    Material m_clothMaterial;


    TextureUPtr m_hdrMap;
    ProgramUPtr m_sphericalMapProgram;
    CubeTextureSPtr m_hdrCubeMap;
    ProgramUPtr m_skyboxProgram;
    CubeTextureSPtr m_diffuseIrradianceMap;
    ProgramUPtr m_diffuseIrradianceProgram;
    CubeTextureSPtr m_preFilteredMap;
    ProgramUPtr m_preFilteredProgram;
    TextureSPtr m_brdfLookupMap;
    ProgramUPtr m_brdfLookupProgram;

    // screen size
    int m_width {640};
    int m_height {480};

    // camera parameter
    bool m_cameraControl { false };
    glm::vec2 m_prevMousePos { glm::vec2(0.0f) };
    float m_cameraPitch { 0.0f };
    float m_cameraYaw { 0.0f };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, -1.0f, 0.0f) };
    glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 8.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };

};

#endif //CHA_ENGINE_CONTEXT_H
