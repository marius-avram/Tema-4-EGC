#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h> 
#include "Game.h"
#include "Player.h"
#include "Background.h"
#include "AsteroidStorm.h" 
#include "AsteroidCamera.h"
#include "Laser.h"
#include <time.h> 

int Game::offsetx = 100;
int Game::offsety = 100;
int Game::width = 800;
int Game::height = 600;
int Game::camera_mode = DYNAMIC;
float Game::total_camera_angleY = 0.0f;
float Game::MOUSE_SPEED = 0.001f;
float Game::MOVING_SPEED = 0.3f;
float Game::TPS_CAMERA_RADIUS = 30.0f;
float Game::COLLISION_RADIUS = 1.2f;
DynamicCamera Game::dynamic_camera(TPS_CAMERA_RADIUS);
AsteroidCamera *Game::asteroid_camera = NULL;
Player *Game::player = NULL;
Background *Game::background = NULL;
AsteroidStorm *Game::a_storm = NULL;
char Game::hud_text[60];
int Game::score = 0;
std::vector<Object*> Game::pickable_objects;
std::vector<Object*> Game::objects;
bool Game::flashlight_on = false;
bool Game::can_pick_camera = false;

void Game::init(){
	srand(time(NULL));
	
	// Initializarea celor doua tipuri de camere
	Game::dynamic_camera.init();
	dynamic_camera.setTPSCamera();
	dynamic_camera.setSimpleRotationTPS_OY(M_PI/2);

	Asteroid::initColors();
	background = new Background();

	a_storm = new AsteroidStorm();
	// Initializare jucator
	player = new Player(dynamic_camera);
	
	pickable_objects.push_back(a_storm);
	

	// Setam cele doua lumini omnidirectionale care 
	// provin de la cele doua stele ale galaxiei
	float position_star1[] = {0.0f, 1.0f, 0.0f, 0.0f};
	float ambient_star1[] = {0.5f, 0.5f, 0.5f, 0.2f};
	float diffuse_star1[] = {0.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_star1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_star1);
	glLightfv(GL_LIGHT0, GL_POSITION, position_star1);
	glEnable(GL_LIGHT0);
	
	float position_star2[] = {0.0f, 0.0f, -1.0f, 0.0f};
	float ambient_star2[] = {0.34f, 0.27f, 0.0f, 0.01f}; 
	float diffuse_star2[] = {0.34f, 0.27f, 0.0f, 0.01f};
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient_star2);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse_star2);
	glLightfv(GL_LIGHT3, GL_POSITION, position_star2);
	glEnable(GL_LIGHT3);
	// Resetare text ce trebuie afisat pe ecran
	resetHudText();

}

// Elibereaza resursele alocate in init. Acestea trebuie 
// eliberate cand se restarteaza jocul pentru ca se apeleaza
// inca o data init si alte noi resurse sunt alocate
void Game::freeResources(){
	delete player;
}


