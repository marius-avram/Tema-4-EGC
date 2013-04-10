#include "Mesh.h" 
#include "Vector3D.h" 
#include <stdio.h>
#include <assert.h>
#include <glut.h> 

Mesh::Mesh(char *filename) : nverts(0), verts(0), nfaces(0), faces(0) {
	loaded = readFromFile(filename);
};

bool Mesh::readFromFile(char *filename){
	int i;
	
	// Open file
	FILE *fp;
	if (!(fp = fopen(filename, "r"))) {
		fprintf(stderr, "Unable to open file %s\n", filename);
		return false;
	}	

	// Read file
	int nverts = 0;
	int nfaces = 0;
	int nedges = 0;
	int line_count = 0;
	char buffer[1024];
	while (fgets(buffer, 1023, fp)) {
    // Increment line counter
    line_count++;

    // Skip white space
    char *bufferp = buffer;
    while (isspace(*bufferp)) bufferp++;

    // Skip blank lines and comments
    if (*bufferp == '#') continue;
    if (*bufferp == '\0') continue;

    // Check section
    if (nverts == 0) {
      // Read header 
      if (!strstr(bufferp, "OFF")) {
        // Read mesh counts
        if ((sscanf(bufferp, "%d%d%d", &nverts, &nfaces, &nedges) != 3) || (nverts == 0)) {
          fprintf(stderr, "Syntax error reading header on line %d in file %s\n", line_count, filename);
          fclose(fp);
          return false;
        }

        // Allocate memory for mesh
        this->verts = new Vector3D[nverts];
        assert(this->verts);
        this->faces = new Face [nfaces];
        assert(this->faces);
      }
    }
    else if (this->nverts < nverts) {
      // Read vertex coordinates
      Vector3D& vert = this->verts[this->nverts++];
      if (sscanf(bufferp, "%f%f%f", &(vert.x), &(vert.y), &(vert.z)) != 3) {
        fprintf(stderr, "Syntax error with vertex coordinates on line %d in file %s\n", line_count, filename);
        fclose(fp);
        return false;
      }
    }

    else if (this->nfaces < nfaces) {
      // Get next face
      Face& face = this->faces[this->nfaces++];

      // Read number of vertices in face 
      bufferp = strtok(bufferp, " \t");
      if (bufferp) face.nverts = atoi(bufferp);
      else {
        fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
        fclose(fp);
        return false;
      }

      // Allocate memory for face vertices
      face.verts = new Vector3D *[face.nverts];
      assert(face.verts);

      // Read vertex indices for face
      for (i = 0; i < face.nverts; i++) {
        bufferp = strtok(NULL, " \t");
        if (bufferp) face.verts[i] = &(this->verts[atoi(bufferp)]);
        else {
          fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
          fclose(fp);
          return false;
        }
      }

      // Compute normal for face
      face.normal[0] = face.normal[1] = face.normal[2] = 0;
      Vector3D *v1 = face.verts[face.nverts-1];
      for (i = 0; i < face.nverts; i++) {
        Vector3D *v2 = face.verts[i];
        face.normal[0] += (v1->y - v2->y) * (v1->z + v2->z);
        face.normal[1] += (v1->z - v2->z) * (v1->x + v2->x);
        face.normal[2] += (v1->x - v2->x) * (v1->y + v2->y);
        v1 = v2;
      }

      // Normalize normal for face
      float squared_normal_length = 0.0;
      squared_normal_length += face.normal[0]*face.normal[0];
      squared_normal_length += face.normal[1]*face.normal[1];
      squared_normal_length += face.normal[2]*face.normal[2];
      float normal_length = sqrt(squared_normal_length);
      if (normal_length > 1.0E-6) {
        face.normal[0] /= normal_length;
        face.normal[1] /= normal_length;
        face.normal[2] /= normal_length;
      }
    }
    else {
      // Should never get here
      fprintf(stderr, "Found extra text starting at line %d in file %s\n", line_count, filename);
      break;
    }
  }

  // Check whether read all faces
  if (nfaces != this->nfaces) {
    fprintf(stderr, "Expected %d faces, but read only %d faces in file %s\n", nfaces, this->nfaces, filename);
  }

  // Close file
  fclose(fp);

  return true;
}

void Mesh::render(){
	if (loaded){
		// Set material
		static GLfloat material_ambient[] = { 0.75f, 0.16f, 0.16f, 1.0f };
		static GLfloat material_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);

		// Draw faces
		for (int i = 0; i < nfaces; i++) {
			Face& face = faces[i];
			glBegin(GL_POLYGON);
			glNormal3fv(face.normal);
			for (int j = 0; j < face.nverts; j++) {
			  Vector3D *vert = face.verts[j];
			  glVertex3f(vert->x, vert->y, vert->z);
			}
			glEnd();
		}
	}
	
}