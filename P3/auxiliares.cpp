#include <cmath>
#include "auxiliares.h"
using namespace std;


///////////////////////// Funciones auxiliares ///////////////////////////

struct Vector3D{
    double x, y, z;
};

//Calcular un vector dados sus puntos
Vector3D calcularVector(float x1, float y1, float z1, float x2, float y2, float z2){
  Vector3D v;
  v.x = x2-x1; v.y = y2-y1; v.z = z2-z1;
  return v;
}

// Función para calcular Módulo Unidad
double moduloUnidad(Vector3D v) {
    double m;
    m = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return m;
}

// Función para normalizar
Vector3D normalizar(float x, float y, float z) {
    Vector3D resultado; resultado.x = x; resultado.y = y; resultado.z = z;
    double m = moduloUnidad(resultado);
    resultado.x /= m; resultado.y /= m; resultado.z /= m;
    return resultado;
}

// Función para calcular el producto vectorial de dos vectores 
//  tridimensionales y de módulo unidad
Vector3D productoVectorial(float x1, float y1, float z1, float x2, float y2, float z2) {
    Vector3D resultado;
    resultado.x = y1 * z2 - z1 * y2;
    resultado.y = z1 * x2 - x1 * z2;
    resultado.z = x1 * y2 - y1 * x2;
    return normalizar(resultado.x,resultado.y,resultado.z);
}