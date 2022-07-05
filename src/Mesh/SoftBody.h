////
//// Created by Chaeyoung Lim on 2022/06/22.
////
//
//#ifndef CHA_ENGINE_SOFTBODY_H
//#define CHA_ENGINE_SOFTBODY_H
//
//#include "Mesh.h"
//
//
//CLASS_PTR(SoftBody);
//class SoftBody {
//public:
//    static SoftBodyUPtr Create(
//            int latiSegmentCount = 18, int longiSegmentCount = 18, float m_radius = 1.0f,  float k_stiff = 1, glm::vec3 initPos = glm::vec3(-5.0f, 5.0f, -5.0f));
//    ~SoftBody() = default;
//    void Update(float deltaTime, float dampingRate, int solverIter, glm::vec3 sphereCenter, float sphereRadius);
//
//    void Draw(const Program* program) const;
//
//private:
//    SoftBody(int latiSegmentCount, int longiSegmentCount, float m_radius, float k_stiff, glm::vec3 initPos)
//            : m_latiSegmentCount(latiSegmentCount), m_longiSegmentCount(longiSegmentCount), m_radius(m_radius), m_k_stiff(k_stiff),
//              m_initPos(initPos) {};
//
//    void Init();  // initialize the m_restLength
//    void InitIndexArray(std::vector<uint32_t> &indices);
//    void InitVertexArray();
//
//    uint32_t m_primitiveType{GL_TRIANGLES};
//    VertexLayoutUPtr m_vertexLayout;
//    BufferSPtr m_vertexBuffer;
//    BufferSPtr m_indexBuffer;
//    MaterialSPtr m_material;
//
//    uint32_t m_latiSegmentCount, m_longiSegmentCount;  // # of m_points on each width and height
//    float m_radius;  // size of the length between each two m_points (could be used to initialize the m_restLength)
//    float m_k_stiff;  // stiffness of the distance constraint
//    glm::vec3 m_initPos;  // init pos in the world coordinate
//    std::vector<Point> m_points; // the m_points that constructs the piece of SoftBody
//    std::vector<Vertex> m_vertices; // the m_points that constructs the piece of SoftBody
//    std::vector<glm::vec2> m_distConstraintList;  // containing the distance constrains between the edges
//    std::vector<float> m_restLength; // the rest lengths between each two m_points of the SoftBody
//};
//
//
//#endif //CHA_ENGINE_SOFTBODY_H
