/*	Prácticas de Informática Gráfica

	Grupo C					Curso 2022-23
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: 

	Programa principal
=======================================================
	G. Arroyo, J.C. Torres 
	Dpto. Lenguajes y Sistemas Informticos
	(Univ. de Granada)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details 
 http://www.gnu.org/copyleft/gpl.html

=======================================================/
modulo visual.c
    Inicialización de ventana X
    Transformación de visualización
    Transformación de proyección
 
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"




//================================================ VARIABLES  

/**
Angulos de rotacion de la camara.
**/
float view_rotx = 0, view_roty = 0;

/**
Distancia de la cámara
**/
float D = 10;

/**
Tamaño de la ventana X
**/
float anchoVentana, altoVentana;


float camaraIniX=0,camaraIniY=0,camarIniZ=-30;  //posicion inicial camra
float x_c=0,y_c=0,z_c=0;                        //cambio con respecto al origen
float dirX=0, dirY=0, dirZ=-1;                  //direccion de la camara

double redondearA6Decimales(double numero) {
    const double factor = 1000000.0;  
    return round(numero * factor) / factor;
}

/** 	void setCamara()
Cambia los parámetros de la cámara en el modulo visual
**/
void setCamara (float ax, float ay, float d, float x, float z, bool &pos) {
  view_rotx = -ax;
  view_roty = -ay;

  calcularDireccion(dirX, dirY, dirZ);
  if(pos) actualizarPosicionCamara(x,z);
}

void calcularDireccion(float &dirX, float &dirY, float &dirZ) {
  // Angulos de rotación de la cámara
  double rotx = (360+view_rotx) * M_PI / 180.0;
  double roty = (360+view_roty) * M_PI / 180.0;
  // Dirección
  dirX = redondearA6Decimales(cos(rotx) * sin(roty));
  dirY = redondearA6Decimales(sin(-rotx));
  dirZ = -redondearA6Decimales(cos(roty) * cos(rotx));
}

void actualizarPosicionCamara(float x, float z) {
    int a1=0,a2=0,a3=0;
    // Actualizar la posición de la cámara en función del vector de dirección
    a1 = x * dirX + z * -dirZ;
    a2 = x * dirY;
    a3 = x * dirZ + z * dirX;
    x_c += a1; y_c += a2; z_c += a3;
}

/** 	void transformacionVisualizacion()
Fija la transformacion de visualizacion en funcion de los angulos de rotacion view_rotx,
view_roty y el desplazamiento de la camara D.
La cámara mira al origen de coordenadas a una distancia D desde la posición angular view_rotx,
view_roty;
**/
void transformacionVisualizacion () {
  //glTranslatef (0, 0, -D);
  glRotatef (view_rotx, 1.0, 0.0, 0.0);
  glRotatef (view_roty, 0.0, 1.0, 0.0);
  glTranslatef(camaraIniX-x_c, camaraIniY-y_c, camarIniZ-z_c);
}

/**	void fijaProyeccion()
Fija la transformacion de proyeccion en funcion del tamaño de la ventana y del tipo de proyeccion
**/
void fijaProyeccion ()
{
  float calto;			// altura de la ventana corregida

  if (anchoVentana > 0)
    calto = altoVentana / anchoVentana;
  else
    calto = 1;

  glFrustum (-1, 1, -calto, calto, 1.5, 1500);

  glMatrixMode (GL_MODELVIEW);
// A partir de este momento las transformaciones son de modelado.       
  glLoadIdentity ();

}


/**	void inicializaVentana(GLsizei ancho,GLsizei alto)
Inicializa el viewport para que ocupe toda la ventana X, y llama a fijaProyeccion.
**/
void inicializaVentana (GLsizei ancho, GLsizei alto)
{
  altoVentana = alto;
  anchoVentana = ancho;

  glViewport (0, 0, ancho, alto);	// Establecemos el Viewport usando la nueva anchura y altura de la ventana X

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  fijaProyeccion ();		// Cargamos la transformacion de proyeccion

}
