//
// Created by Chaeyoung Lim on 2022/02/18.
//

#ifndef CHA_ENGINE_MESH_H
#define CHA_ENGINE_MESH_H

#include "common.h"
#include "Buffer.h"
#include "Vertex_layout.h"
#include "Texture.h"
#include "Program.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

CLASS_PTR(Material);

class Material {
public:
    static MaterialUPtr Create() {
        return MaterialUPtr(new Material());
    }

    TextureSPtr diffuse;
    TextureSPtr specular;
    float shininess{32.0f};

    void SetToProgram(const Program* program) const;
private:
    Material() {}
};

CLASS_PTR(Mesh);

class Mesh {
public:
    static MeshUPtr Create(
            const std::vector<Vertex> &vertices,
            const std::vector<uint32_t> &indices,
            uint32_t primitiveType);

    void SetMaterial(MaterialSPtr material) { m_material = material; }

    MaterialSPtr GetMaterial() const { return m_material; }

    static MeshUPtr CreateBox();

    const VertexLayout *GetVertexLayout() const {
        return m_vertexLayout.get();
    }

    BufferSPtr GetVertexBuffer() const { return m_vertexBuffer; }

    BufferSPtr GetIndexBuffer() const { return m_indexBuffer; }

    void Draw(const Program* program) const;

private:
    Mesh() {}

    void Init(
            const std::vector<Vertex> &vertices,
            const std::vector<uint32_t> &indices,
            uint32_t primitiveType);

    uint32_t m_primitiveType{GL_TRIANGLES};
    VertexLayoutUPtr m_vertexLayout;
    BufferSPtr m_vertexBuffer;
    BufferSPtr m_indexBuffer;
    MaterialSPtr m_material;
};


#endif //CHA_ENGINE_MESH_H
