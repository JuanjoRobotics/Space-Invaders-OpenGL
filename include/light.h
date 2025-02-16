#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>
#include <camera.h>

enum LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

class Light {
private:
    glm::vec3 Position;
    glm::vec3 Direction;
    LightType Type;
    // attributes
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Color;
    // attenuation
    float Constant;
    float Linear;
    float Quadratic;
    // spotlight attributes
    float CutOff;
    float OuterCutOff;

public:
    // Directional light
    Light(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f))
          : Direction(direction), Ambient(ambient), Diffuse(diffuse), Specular(specular), Color(color), Type(DIRECTIONAL) {}
    // Point light
    Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f))
          : Position(position), Ambient(ambient), Diffuse(diffuse), Specular(specular), Constant(constant), Linear(linear), Quadratic(quadratic), Color(color), Type(POINT) {}
    // Spot light
    Light(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f))
         : Position(position), Direction(direction), Ambient(ambient), Diffuse(diffuse), Specular(specular), Constant(constant), Linear(linear), Quadratic(quadratic), CutOff(cutOff), OuterCutOff(outerCutOff), Color(color), Type(SPOT) {}

    void setUniforms(Shader &shader,  std::string shaderName) {
        shader.setVec3(shaderName + ".ambient", Ambient);
        shader.setVec3(shaderName + ".diffuse", Diffuse);
        shader.setVec3(shaderName + ".specular", Specular);
        shader.setVec3(shaderName + ".color", Color);
        if (Type != POINT)
            shader.setVec3(shaderName + ".direction", Direction);
        if (Type != DIRECTIONAL) {
            shader.setVec3(shaderName + ".position",Position);
            shader.setFloat(shaderName + ".constant", Constant);
            shader.setFloat(shaderName + ".linear", Linear);
            shader.setFloat(shaderName + ".quadratic", Quadratic);
        }
        if (Type == SPOT) {
            shader.setFloat(shaderName + ".cutOff", CutOff);
            shader.setFloat(shaderName + ".outerCutOff", OuterCutOff);
        }
    }
    void setPosition(glm::vec3 position) {
        Position = position;
    }
    void setDirection(glm::vec3 direction) {
        Direction = direction;
    }

};
#endif