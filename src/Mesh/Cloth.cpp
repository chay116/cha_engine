//
// Created by Chaeyoung Lim on 2022/06/14.
//

#include "Cloth.h"

ClothUPtr Cloth::Create(uint32_t resX, uint32_t resY, float sizeX, float sizeY, float k_stiff,
                        bool hasPosConstr, glm::vec3 initPos) {
    auto cloth = ClothUPtr(new Cloth(resX, resY, sizeX, sizeY, k_stiff, hasPosConstr, initPos));
    cloth->Init();
    return std::move(cloth);
}
#include <iostream>
void Cloth::Init() {
    std::vector<uint32_t> indices;
    InitIndexArray(indices);
    InitVertexArray();

    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(
            GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,
            m_vertices.data(), sizeof(Vertex), m_vertices.size());
    m_indexBuffer = Buffer::CreateWithData(
            GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,
            indices.data(), sizeof(uint32_t), indices.size());
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, false,
                              sizeof(Vertex), 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, false,
                              sizeof(Vertex), offsetof(Vertex, normal));
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, false,
                              sizeof(Vertex), offsetof(Vertex, texCoord));
}

void Cloth::InitIndexArray(std::vector<uint32_t> &indices)
{
    unsigned int countVer = 0;
    unsigned int totalTris = (m_resX - 1) * (m_resY - 1) * 2;
    for (unsigned int j = 0; j < m_resY - 1; ++j)
    {
        for (unsigned int i = 0; i < m_resX - 1; ++i)
        {
            //             1/5._______.4
            //                |       |
            //                |       |
            //               2|_______|3/6
            indices.push_back((GLuint)(j * m_resX + i));
            indices.push_back((GLuint)((j + 1) * m_resX + i));//j*m_resX + i + 1
            indices.push_back((GLuint)((j + 1) * m_resX + i + 1));
            indices.push_back((GLuint)(j * m_resX + i + 1)); //(j + 1)*m_resX + i + 1)
            indices.push_back((GLuint)(j * m_resX + i)); // (j + 1)*m_resX + i)
            indices.push_back((GLuint)((j + 1) * m_resX + i + 1));//j*m_resX + i
            countVer += 6;
        }
    }
    assert(countVer == totalTris*3);
}


void Cloth::InitVertexArray() {
    // setting up initial m_points positions
    int totalPoints = m_resX * m_resY;
    float shearRestLength = sqrt(m_sizeX * m_sizeX + m_sizeY * m_sizeY);
    m_vertices.resize(totalPoints);
    for (int j = 0; j < m_resY; ++j)
    {
        for (int i = 0; i < m_resX; ++i)
        {
            glm::vec3 newPos = glm::vec3((float)i * m_sizeX, 0.0f, (float)j * m_sizeY);
            newPos += m_initPos;
            float invNewMass = 2.0f;

            // initialize point constraint
            Point newP;
            newP.pos = newPos;
            newP.vel = glm::vec3(0.0f, 0.0f, 0.0f);
            newP.accel = glm::vec3(0.0f, 0.0f, 0.0f);

            m_vertices[FindGridIndex(i, j)] = Vertex {
                    newPos, glm::vec3(0.0f, 0.0f, 1.0f),
                    glm::vec2(i / m_resX, j / m_resY), glm::vec3(0.0f, 0.0f, 0.0f)
            };

            if (m_hasPosConstr && (j == m_resY - 1 && (i == 0 || i == m_resX - 1)))  // fix two m_points
            {
                newP.invMass = 0;
                m_points.push_back(newP);
                m_posConstraintList.push_back(newPos);
            }
            else
            {
                newP.invMass = invNewMass;
                m_points.push_back(newP);
            }

            // initialize constraint and m_restLength
            for (int x = -1; x < 2; ++x)
            {
                for (int y = -1; y < 2; ++y)
                {
                    if (x == 0 && y == 0)
                        continue;
                    int currX = i + x;
                    int currY = j + y;
                    //if (i == 0 && j == 0) printf("currX: %d, currY: %d\n", currX, currY);
                    int currIndex = FindGridIndex(currX, currY);
                    int currOrigin = FindGridIndex(i, j);
                    if (IsInside(currX, currY) && currIndex > currOrigin)
                    {
                        //if (i == 0 && j == 0) printf("adding\n");
                        m_distConstraintList.emplace_back(glm::vec2(currOrigin, currIndex));
                        if (x == y || x == -y)  // shear
                            m_restLength.push_back(shearRestLength);
                        else if (x > y) // horizontal; works only when we restrict that currIndex > currOrigin
                            m_restLength.push_back(m_sizeX);
                        else // verticle
                            m_restLength.push_back(m_sizeY);
                    }
                }
            }
        }
    }
    assert(totalPoints == m_points.size());
    assert(m_distConstraintList.size() == m_restLength.size());
}

