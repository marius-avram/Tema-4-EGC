#include "AsteroidCamera.h" 
#include "Player.h"
#include "Constants.h"

AsteroidCamera::AsteroidCamera(Vector3D asteroid_position, float speed, Player &player):
		player(player), 
		speed(speed),
		position(asteroid_position) {

}

AsteroidCamera::~AsteroidCamera(){
	
}

void AsteroidCamera::render(){
	Vector3D center = player.getPosition();
	gluLookAt(position.x, position.y, position.z, 
			  center.x, center.y, center.z,
			  0, 1, 0);
	position.x -= speed;
}

bool AsteroidCamera::canDelete(){
	std::cout << "asteroid" << position.x << std::endl;
	std::cout << "min" << X_MIN+10.0f << std::endl;
	if (position.x < X_MIN+10.0f){
		return true;
	}
	else {
		return false;
	}
}

bool AsteroidCamera::isColliding(Vector3D position, float scale){
	return false;
}