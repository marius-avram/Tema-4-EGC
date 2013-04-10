//include librarii de opengl, glut si glu
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")

//includes
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>

#include "glut.h"
#include "Game.h"


int main(int argc, char *argv[]){

	//init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	//init window
	
	glutInitWindowSize(Game::width, Game::height);
	glutInitWindowPosition(Game::offsetx, Game::offsety);
	glutCreateWindow("SpaceEscape 2013");
	Game::init();

	//callbacks
	glutDisplayFunc(Game::display);
	glutReshapeFunc(Game::reshape);
	glutKeyboardFunc(Game::keyboard);
	glutMouseFunc(Game::mouseKey);
	glutIdleFunc(Game::idle);
	

	glEnable(GL_DEPTH_TEST);

	//set background
	glClearColor(0.0f,0.0f,0.01f,1.0f);


	//loop
	glutMainLoop();

	return 0;
}