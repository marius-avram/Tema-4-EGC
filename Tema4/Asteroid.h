#ifndef _ASTEROID_H_
#define _ASTEROID_H_
#pragma once 
#include "Vector3D.h"
#include "Object.h"
#include <vector> 

enum GameState { BEGINNING, RUNTIME };

class Asteroid : public Object {
	public:
		Asteroid(GameState state);
		virtual ~Asteroid();
		void init(GameState state);
		void render();
		Vector3D getPosition();
		float getScale();
		void setColor(float red, float green, float blue);
		void setTransparency(float transparency);
		float getSpeed();
		void destroy();
		bool canDelete();
		bool isColliding(Vector3D position, float scale);
		bool isInDestruction();
		static void initColors();

	private:
		void renderPolyhedron();
		void restoreInitialColor();
		// In functie de tip obiectele pot avea un numar diferit de fete 
		// Nu toti asteroizii sunt reprezentati ca dodecahedron. Tipul 
		// va fi generat aleator
		int type;
		// De asemenea toate celelalte elemente vor fi generate aleator
		Vector3D position;
		float scale;
		float speed;
		float color[4];
		float transparency;
		float initial_color[4];
		// Numarul de cadre de cand a inceput distrugerea asteroidului
		int steps;
		// Asteroidul este sau nu in decurs de distrugere
		bool in_destruction;

		static std::vector<std::vector<float>> colors;
		static bool colors_inited;
};
#endif