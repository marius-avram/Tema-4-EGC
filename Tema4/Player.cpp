#include "Player.h"
#include "DynamicCamera.h"
#include "Game.h"
#include "Mesh.h"
#define _USE_MATH_DEFINES
#include <math.h>

Player::Player(DynamicCamera &camera): camera(camera){
	mesh = new Mesh("models/starship.off");
	scale = 1.5f;
	transparency = 0.2f;
	health = 100;
}

Player::~Player(){
}

void Player::render(){
	GLfloat material_sphere[] = {1.0f, 1.0f, 0.0f, transparency};
	glEnable(GL_LIGHTING);
	
	glPushMatrix();
		glPushMatrix();
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(-89.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, -1.3f, 0.0f);
			glScalef(3.0f, 3.0f, 3.0f);
			updatePosition();
			mesh->render();
		glPopMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -1.7f);
			glTranslatef(-0.1f, 0.0f, 0.0f);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_sphere); 
			if (Game::getCameraMode()==DYNAMIC){
				// Rezolvam glitchul cu transparenta sferei
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			glutSolidSphere(scale, 150, 150);
		glPopMatrix();
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
}

void Player::updatePosition(){
	position = camera.getPosition();
	if (Game::getCameraMode()==THIRD){
		camera.force_translate_Forward(camera.getTPSRadius());
		position = camera.getPosition();
		camera.force_translate_Forward(-camera.getTPSRadius());
	}
	else if (Game::getCameraMode()==DYNAMIC){
		camera.force_translate_Forward(camera.getTPSRadius());
		position = camera.getPosition();
		camera.force_translate_Forward(-camera.getTPSRadius());
		position.x -= 11.0f;
	}
}

Vector3D Player::getPosition(){
	return position;
}

void Player::doDamage(){
	transparency -= 0.04f;
	health -= 20;
}

bool Player::isDestroyed(){
	return (health <= 0);
}

int Player::getHealth(){
	if (health <= 0){
		return 0;
	}
	else{ 
		return health;
	}
}

float Player::getScale(){
	return scale;
}