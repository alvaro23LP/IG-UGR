#ifndef OBJETO3D_H
#define OBJETO3D_H

#include <math.h>
#include "auxiliares.h"
#include "lector-jpg.h"
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
using namespace std;


class Objeto3D 
{ 
public: 
	GLuint texId = 0;
	unsigned int ancho, alto;
	unsigned char* textura;

	virtual void draw( ) = 0; // Dibuja el objeto

	void asignarTex(const char *nombre);
} ; 

#endif  // OBJETO3D_H