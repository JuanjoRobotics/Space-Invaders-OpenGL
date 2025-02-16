#ifndef PLAYER_H
#define PLAYER_H

#include <model.h>
#include <memory>
#include <mutex>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* This class must be unique:
    - Delete the copy constructor and the assignment operator.
	- Make constructor private and use unique pointer.
*/
class Player: public Model
{
public:
	enum class Direction {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	static Player& getInstance(const std::string& path)
    {
        static std::once_flag flag;
        std::call_once(flag, [&]() { instance.reset(new Player(path)); });
        return *instance;
    }
	~Player() {};
	void processKeyboard(Direction direction, float deltaTime);
//private:

	Player(const std::string& path) : Model(path) {};

	static std::unique_ptr<Player> instance;
	float speed = 1.0f;
};

#endif // PLAYER_H