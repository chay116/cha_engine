//
// Created by Chaeyoung Lim on 2022/02/10.
//

#ifndef CHA_ENGINE_PROGRAM_H
#define CHA_ENGINE_PROGRAM_H


#include "common.h"
#include "Shader.h"

CLASS_PTR(Program)

class Program {
public:
    static ProgramUPtr Create(
            const std::vector<ShaderSPtr> &shaders);

    static ProgramUPtr Create(
            const std::string &vertShaderFilename,
            const std::string &fragShaderFilename);

    ~Program();

    uint32_t Get() const { return m_program; };

    void Use() const;

    void SetUniform(const std::string &name, int value) const;

    void SetUniform(const std::string &name, float value) const;


    void SetUniform(const std::string &name, const glm::mat4 &value) const;

    void SetUniform(const std::string &name, const glm::vec2 &value) const;

    void SetUniform(const std::string &name, const glm::vec3 &value) const;

    void SetUniform(const std::string &name, const glm::vec4 &value) const;

private:
    Program() {};

    bool Link(
            const std::vector<ShaderSPtr> &shaders);

    uint32_t m_program{0};
};


#endif //CHA_ENGINE_PROGRAM_H
