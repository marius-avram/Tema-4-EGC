#include "AsteroidStorm.h" 
#include "Constants.h" 
#include <cmath> 


// Numarul de asteroizi prezenti in joc
unsigned int AsteroidStorm::number = 120;

AsteroidStorm::AsteroidStorm(){
	init();
}

AsteroidStorm::~AsteroidStorm(){
}

void AsteroidStorm::init(){
	for(unsigned int i=0; i<number; i++){
		insertAsteroid(BEGINNING);
	}
}

void AsteroidStorm::render(){
	std::list<Asteroid*>::iterator it;
	unsigned int removed, i=1;
	// Verificam daca nu cumva unul din asteroizi a iesit 
	// din limitele cutiei virtuale sau daca nu a fost 
	// distrus de o raza laser
	for(it=asteroids.begin(); it!=asteroids.end();){
		if ((*it)->canDelete()){
			// Daca da atunci este sters
			delete *it;
			it = asteroids.erase(it);
		}
		else { 
			it++;
		}
	}

	removed = number - asteroids.size();
	for (unsigned int i=0; i<removed; ++i){
		insertAsteroid(RUNTIME);
	}

	// std::cout << "size " << asteroids.size() << std::endl;
	glInitNames();
	for(it=asteroids.begin(); it!=asteroids.end(); ++it){
		if (*it)
		glPushName(i);
		(*it)->render();
		glPopName();
		i++;
	}
}

void AsteroidStorm::insertAsteroid(GameState state){
	Asteroid *p_asteroid;
	std::list<Asteroid*>::iterator it;
	Vector3D pos1, pos2;
	float distance; 
	bool ok = false;

	while (!ok){
		p_asteroid = new Asteroid(state);
		// Verificam daca asteroidul nou creat este corect distantat 
		// fata de ceilalti asteroizi.
		ok = true;
		pos1 = p_asteroid->getPosition();
		for (it=asteroids.begin(); it!=asteroids.end(); ++it){
			pos2 = (*it)->getPosition();
			distance = sqrt(pow(pos1.x-pos2.x,2) + 
							pow(pos1.y-pos2.y,2) +
							pow(pos1.z - pos2.z, 2)); 
			if (distance < 1.8f * (p_asteroid->getScale() + (*it)->getScale())){
				ok = false;
				break;
			}
		}

		if (ok){
			// Daca nu exista coliziuni cu alti asteroizi este acceptat
			asteroids.push_back(p_asteroid);
		}
		else{
			// Altfel este sters si se incearca crearea altuia
			delete p_asteroid;
		}
	}
}

void AsteroidStorm::destroyAsteroid(int index){
	unsigned int i = 1;
	std::list<Asteroid*>::iterator it;
	for (it=asteroids.begin(); it!=asteroids.end(); ++it){
		if (i==index && !(*it)->isInDestruction()){
			(*it)->setColor(0.0f, 0.0f, 1.0f);
			(*it)->destroy();
		}
		i++;
	}
}

Asteroid* AsteroidStorm::getAsteroid(int index){
	unsigned int i = 1;
	std::list<Asteroid*>::iterator it; 
	for (it=asteroids.begin(); it!=asteroids.end(); ++it){
		if (i==index){
			return *it;
		}
		i++;
	}
}

bool AsteroidStorm::canDelete(){
	return false;
}


bool AsteroidStorm::isColliding(Vector3D position, float scale){
	std::list<Asteroid*>::iterator it;
	bool ok = false;
	// Se verifica daca se face coliziunea cu vreun asteroid 
	// din furtuna de asteroizi
	for (it=asteroids.begin(); it!=asteroids.end(); ++it){
		if ((*it)->isColliding(position, scale) && !(*it)->isInDestruction()){
			// atunci pur si simplu initiem etapa de distrugere 
			(*it)->destroy();
			ok = true;
		}
	}
	return ok;
}

bool cmpAsteroids(Asteroid* asteroid1, Asteroid* asteroid2){
	return (asteroid1->getPosition().x < asteroid2->getPosition().x);
}