#include <math.h>
#include "auxiliares.h"
#include <cmath>
#include <vector>
using namespace std;


class Objeto3D 
{ 
public: 

virtual void draw( ) = 0; // Dibuja el objeto
} ; 

/**
	Funcion de para cambiar de modo de visualización
**/
void setModo(const int &M);

/**
	Funcion de para cambiar iluminación
**/
void cambiarIlum();