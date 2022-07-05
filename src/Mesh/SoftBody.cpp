////
//// Created by Chaeyoung Lim on 2022/06/22.
////
//
//#include "SoftBody.h"
//
//SoftBodyUPtr SoftBody::Create(int latiSegmentCount, int longiSegmentCount, float m_radius, float k_stiff, glm::vec3 initPos) {
//    auto softBody = SoftBodyUPtr(new SoftBody(latiSegmentCount, longiSegmentCount, m_radius, k_stiff, initPos));
//    softBody->Init();
//    return std::move(softBody);
//}
//
//void SoftBody::Init() {
//    std::vector<uint32_t> indices;
//    InitIndexArray(indices);
//    InitVertexArray();
//
//    m_vertexLayout = VertexLayout::Create();
//    m_vertexBuffer = Buffer::CreateWithData(
//            GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,
//            m_vertices.data(), sizeof(Vertex), m_vertices.size());
//    m_indexBuffer = Buffer::CreateWithData(
//            GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,
//            indices.data(), sizeof(uint32_t), indices.size());
//    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, false,
//                              sizeof(Vertex), 0);
//    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, false,
//                              sizeof(Vertex), offsetof(Vertex, normal));
//    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, false,
//                              sizeof(Vertex), offsetof(Vertex, texCoord));
//}
//
//void SoftBody::InitIndexArray(std::vector<uint32_t> &indices)
//{
//    uint32_t circleVertCount = m_longiSegmentCount + 1;
//    indices.resize(m_latiSegmentCount * m_longiSegmentCount * 6);
//    for (uint32_t i = 0; i < m_latiSegmentCount; i++) {
//        for (uint32_t j = 0; j < m_longiSegmentCount; j++) {
//            uint32_t vertexOffset = i * circleVertCount + j;
//            uint32_t indexOffset = (i * m_longiSegmentCount + j) * 6;
//            indices[indexOffset    ] = vertexOffset;
//            indices[indexOffset + 1] = vertexOffset + 1;
//            indices[indexOffset + 2] = vertexOffset + 1 + circleVertCount;
//            indices[indexOffset + 3] = vertexOffset;
//            indices[indexOffset + 4] = vertexOffset + 1 + circleVertCount;
//            indices[indexOffset + 5] = vertexOffset + circleVertCount;
//        }
//    }
//
//    // initialize constraint and m_restLength
//    for (uint32_t i = 0; i < m_latiSegmentCount; i++) {
//        float shearupperlatiiLength = mag(m_vertices[i * circleVertCount].position - m_vertices[(i - 1) * circleVertCount].position);
//        float shearunderlatiiLength = mag(m_vertices[i * circleVertCount].position - m_vertices[(i + 1) * circleVertCount].position);
//        float shearlongiLength = mag(m_vertices[i * circleVertCount].position - m_vertices[i * circleVertCount + 1].position);
//        float uppershear = sqrt(shearupperlatiiLength * shearupperlatiiLength + shearlongiLength * shearlongiLength);
//        float downshear = sqrt(shearupperlatiiLength * shearupperlatiiLength + shearlongiLength * shearlongiLength);
//        for (uint32_t j = 0; j < m_longiSegmentCount; j++) {
//            uint32_t vertexOffset = i * circleVertCount + j;
//            uint32_t indexOffset = (i * m_longiSegmentCount + j) * 6;
//            // UP
//            if (vertexOffset - circleVertCount > 0) {
//                // CENTER
//                m_distConstraintList.emplace_back(glm::vec2(vertexOffset, vertexOffset - circleVertCount));
//                m_restLength.push_back(shearupperlatiiLength);
//                // LEFT
//                m_distConstraintList.emplace_back(glm::vec2(vertexOffset, vertexOffset - circleVertCount + 1));
//                m_restLength.push_back(uppershear);
//                // RIGHT
//                m_distConstraintList.emplace_back(glm::vec2(vertexOffset, vertexOffset - circleVertCount - 1));
//                m_restLength.push_back(uppershear);
//            }
//            // DOWN
//            if (vertexOffset + circleVertCount < m_latiSegmentCount * (m_longiSegmentCount + 1)) {
//                // CENTER
//                m_distConstraintList.emplace_back(glm::vec2(vertexOffset, vertexOffset + circleVertCount));
//                m_restLength.push_back(shearRestLength);
//                // LEFT
//                m_distConstraintList.emplace_back(glm::vec2(vertexOffset, vertexOffset + circleVertCount + 1));
//                m_restLength.push_back(downshear);
//                // RIGHT
//                m_distConstraintList.emplace_back(glm::vec2(vertexOffset, vertexOffset + circleVertCount - 1));
//                m_restLength.push_back(downshear);
//            }
//            // LEFT
//            m_distConstraintList.emplace_back(glm::vec2(vertexOffset, vertexOffset + 1));
//            m_restLength.push_back(shearlongiLength);
//            // RIGHT
//            m_distConstraintList.emplace_back(glm::vec2(vertexOffset, vertexOffset - 1));
//            m_restLength.push_back(shearlongiLength);
//
//
//            indices[indexOffset    ] = vertexOffset;
//            indices[indexOffset + 1] = vertexOffset + 1;
//            indices[indexOffset + 2] = vertexOffset + 1 + circleVertCount;
//            indices[indexOffset + 3] = vertexOffset;
//            indices[indexOffset + 4] = vertexOffset + 1 + circleVertCount;
//            indices[indexOffset + 5] = vertexOffset + circleVertCount;
//        }
//    }
//}
//
//void SoftBody::InitVertexArray() {
//    // setting up initial m_points positions
//    uint32_t circleVertCount = m_longiSegmentCount + 1;
//    m_vertices.resize((m_latiSegmentCount + 1) * circleVertCount);
//    for (uint32_t i = 0; i <= m_latiSegmentCount; i++) {
//        float v = (float)i / (float)m_latiSegmentCount;
//        float phi = (v - 0.5f) * glm::pi<float>();
//        auto cosPhi = cosf(phi);
//        auto sinPhi = sinf(phi);
//        for (uint32_t j = 0; j <= m_longiSegmentCount; j++) {
//            float u = (float)j / (float)m_longiSegmentCount;
//            float theta = u * glm::pi<float>() * 2.0f;
//            auto cosTheta = cosf(theta);
//            auto sinTheta = sinf(theta);
//            auto newPos = glm::vec3(cosPhi * cosTheta, sinPhi, -cosPhi * sinTheta);
//
//            // initialize point constraint
//            Point newP;
//            newP.pos = newPos;
//            newP.vel = glm::vec3(0.0f, 0.0f, 0.0f);
//            newP.accel = glm::vec3(0.0f, 0.0f, 0.0f);
//            newP.invMass = 2.0f;
//
//            m_vertices[i * circleVertCount + j] = Vertex {
//                    newPos * m_radius, newPos, glm::vec2(u, v), glm::vec3(0.0f, 0.0f, 0.0f)
//            };
//            m_points.push_back(newP);
//        }
//    }
//}
//
//void SoftBody::Draw(const Program* program) const {
//    m_vertexBuffer->Update(m_vertices.data());
//    m_vertexLayout->Bind();
//    glDrawElements(m_primitiveType, m_indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
//}
//
//void SoftBody::Update(float deltaTime, float dampingRate, int solverIter, glm::vec3 sphereCenter, float sphereRadius)
//{
//    // external forces (gravity ONLY)
//    // --------------------------------
//    glm::vec3 gravity = glm::vec3(0, -9.8f, 0);
//    auto gravity_force = deltaTime * this->m_points[0].invMass * gravity;
//    // COARSE: damping velocities
//    auto damping_velocity = pow((1 - dampingRate), deltaTime);
//    for (int i = 0; i < this->m_points.size(); ++i)
//    {
//        if (!(i == m_latiSegmentCount * (m_longiSegmentCount - 1) || i == (m_latiSegmentCount * m_longiSegmentCount) - 1))  // do not conserve external forces
//        {
//            this->m_points[i].vel += gravity_force;
//            this->m_points[i].vel *= damping_velocity;
//        }
//        // add the predicted position with velocities
//        this->m_points[i].predPos = this->m_points[i].pos + deltaTime * this->m_points[i].vel;
//    }
//
//    // project constraints (ONLY distance contraints and position contraints for now)
//    // ---------------------------------
//    sphereRadius = sphereRadius * 1.03f;
//    for (int iter = 0; iter < solverIter; iter++)
//    {
//        // distance contraint
//        for (int i = 0; i < m_distConstraintList.size(); ++i)
//        {
//            glm::vec2 currDistConstr = m_distConstraintList[i]; // point-pair
//            Point pt1 = m_points[currDistConstr[0]];
//            Point pt2 = m_points[currDistConstr[1]];
//
//            glm::vec3 vecP2P1 = pt1.predPos - pt2.predPos;
//            float magP2P1 = mag(vecP2P1);
//            if (magP2P1 <= M_EPSION)
//                return;
//            float invMass = pt1.invMass + pt2.invMass;
//            if (invMass <= M_EPSION)
//                return;
//
//            glm::vec3 n_val = vecP2P1 / magP2P1;  // direction
//            float s_val = (magP2P1 - m_restLength[i]) * (1 / invMass);  // scaler
//
//            glm::vec3 distProj = s_val * n_val * m_k_stiff;
//            if (pt1.invMass > 0.0) // should always be true
//                m_points[currDistConstr[0]].predPos -= (distProj * pt1.invMass);
//            if (pt2.invMass > 0.0) // should always be true
//                m_points[currDistConstr[1]].predPos += (distProj * pt2.invMass);
//        }
//
//        // collision constraints
//        for (int i = 0; i < m_points.size(); ++i)
//        {
//            glm::vec3 p2c = m_points[i].predPos - sphereCenter; // distance between current predpos to the center of the sphere
//            float dist = mag(p2c);
//            // collision detection
//            if (dist - sphereRadius < M_EPSION) // collide with the sphere
//            {
//                float distToGo = sphereRadius - dist; // the distance to set the current predpos to the surface of the sphere
//                m_points[i].predPos += p2c * distToGo;  // direction * distance
//            }
//        }
//    }
//
//    // commit the velocity and the position changes
//    for (int i = 0; i < this->m_points.size(); ++i)
//    {
//        // commit velocity based on position changes
//        this->m_points[i].vel = (m_points[i].predPos - m_points[i].pos) / deltaTime;
//        this->m_points[i].pos = m_points[i].predPos;
//        m_vertices[i].position = m_points[i].pos;
//    }
//}