//functie afisare
void Game::display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	char score_text[20];
	if (!isgameOver()){
		sprintf_s(score_text, 20, "Scor: %i", score);
		setHudText(score_text);
		displayHud();
	}
	else{
		sprintf_s(score_text, 20, "Jocul s-a terminat!");
		setHudText(score_text);
		displayHud();
	}
	reshape(width, height);
	//setup view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (camera_mode==COCKPIT || camera_mode==DYNAMIC || camera_mode==THIRD){
		dynamic_camera.render();
	}
	else if (camera_mode==ASTEROID && asteroid_camera!=NULL){
		asteroid_camera->render();
		can_pick_camera = false;
		if (asteroid_camera->canDelete()){
			delete asteroid_camera;
			asteroid_camera = NULL;
			camera_mode = DYNAMIC;
		}
	}
	background->render();
	if (!isgameOver()){
		a_storm->render();
	}
	
	
	// Randeaza alte obiecte din scena care nu au comportamente
	// particulare precum cele ale asteroizilor sau navei
	std::vector<Object*>::iterator it;
	for (it=objects.begin(); it!=objects.end(); ++it){
		(*it)->render();
	}
	
	// Desenare personaj principal
	Vector3D camera_position = dynamic_camera.getPosition();
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
		glTranslatef(camera_position.x+1.0f, camera_position.y-1.0f, camera_position.z);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glRotatef((float)(-total_camera_angleY * 180 / M_PI), 0.0f, 1.0f, 0.0f);
		if (camera_mode==THIRD){
			// Daca suntem in modul TPS plasam obiectul mai in fata
			// relativ la camera, deoarece prima tanslatie depinde de
			// pozitia camerei. Iar in modul TPS camera este translata
			// in spate cu 10.
			glTranslatef(0, 0, TPS_CAMERA_RADIUS);
		}
		else if (camera_mode==DYNAMIC){

			glTranslatef(0, 0, TPS_CAMERA_RADIUS-11.0f);
		}
		player->render();
	glPopMatrix();


	// Creeam luminile spotlight ce pornesc de la nava
	if (flashlight_on){
		Vector3D pposition = player->getPosition();
		GLfloat color_spot1[] = {1.0f, 0.0f, 0.0f, 1.0f};
		GLfloat position_spot1[] = {pposition.x, pposition.y, pposition.z-5.0f, 1.0f};
		GLfloat direction_spot1[] = {1.0f, 0.0f, -0.1f};
		
		glLightfv(GL_LIGHT1, GL_POSITION, position_spot1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, color_spot1);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction_spot1);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 5.0f);
		glEnable(GL_LIGHT1);

		GLfloat color_spot2[] = {0.0f, 1.0f, 0.0f, 1.0f};
		GLfloat position_spot2[] = {pposition.x, pposition.y, pposition.z+7.0f, 1.0f};
		GLfloat direction_spot2[] = {1.0f, 0.0f, 0.1f};

		glLightfv(GL_LIGHT2, GL_POSITION, position_spot2);
		glLightfv(GL_LIGHT2, GL_AMBIENT, color_spot2);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction_spot2);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 5.0f);
		glEnable(GL_LIGHT2);
	}
	else { 
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	}
	
	// swap buffers
	glutSwapBuffers();
}

void Game::reshape(int width, int height){
	//set viewport
	glViewport(0,0,width,height);
	
	//set proiectie
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)width/(float)height,0.2,250);
	Game::height = height;
	Game::width = width;
}

void Game::pickObject(int x, int y){
	// buffer de selectie 
	GLuint buffer[1024];
	// numar de hitturi
	GLint nhits;
	// coordonate viewport curent
	GLint viewport[4];
	// iterator pe obiectele ce pot fi alese si vor fi "pseudo-randate" 
	std::vector<Object*>::iterator it;
	// indicele obiectului selectat 
	int object = 0;
	
	// Se obtin coordonatele viewportului curent
	glGetIntegerv(GL_VIEWPORT, viewport);

	memset(buffer,0x0, 1024*sizeof(GLuint));
	glSelectBuffer(1024, buffer);

	glRenderMode(GL_SELECT);

	// salvare matrice de proiecte curenta
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		
		// se va randa doar in zona ce inconjoara cursorul mouseului
		glGetIntegerv(GL_VIEWPORT, viewport);
		gluPickMatrix(x, viewport[3]-y, 1.0f, 1.0f, viewport);
		gluPerspective(45,(viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]),0.1,1000);
		glMatrixMode(GL_MODELVIEW);

		// se "deseneaza" scena : de fapt nu se va desena nimic 
		// in framebuffer ci se va folosi bufferul de selectie
		for(it=pickable_objects.begin(); it!=pickable_objects.end(); ++it){
			(*it)->render();
		}

		// restaurare matrice de proiectie initiala
		glMatrixMode(GL_PROJECTION);						
	glPopMatrix();				

	glMatrixMode(GL_MODELVIEW);
	// restaurarea modului de randare uzual si obtinerea numarului de hituri
	nhits=glRenderMode(GL_RENDER);	
	
	// procesare hituri
	if(nhits != 0){
		// returneaza indicele obiectului cel mai apropiat de observator
		object=processHits(nhits,buffer);
	}

	// Executa o anumita actiune asupra obiectului selectat
	pickAction(object);
	
}


