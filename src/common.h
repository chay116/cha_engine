#ifndef CHA_ENGINE__COMMON_H
#define CHA_ENGINE__COMMON_H

#include <string>
#include <optional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#ifndef M_PI
const double M_PI = 3.1415926535897932384626433832795;
#endif

#ifndef M_EPSION
const float M_EPSION = 0.0000001f;
#endif

#define CLASS_PTR(klassName) \
class klassName; \
using klassName ## UPtr = std::unique_ptr<klassName>; \
using klassName ## SPtr = std::shared_ptr<klassName>; \
using klassName ## WPtr = std::weak_ptr<klassName>;

std::optional<std::string> LoadTextFile(const std::string& filename);
glm::vec3 GetAttenuationCoeff(float distance);
float RandomRange(float minValue = 0.0f, float maxValue = 1.0f);

#endif // CHA_ENGINE_COMMON_H