void Cloth::SetPositionConstraint()
{
    // top left point = first point in last row
    uint32_t tlIndex = m_resX * (m_resY - 1);
    this->m_points[tlIndex].pos = m_posConstraintList[0];
    // top right point = last point in last row
    uint32_t trIndex = (m_resX * m_resY) - 1;
    this->m_points[trIndex].pos = m_posConstraintList[1];
}

void Cloth::Draw(const Program* program) const {
    m_vertexBuffer->Update(m_vertices.data());
    m_vertexLayout->Bind();
    glDrawElements(m_primitiveType, m_indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
}

void Cloth::Update(float deltaTime, float dampingRate, bool hasPosConstr, int solverIter, glm::vec3 sphereCenter, float sphereRadius)
{
     printf("updating... %f\n", deltaTime);
    // external forces (gravity ONLY)
    // --------------------------------
    glm::vec3 gravity = glm::vec3(0, -9.8f, 0);
    auto gravity_force = deltaTime * this->m_points[0].invMass * gravity;
    // COARSE: damping velocities
    auto damping_velocity = pow((1 - dampingRate), deltaTime);
    for (int i = 0; i < this->m_points.size(); ++i)
    {
        if (!(i == m_resX * (m_resY - 1) || i == (m_resX * m_resY) - 1))  // do not conserve external forces
        {
            this->m_points[i].vel += gravity_force;
            this->m_points[i].vel *= damping_velocity;
        }
        // add the predicted position with velocities
        this->m_points[i].predPos = this->m_points[i].pos + deltaTime * this->m_points[i].vel;
    }

    // project constraints (ONLY distance contraints and position contraints for now)
    // ---------------------------------
    sphereRadius = sphereRadius * 1.03f;
    for (int iter = 0; iter < solverIter; iter++)
    {
        // distance contraint
        for (int i = 0; i < m_distConstraintList.size(); ++i)
        {
            glm::vec2 currDistConstr = m_distConstraintList[i]; // point-pair
            Point pt1 = m_points[currDistConstr[0]];
            Point pt2 = m_points[currDistConstr[1]];

            glm::vec3 vecP2P1 = pt1.predPos - pt2.predPos;
            float magP2P1 = mag(vecP2P1);
            if (magP2P1 <= M_EPSION)
                return;
            float invMass = pt1.invMass + pt2.invMass;
            if (invMass <= M_EPSION)
                return;

            glm::vec3 n_val = vecP2P1 / magP2P1;  // direction
            float s_val = (magP2P1 - m_restLength[i]) * (1 / invMass);  // scaler

            glm::vec3 distProj = s_val * n_val * m_k_stiff;
            if (pt1.invMass > 0.0) // should always be true
                m_points[currDistConstr[0]].predPos -= (distProj * pt1.invMass);
            if (pt2.invMass > 0.0) // should always be true
                m_points[currDistConstr[1]].predPos += (distProj * pt2.invMass);
        }
        // position constraint
        if (hasPosConstr)
            SetPositionConstraint();

        // collision constraints
        for (int i = 0; i < m_points.size(); ++i)
        {
            glm::vec3 p2c = m_points[i].predPos - sphereCenter; // distance between current predpos to the center of the sphere
            float dist = mag(p2c);
            // collision detection
            if (dist - sphereRadius < M_EPSION) // collide with the sphere
            {
                float distToGo = sphereRadius - dist; // the distance to set the current predpos to the surface of the sphere
                m_points[i].predPos += p2c * distToGo;  // direction * distance
            }
        }
    }

    // commit the velocity and the position changes
    // ---------------------------------
    for (int i = 0; i < this->m_points.size(); ++i)
    {
        // commit velocity based on position changes
        this->m_points[i].vel = (m_points[i].predPos - m_points[i].pos) / deltaTime;
        this->m_points[i].pos = m_points[i].predPos;
        m_vertices[i].position = m_points[i].pos;
   }
}