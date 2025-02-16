#include <player.h>

// Define the static member
std::unique_ptr<Player> Player::instance = nullptr;

void Player::processKeyboard(Direction direction, float deltaTime) {
	// move the player
	float velocity = speed * deltaTime;
	if (direction == Direction::LEFT)
		Position.x -=  velocity;
	if (direction == Direction::RIGHT)
		Position.x += velocity;
	if (direction == Direction::UP)
		Position.y += velocity;
	if (direction == Direction::DOWN)
		Position.y -= velocity;
};