#ifndef _MESH_H_
#define _MESH_H_

class Vector3D;
class Face{
	public: 
		Face(): nverts(0), verts(0) {};
		
		int nverts;
		Vector3D **verts;
		float normal[3];
};

class Mesh{
	public:
		Mesh(char *filename);
		bool readFromFile(char *filename);
		void render();

		int nverts;
		Vector3D *verts;
		int nfaces; 
		Face *faces;
		bool loaded;
};

#endif