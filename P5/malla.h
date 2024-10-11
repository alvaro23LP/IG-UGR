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
    
  
    GLfloat material_diffuse[4] = { 0.0f, 0.0f, 0.0f, 1.0f };;  //Md
    GLfloat material_specular[4]= { 0.0, 0.0, 0.0, 1.0 };       //Ms
    GLfloat material_shininess = 0.0 ;                    //e
    float  color[4] = { 0.0, 0.0, 0.0, 1 };
    

  Malla();

  Malla(const char *nombre);
 
  void leerMallaVector(vector<float> v, vector<int> c);

  void leerMallaPLY(const char *nombre);

  void calcularNormales();

  void calcularNormalesVertices();

  void draw();
  
  void drawSMOOTH();

  void setShininess(GLfloat v);
  void setAmbient(GLfloat v[4]);
  void setSpecular(GLfloat v[4]);
  void setDiffuse(GLfloat v[4]);
};

#endif