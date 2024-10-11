#ifndef REVOLUCION_H
#define REVOLUCION_H
#include "malla.h"
/////////////// CLASE REVOLUCION //////////////
class Revolucion: public Malla { 
public: 
  float ncopias = 4, vueltas = 1;

Revolucion(int n, const char *nombre, int v = 1);

void generarRev2();

void añadirTapas();

}; 

#endif
