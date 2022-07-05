//
// Created by Chaeyoung Lim on 2022/06/14.
//

#ifndef CHA_ENGINE_CLOTH_H
#define CHA_ENGINE_CLOTH_H

#include "Mesh.h"


CLASS_PTR(Cloth);
class Cloth {
public:
    static ClothUPtr Create(
            uint32_t resX = 51, uint32_t resY = 51, float sizeX = 0.2, float sizeY = 0.2, float k_stiff = 1,
            bool hasPosConstr = false, glm::vec3 initPos = glm::vec3(-5.0f, 5.0f, -5.0f));
    ~Cloth() = default;
    void Update(float deltaTime, float dampingRate, bool hasPosConstr,
                int solverIter, glm::vec3 sphereCenter, float sphereRadius);

    void Draw(const Program* program) const;

private:
    Cloth(int resX, int resY, float sizeX, float sizeY, float k_stiff, bool hasPosConstr, glm::vec3 initPos)
            : m_resX(resX), m_resY(resY), m_sizeX(sizeX), m_sizeY(sizeY), m_k_stiff(k_stiff),
              m_hasPosConstr(hasPosConstr), m_initPos(initPos) {};

    void Init();  // initialize the m_restLength
    [[nodiscard]] bool IsInside(int x, int y) const { return x >= 0 && y >= 0 && x < m_resX && y < m_resX; } // check whether the current checking point is inside the grid
    [[nodiscard]] int FindGridIndex(int p0, int p1) const { return p1 * m_resX + p0; }  // change from vec2i of constraint to grid point index
    void InitIndexArray(std::vector<uint32_t> &indices);
    void InitVertexArray();
    void SetPositionConstraint(); // only used in single cloth mode to check updating

    uint32_t m_primitiveType{GL_TRIANGLES};
    VertexLayoutUPtr m_vertexLayout;
    BufferSPtr m_vertexBuffer;
    BufferSPtr m_indexBuffer;
    MaterialSPtr m_material;


    uint32_t m_resX, m_resY;  // # of m_points on each width and height
    float m_sizeX, m_sizeY;  // size of the length between each two m_points (could be used to initialize the m_restLength)
    float m_k_stiff;  // stiffness of the distance constraint
    bool m_hasPosConstr;
    glm::vec3 m_initPos;  // init pos in the world coordinate
    std::vector<Point> m_points; // the m_points that constructs the piece of cloth
    std::vector<Vertex> m_vertices; // the m_points that constructs the piece of cloth
    std::vector<glm::vec2> m_distConstraintList;  // containing the distance constrains between the edges
    std::vector<float> m_restLength; // the rest lengths between each two m_points of the cloth
    std::vector<glm::vec3> m_posConstraintList;  // stores the position of position contraints
};

#endif //CHA_ENGINE_CLOTH_H
