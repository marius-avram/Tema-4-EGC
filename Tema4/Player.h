#ifndef _PLAYER_H_
#define _PLAYER_H_
#pragma once
#include "Vector3D.h"

class DynamicCamera;
class Mesh;
class Player {
	public:
		Player(DynamicCamera &camera);
		virtual ~Player();
		void updatePosition();
		Vector3D getPosition();
		void render();
		void doDamage();
		bool isDestroyed();
		int getHealth();
		float getScale();

	private:
		void drawSphere(double radius, int lats, int longs);
		DynamicCamera &camera;
		Vector3D position;
		Mesh *mesh; 
		float transparency; 
		int health;
		float scale;
};

#endif 