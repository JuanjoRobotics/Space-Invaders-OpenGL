#ifndef ALIEN_H
#define ALIEN_H

#include <model.h>

#include <string>
#include <unordered_map>

enum alienEnum {
	INVADER1,
	INVADER2,
	INVADER3,
	INVADER4,
	INVADER5,
};

static const std::unordered_map<alienEnum, std::string> alienPaths = {
	{INVADER1, "invader-1/source/invader_1.obj"},
	{INVADER2, "invader-2/source/invader_2.obj"},
	{INVADER3, "invader-3/source/invader_3.obj"},
	{INVADER4, "invader-4/source/invader_4.obj"},
	{INVADER5, "invader-5/source/invader_5.obj"},
};

class Alien : public Model
{
public:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);

	Alien(const std::string& path) : Model(path) {}
	static std::vector<Alien> generate(unsigned int count);
	static void addAlien(std::vector<Alien>& aliens);
};

#endif // ALIEN_H
