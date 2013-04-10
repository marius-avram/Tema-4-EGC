#include "Background.h" 
#include <time.h> 
#include <glut.h>
#include <iostream>
#include "Constants.h" 

Background::Background(){
	srand(time(NULL));
	xmin = X_MIN;  
	xmax = X_MAX;
	ymin = Y_MIN;
	ymax = Y_MAX;
	zmin = Z_MIN;
	zmax = Z_MAX;
	indexes.push_back(0);
	createStars(xmin, xmax, zmin, zmax);
	createStars(ymin, ymax, zmin, zmax);
	createStars(ymin, ymax, zmin, zmax);
	createStars(xmin, xmax, ymin, ymax);
	createStars(xmin, xmax, ymin, ymax);
	createStars(xmin, xmax, zmin, zmax);
}

Background::~Background(){
}

void Background::createStars(float xmin, float xmax, 
							 float zmin, float zmax){
	float step = 5.0f;
	for(float i=xmin; i<xmax; i+=step){
		for(float j=zmin; j<zmax; j+=step){
			xpoints.push_back((float)rand()/(float)RAND_MAX*step + i);
			ypoints.push_back((float)rand()/(float)RAND_MAX);
			zpoints.push_back((float)rand()/(float)RAND_MAX*step + j);
		}
	}
	indexes.push_back(xpoints.size()-1);
}

void Background::renderPlane(int start_index){
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	for(unsigned int j=indexes[start_index]; j<indexes[start_index+1]; j++){
		glVertex3f(xpoints[j], ypoints[j], zpoints[j]);
	}
	glEnd();
}

void Background::render(){
		// Pozitiile planelor sunt raportate la nava
		// Planul de jos
		renderPlane(0);
		// Planul din spate
		glPushMatrix();
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, 75.0f, 0.0f);
			renderPlane(1);
		glPopMatrix();
		// Planul din fata
		glPushMatrix();
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, -175.0f, 0.0f);
			renderPlane(2);
		glPopMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		// Planul stanga
		glPushMatrix();
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 75.0f, 0.0f);
			renderPlane(3);
		glPopMatrix();
		// Planul Dreapta
		glPushMatrix();
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 75.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, -75.0f);
			renderPlane(4);
		glPopMatrix();
		// Planul Sus
		glPushMatrix();
			glTranslatef(0.0f, 75.0f, 0.0f);
			renderPlane(5);
		glPopMatrix();
}