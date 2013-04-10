#ifndef _OBJECT_H_
#define _OBJECT_H_
#pragma once 
#include "Vector3D.h" 

// O clasa pur abstracta ce contine metodele ce trebuie
// implementate de toate obiectele din joc
class Object{
	public:
		// Afiseaza obiectul respectiv
		virtual void render() = 0;
		// Un obiect poate fi sters cand acesta nu mai apare pe ecran
		// fie este transparent, fie a iesit a iesit din aria vizuala, etc.
		virtual bool canDelete() = 0;
		// Verifica daca nu cumva jucatorul se ciocneste de obiectul 
		// ce mosteneste aceasta clasa 
		virtual bool isColliding(Vector3D position, float scale) = 0;
};

#endif 