int Game::processHits(GLint hits, GLuint buffer[])
{
	GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

	// pointer la inceputul bufferului ce contine hit recordurile
	ptr = (GLuint *) buffer;
	// se doreste selectarea obiectului cel mai aproape de observator
	minZ = 0xffffffff;
	for (int i = 0; i < hits; i++){
	  // numarul de nume numele asociate din stiva de nume
	  names = *ptr;
	  ptr++;
	  // Z-ul asociat hitului - se retine 
	  if (*ptr < minZ) {
		  numberOfNames = names;
		  minZ = *ptr;
		  // primul nume asociat obiectului
		  ptrNames = ptr+2;
	}
	  
	// salt la urmatorul hitrecord
	ptr += names+2;
  }

  // identificatorul asociat obiectului
  ptr = ptrNames;
  
  return *ptr;
     
}

void Game::pickAction(int object){
	// Daca s-a selectat un obiect
	if (object!=0 && !isgameOver()){
		if (!can_pick_camera){
			// Si nu se doreste pozitionarea unei camere pe asteroidul
			// selectat atunci se disturge asteroidul respectiv
			Vector3D player_pos;
			a_storm->destroyAsteroid(object);
			player_pos = player->getPosition();
			player_pos.z -= 0.8f;
			if (camera_mode==COCKPIT){
				player_pos.y -= 2.2f;
			}
			// Instantiere laser tun stanga
			objects.push_back(new Laser(player_pos, a_storm->getAsteroid(object)->getPosition()));
			player_pos = player->getPosition();
			player_pos.z += 0.8f;
			if (camera_mode==COCKPIT){
				player_pos.y -= 2.2f;
			}
			// Instantiere laser tun dreapta
			objects.push_back(new Laser(player_pos, a_storm->getAsteroid(object)->getPosition()));
			// Incrementare scor
			score += 10.0f;
		}
		else {
			// Altfel se doreste doreste instantierea unei camere
			Asteroid *picked_asteroid = a_storm->getAsteroid(object);
			picked_asteroid->setTransparency(0.0f);
			asteroid_camera = new AsteroidCamera(picked_asteroid->getPosition(),
												 picked_asteroid->getSpeed(), 
												 *player);
			camera_mode = ASTEROID;
		}
	}
}

void Game::displayHud(){
	if (strcmp(hud_text, "")!=0){
		// Dezactivam testul de adancime
		glDisable(GL_DEPTH_TEST);
		// Setam alt viewport
		if (!isgameOver()){
			glViewport(10, height-30, 5, 5);
		}
		else {
			glViewport((float)(width/2)-60.0f, (float)(height/2), 5, 5);
		}
		// Setam proiectie ortografica
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, width-1.0, 0.0, height-1.0, -1.0, 1.0);
		// Afisam textul
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		printText(0.0f, 0.0f, hud_text);
		glEnable(GL_DEPTH_TEST);
	}
}


