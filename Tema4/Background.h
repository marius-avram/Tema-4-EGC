#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_
#pragma once 
#include <vector> 

class Background{
	public: 
		Background(); 
		virtual ~Background();
		void createStars(float xmin, float xmax, 
						 float zmin, float zmax);
		void renderPlane(int start_index);
		void render();

	private:
		float xmin, xmax;
		float ymin, ymax;
		float zmin, zmax;
		std::vector<float> xpoints;
		std::vector<float> ypoints;
		std::vector<float> zpoints;
		std::vector<unsigned int> indexes;
};

#endif