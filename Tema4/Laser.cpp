#include "Laser.h" 

Laser::Laser(Vector3D start, Vector3D end){
	this->start = start;
	this->end = end;
	fraction = 1;
}

Laser::~Laser(){
}

void Laser::render(){
	if (fraction > 0){
		middle.x = fraction*start.x + (1-fraction)*end.x;
		middle.y = fraction*start.y + (1-fraction)*end.y;
		middle.z = fraction*start.z + (1-fraction)*end.z;
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(10, 0xf0f0);
		glColor3f(0.0f, 0.0f, 1.0f);
		glLineWidth(3.0f);
		glBegin(GL_LINES);
			glVertex3f(middle.x, middle.y, middle.z);
			glVertex3f(end.x, end.y, end.z);
		glEnd();
		fraction -= 0.003f;
	} 
}

bool Laser::canDelete(){
	return (fraction <= 0);
}

bool Laser::isColliding(Vector3D position, float scale){
	return false;
}
