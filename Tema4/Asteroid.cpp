#include "Asteroid.h"
#include "Constants.h"
#include <time.h> 
#include <math.h>

bool Asteroid::colors_inited = false;
std::vector<std::vector<float>> Asteroid::colors;
Asteroid::Asteroid(GameState state){
	init(state);
	steps = 0;
	in_destruction = false;
	transparency = 1.0f;
}

Asteroid::~Asteroid(){
}

void Asteroid::init(GameState state){
	type = rand()%2;
	int index = rand() % colors.size(); 
	color[0] = colors[index][0];
	color[1] = colors[index][1];
	color[2] = colors[index][2];
	color[3] = colors[index][3]; 
	initial_color[0] = color[0];
	initial_color[1] = color[1];
	initial_color[2] = color[2];
	initial_color[3] = color[3];
	float x;
	if (state==RUNTIME){
		x = X_MAX;
	}
	else if (state==BEGINNING){
		x = X_MIN + 5.0f + (float)rand()/RAND_MAX*(X_MAX-10.0f - X_MIN);
	}
	float y = Y_MIN + 5.0f + (float)rand()/RAND_MAX*(Y_MAX-10.0f - Y_MIN);
	float z = Z_MIN + 5.0f + (float)rand()/RAND_MAX*(Z_MAX-10.0f - Z_MIN);
	position = Vector3D(x, y, z);
	scale = 2.0f + (float)rand()/RAND_MAX * 4.0f;
	speed = 0.05f + (float)rand()/(RAND_MAX*100.0f);
}

void Asteroid::render(){
	if (transparency != 0.0f){
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glEnable(GL_LIGHTING);
		glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			if (!in_destruction){
				glPushMatrix();
					glScalef(scale, scale, scale);
					renderPolyhedron();
				glPopMatrix();
				position.x -= speed;
			}
			else{
				if (steps<=50){
					// Initial asteroidul ramane nemiscat pentru un numar cadre dupa care 
					// va incepe distrugerea propri-zisa. Motivul: pentru a putea fi 
					// vizibila modificare culorii ceruta in enunt.
					glPushMatrix();
						glScalef(scale, scale, scale);
						renderPolyhedron();
					glPopMatrix();
					steps++;
					if (steps==50){
						restoreInitialColor();
					}
				}
				else{
					steps -= 50;
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					// Daca este in curs de distrugere atunci se vor crea 
					// mai multe obiecte de dimensiune mai mica care se departeaza
					glPushMatrix();
						glTranslatef(0.05f * steps, 0.05f * steps, 0.05f * steps);
						glScalef(scale/4, scale/4, scale/4);
						renderPolyhedron();
					glPopMatrix();
					glPushMatrix();
						glTranslatef(-0.05f * steps, -0.05f * steps, -0.05f * steps); 
						glScalef(scale/4, scale/4, scale/4);
						renderPolyhedron();
					glPopMatrix();
					glPushMatrix();
						glTranslatef(0.05f*steps, 0.05f*steps, -0.05f*steps);
						glScalef(scale/4, scale/4, scale/4);
						renderPolyhedron();
					glPopMatrix();
					glPushMatrix();
						glTranslatef(-0.05f*steps, -0.05f*steps, 0.05f*steps);
						glScalef(scale/4, scale/4, scale/4);
						renderPolyhedron();
					glPopMatrix();
					glPushMatrix();
						glTranslatef(-0.05f*steps, 0.05f*steps, 0.05f*steps);
						glScalef(scale/4, scale/4, scale/4);
						renderPolyhedron();
					glPopMatrix();

					// Crestem pasul de cand a inceput distrugerea asteroidului
					steps++;
					// Scadem transparenta pentru efectul de fade-out
					color[3] -= 0.01f;
					glDisable(GL_BLEND);
					steps += 50;
				}
			}
		glPopMatrix();
		glDisable(GL_LIGHTING);
	}
}

void Asteroid::renderPolyhedron(){
	switch (type){
		case 0:
			glutSolidDodecahedron();
			break;
		case 1: 
			glutSolidIcosahedron();
			break;
		default: 
			break;
	}
}

Vector3D Asteroid::getPosition(){
	return position;
}

float Asteroid::getScale(){
	return scale;
}

void Asteroid::initColors(){
	if (!colors_inited){
		float color1[] = {0.28f, 0.31f, 0.30f, 1.0f};
		float color2[] = {0.16f, 0.18f, 0.16f, 1.0f};
		float color3[] = {0.52f, 0.52f, 0.41f, 1.0f};
		float color4[] = {0.09f, 0.11f, 0.21f, 1.0f};
		float color5[] = {0.3f, 0.2f, 0.18f, 1.0f};
		colors.push_back(std::vector<float>(color1, color1+4)); 
		colors.push_back(std::vector<float>(color2, color2+4));
		colors.push_back(std::vector<float>(color3, color3+4));
		colors.push_back(std::vector<float>(color4, color4+4));
		colors.push_back(std::vector<float>(color5, color5+4));
		colors_inited = true;
	}
}

void Asteroid::setColor(float red, float green, float blue){
	color[0] = red;
	color[1] = green;
	color[2] = blue;
}

void Asteroid::setTransparency(float transparency){
	color[3] = transparency;
	this->transparency = transparency;
}

float Asteroid::getSpeed(){
	return speed;
}

void Asteroid::restoreInitialColor(){
	color[0] = initial_color[0];
	color[1] = initial_color[1];
	color[2] = initial_color[2];
}

void Asteroid::destroy(){
	steps = 1;
	in_destruction = true;
}

bool Asteroid::canDelete(){
	return ((color[3] <= 0.0f) || (position.x < X_MIN + 5));
}

bool Asteroid::isInDestruction(){
	return in_destruction;
}

bool Asteroid::isColliding(Vector3D position, float scale){
	float distance = 0;
	distance = sqrt(pow(position.x - this->position.x, 2) + 
					pow(position.y - this->position.y, 2) +
					pow(position.z - this->position.z, 2));
	if (distance < scale + this->scale * 1.8f){
		return true;
	}
	else{
		return false;
	}
}

