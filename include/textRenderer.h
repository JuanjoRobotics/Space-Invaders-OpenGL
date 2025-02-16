#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>
#include <iostream>

#include <shader.h>

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer {

private:
	unsigned int VAO;
	unsigned int VBO;
	std::map<GLchar, Character> Characters;
	Shader shader;

public:
	TextRenderer( const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT);
void renderText(std::string text, float x, float y, float scale, glm::vec3 color);
};

#endif