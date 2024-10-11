#ifndef AUX_H
#define AUX_H

// Estructura para representar un vector tridimensional
struct Vector3D;

//Calcular un vector dados sus puntos
Vector3D calcularVector(float x1, float y1, float z1, float x2, float y2, float z2);

// Función para calcular Módulo Unidad
double moduloUnidad(Vector3D v);

// Función para normalizar
Vector3D normalizar(float x, float y, float z);

// Función para calcular el producto vectorial de dos vectores 
//  tridimensionales y de módulo unidad
Vector3D productoVectorial(float x1, float y1, float z1, float x2, float y2, float z2);

#endif