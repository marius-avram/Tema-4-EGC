#ifndef _ASTEROID_STORM_H_
#define _ASTEROID_STORM_H_
#pragma once 
#include <list>
#include "Object.h"
#include "Asteroid.h" 
class AsteroidStorm : public Object{
	public:
		AsteroidStorm();
		virtual ~AsteroidStorm();
		void init();
		void render();
		void insertAsteroid(GameState state);
		void destroyAsteroid(int index);
		Asteroid* getAsteroid(int index);
		bool canDelete();
		bool isColliding(Vector3D position, float scale);

	private:
		static unsigned int number; 
		std::list<Asteroid*> asteroids;
};

bool cmpAsteroids(Asteroid* asteroid1, Asteroid* asteroid2);

#endif