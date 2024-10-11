#ifndef MALLA_H
#define MALLA_H

#include <math.h>
#include "auxiliares.h"
#include <cmath>
#include <vector>
#include "objeto3d.h"
using namespace std;


//////////////// CLASE MALLA ///////////////
class Malla: public Objeto3D {
  public:
    vector<float> vertices;
    vector<int> indices;
    vector<float> normales, normalesVer;

  Malla();

  Malla(const char *nombre);
 
  void leerMallaVector(vector<float> v, vector<int> c);

  void leerMallaPLY(const char *nombre);

  void calcularNormales();

  void calcularNormalesVertices();

  void draw();
  
  void drawSMOOTH();
};

#endif