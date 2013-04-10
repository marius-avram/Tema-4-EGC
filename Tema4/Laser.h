#ifndef _LASER_H_
#define _LASER_H_
#pragma once 
#include "Vector3D.h" 
#include "Object.h"

class Laser : public Object {
	public:
		Laser(Vector3D start, Vector3D end);
		virtual ~Laser();
		void render();
		bool canDelete();
		bool isColliding(Vector3D position, float scale);
	private:
		Vector3D start, middle, end;
		float fraction;
};

#endif 