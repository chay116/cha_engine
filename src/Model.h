//
// Created by Chaeyoung Lim on 2022/02/18.
//

#ifndef CHA_ENGINE_MODEL_H
#define CHA_ENGINE_MODEL_H

#include "common.h"
#include "Mesh/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

CLASS_PTR(Model);
class Model {
public:
    static ModelUPtr Load(const std::string& filename);

    int GetMeshCount() const { return (int)m_meshes.size(); }
    MeshSPtr GetMesh(int index) const { return m_meshes[index]; }
    void Draw(const Program* programs) const;

private:
    Model() {}
    bool LoadByAssimp(const std::string& filename);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene);

    std::vector<MeshSPtr> m_meshes;
    std::vector<MaterialSPtr> m_materials;
};



#endif //CHA_ENGINE_MODEL_H
