#ifndef _ASTEROID_CAMERA_H_
#define _ASTEROID_CAMERA_H_
#include "Vector3D.h" 
#include "Object.h" 

class Player;
class AsteroidCamera : public Object {
	public:
		AsteroidCamera(Vector3D asteroid_position, float speed, Player &player);
		virtual ~AsteroidCamera();
		void render();
		bool canDelete();
		bool isColliding(Vector3D position, float scale);
	private:
		Vector3D position;
		float speed;
		Player &player;

};

#endif 