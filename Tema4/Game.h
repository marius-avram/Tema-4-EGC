#pragma once 
#include <ctime>
#include "DynamicCamera.h"
#include "Object.h"
#include <vector> 

enum CAMERA {COCKPIT, THIRD, DYNAMIC, ASTEROID};
class Player;
class Background;
class AsteroidStorm;
class AsteroidCamera;
class Game{
	public:
		static void init();
		static void freeResources();
		static void display();
		static void reshape(int width, int height);
		static void idle();
		static void pickObject(int x, int y);
		static int processHits(int hits, unsigned int buffer[]);
		static void pickAction(int object);
		static void keyboard(unsigned char ch, int x, int y);
		static void mouseKey(int button, int state, int x, int y);
		static int getCameraMode();
		static void printText(float x, float y, char *text);
		static void setHudText(char *text);
		static void resetHudText();
		static void displayHud();
		static bool isgameOver();

		static DynamicCamera dynamic_camera;
		static AsteroidCamera *asteroid_camera;
		static AsteroidStorm *a_storm;
		static Player *player;
		static Background *background;
		static float total_camera_angleY;
		static float MOUSE_SPEED, MOVING_SPEED, 
					 TPS_CAMERA_RADIUS, COLLISION_RADIUS;
		static int offsetx, offsety, width, height;
		static float forward_character_pos, right_character_pos;
		static int camera_mode;
		static char hud_text[60];
		static int score;
		static std::vector<Object*> pickable_objects;
		static std::vector<Object*> objects;
		static bool flashlight_on;
		static bool can_pick_camera;
};