void Game::keyboard(unsigned char ch, int x, int y){
	switch(ch){
		case 27:	//esc
			exit(0);
			break;

		case 'w':
			dynamic_camera.translate_Forward(MOVING_SPEED);
			player->updatePosition();
			break;
		case 'a':
			dynamic_camera.translate_Right(-MOVING_SPEED);
			player->updatePosition();
			break;
		case 's':
			dynamic_camera.translate_Forward(-MOVING_SPEED);
			player->updatePosition();
			break;
		case 'd':
			dynamic_camera.translate_Right(MOVING_SPEED);
			player->updatePosition();
			break;
		case 't': 
			if (camera_mode==THIRD){
				// Doar camera tps putem sa o mutam in ce directie vrem
				dynamic_camera.setRotationTPS_OX(MOUSE_SPEED*30, TPS_CAMERA_RADIUS);
			}
			break; 
		case 'g': 
			if (camera_mode==THIRD){
				dynamic_camera.setRotationTPS_OX(-MOUSE_SPEED*30, TPS_CAMERA_RADIUS);
			}
			break;
		case 'e':
			if (camera_mode==THIRD){
				dynamic_camera.setRotationTPS_OZ(MOUSE_SPEED*30, TPS_CAMERA_RADIUS);
			}
			break;
		case 'q':
			if (camera_mode==THIRD){
				dynamic_camera.setRotationTPS_OZ(-MOUSE_SPEED*30, TPS_CAMERA_RADIUS);
			}
			break;
		case 'z':
			if (camera_mode==THIRD){
				dynamic_camera.setRotationTPS_OY(MOUSE_SPEED*30, TPS_CAMERA_RADIUS);
			}
			break;
		case 'c':
			if (camera_mode==THIRD){
				dynamic_camera.setRotationTPS_OY(-MOUSE_SPEED*30, TPS_CAMERA_RADIUS);
			}
			break;
		case 'f':
			// Apasam butonul "lanternelor" ce pornesc dinspre nava
			flashlight_on = !flashlight_on;
			break;
		case 'r':
			dynamic_camera.translate_Up(MOVING_SPEED);
			player->updatePosition();
			break;
		case 'v': 
			dynamic_camera.translate_Up(-MOVING_SPEED);
			player->updatePosition();
			break;
		case '1':
			can_pick_camera = false;
			camera_mode = COCKPIT;
			dynamic_camera.setFPSCamera();
			player->updatePosition();
			break;
		case '2':
			can_pick_camera = false;
			camera_mode = THIRD;
			dynamic_camera.setTPSCamera();
			dynamic_camera.setSimpleRotationTPS_OY(0.0f);
			TPS_CAMERA_RADIUS = 10.0f;
			player->updatePosition();
			break;
		case '3':
			can_pick_camera = true;
			break;
		case '4':
			can_pick_camera = false;
			dynamic_camera.setTPSCamera();
			dynamic_camera.resetTPSCamera();
			dynamic_camera.setSimpleRotationTPS_OY(M_PI/2);
			TPS_CAMERA_RADIUS = 30.0f;
			player->updatePosition();
			camera_mode = DYNAMIC;
			break;
		default:
			break;
	}

}

void Game::mouseKey(int button, int state, int x, int y){
	if (button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		pickObject(x,y);
	}
}


void Game::idle(){
	// Stergem obiectele ce nu mai prezinta interes
	std::vector<Object*>::iterator it;
	for (it=objects.begin(); it!=objects.end(); ){
		if ((*it)->canDelete()){
			delete *it;
			it = objects.erase(it);
		}
		else {
			it++;
		}
	}
	// Verificam coliziunea navei cu asteroizii
	if (a_storm->isColliding(player->getPosition(), player->getScale())){
		// Daca se ciocneste cu un asteroid atunci scadem din viata
		player->doDamage();
	}
	glutPostRedisplay();	
}

int Game::getCameraMode(){
	return camera_mode;
}

void Game::printText(float x, float y, char *text){
	if (!text || !strlen(text)) return;
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(x,y);
	while(*text){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *text);
		text++;
	}
}


void Game::setHudText(char *text){
	// Daca textul pe care dorim sa il afisam
	// este diferit de cel care este afisat in 
	// momentul de fata atunci il modificam
	if (strcmp(hud_text, text)!=0){
		sprintf(hud_text, text);
	}
}

void Game::resetHudText(){
	sprintf_s(hud_text, "");
}

// Verifica daca jocul s-a terminat (jucatorul a ajuns la portal).
bool Game::isgameOver(){
	return player->isDestroyed();
}
