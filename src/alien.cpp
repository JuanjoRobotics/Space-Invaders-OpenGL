#include <alien.h>

const std::string rootPath = "resources/objects/";
alienEnum randomAlien();

std::vector<Alien> Alien::generate(unsigned int count) {
	std::vector<Alien> aliens;
	aliens.reserve(count);

	for (unsigned int i = 0; i < count; i++) {
		std::string path = rootPath + alienPaths.at(randomAlien());
		aliens.emplace_back(path);
	}

	return aliens;
}

void Alien::addAlien(std::vector<Alien>& aliens) {
	std::string path = rootPath + alienPaths.at(randomAlien());
	aliens.emplace_back(path);
}

alienEnum randomAlien() {
	return static_cast<alienEnum>(rand() % 5);
}