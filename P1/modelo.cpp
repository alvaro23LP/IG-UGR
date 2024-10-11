/*	Prácticas de Informática Gráfica

	Grupo C					Curso 2022-23
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: 

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
modulo modelo.c
    Representación del modelo
    Funciones de dibujo
    Función Idle

*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"


/**	void initModel()
Inicializa el modelo y de las variables globales
**/
void
initModel () {}

int modo = GL_FILL;
bool ilum = true;
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// P1 ///////////////////////////////////
//Clase Cubo
class Cubo:Objeto3D
//Construye una caja con un vertice en origen y otro en (x,y,z)
{
  public:
  float lado = 10;
  
  Cubo(float l){
    lado = l;
  }

  void draw( )
  {
    //glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
    glBegin (GL_QUAD_STRIP);
    {				/* Caras transversales */
      glNormal3f (0.0, 0.0, 1.0);	/*Vertical delantera */
      glVertex3f (lado, lado, lado);
      glVertex3f (0, lado, lado);
      glVertex3f (lado, 0, lado);
      glVertex3f (0, 0, lado);
      glNormal3f (0.0, -1.0, 0.0);	/*Inferior */
      glVertex3f (lado, 0, 0);
      glVertex3f (0, 0, 0);
      glNormal3f (0.0, 0.0, -1.0);	/* Vertical hacia atras */
      glVertex3f (lado, lado, 0);
      glVertex3f (0, lado, 0);
      glNormal3f (0.0, 1.0, 0.0);	/* Superior, horizontal */
      glVertex3f (lado, lado, lado);
      glVertex3f (0, lado, lado);
    }
    glEnd ();
    glBegin (GL_QUADS);
    {				/* Costados */
      glNormal3f (1.0, 0.0, 0.0);
      glVertex3f (lado, 0, 0);
      glVertex3f (lado, lado, 0);
      glVertex3f (lado, lado, lado);
      glVertex3f (lado, 0, lado);
      glNormal3f (-1.0, 0.0, 0.0);
      glVertex3f (0, 0, 0);
      glVertex3f (0, 0, lado);
      glVertex3f (0, lado, lado);
      glVertex3f (0, lado, 0);
    }
    glEnd ();

  }
};

// Estructura para representar un vector tridimensional
struct Vector3D {
    double x, y, z;
};

// Módulo Unidad
double moduloUnidad(Vector3D v) {
    double m;
    m = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return m;
}
// Función para calcular el producto vectorial de dos vectores tridimensionales
Vector3D productoVectorial(float x1, float y1, float z1, float x2, float y2, float z2) {
    Vector3D resultado;
    resultado.x = y1 * z2 - z1 * y2;
    resultado.y = z1 * x2 - x1 * z2;
    resultado.z = x1 * y2 - y1 * x2;
    double m = moduloUnidad(resultado);
    resultado.x /= m;
    resultado.y /= m;
    resultado.z /= m;
    return resultado;
}


//Clase Piramide
class Piramide:Objeto3D
//Construye una caja con un vertice en origen y otro en (x,y,z)
{
  public:
  float alto = 10, lado = 10; 

  Piramide(float l, float a){
    lado = l;
    alto = a;
  }

  void draw( )
  {
    glBegin (GL_TRIANGLE_FAN);
    {				/* Caras */
      Vector3D aux;
      aux = productoVectorial(lado/2,alto,lado/2, 0,0,lado);
      glNormal3f (-aux.x, -aux.y, -aux.z);	 
      glVertex3f (lado/2, alto, lado/2);
      glVertex3f (0, 0, 0);
      glVertex3f (0, 0, lado);
      
      aux = productoVectorial(lado,0,0, lado/2,-alto,lado/2);
      glNormal3f (-aux.x, -aux.y, -aux.z);
      glVertex3f (lado, 0, lado);

      aux = productoVectorial(0,0,lado, lado/2,-alto,-lado/2);
      glNormal3f (aux.x, aux.y, aux.z);
      glVertex3f (lado, 0, 0);

      aux = productoVectorial(lado,0,0, lado/2,alto,lado/2);
      glNormal3f (-aux.x, -aux.y, -aux.z);
      glVertex3f (0, 0, 0);
    }
    glEnd ();
    glBegin (GL_QUADS);
    {				/* Base */
      glNormal3f (0.0, -1.0, 0.0);
      glVertex3f (0, 0, 0);
      glVertex3f (lado, 0, 0);
      glVertex3f (lado, 0, lado);
      glVertex3f (0, 0, lado);
      
    }
    glEnd ();

  }
};

//Clase Piramide
class PrismaHex:Objeto3D
//Construye una caja con un vertice en origen y otro en (x,y,z)
{
  public:
  float alto = 10, lado = 10; 

  PrismaHex(float l, float a){
    lado = l;
    alto = a;
  }

  void draw( )
  {
    glBegin (GL_TRIANGLE_FAN);
    {				/* base inf */
      glNormal3f (0,-1.0,0);	
      glVertex3f (lado, 0, lado);
      glVertex3f (lado/2, 0, 0);
      glVertex3f (0, 0, lado);
      
      glNormal3f (0,-1.0,0);	
      glVertex3f (lado/2, 0, 2*lado);

      glNormal3f (0,-1,0);	
      glVertex3f (3*lado/2, 0, 2*lado);

      glNormal3f (0,-1,0);	
      glVertex3f (2*lado, 0, lado);
      
      glNormal3f (0,-1,0);	
      glVertex3f (3*lado/2, 0, 0);
      
      glNormal3f (0,-1,0);	
      glVertex3f (lado/2, 0, 0);
    }
    glEnd ();

    glBegin (GL_TRIANGLE_FAN);
    {				/* base sup */
      glNormal3f (0,1,0);	
      glVertex3f (lado, alto, lado);
      glVertex3f (lado/2, alto, 0);
      glVertex3f (0, alto, lado);
      
      glNormal3f (0, 1,0);	
      glVertex3f (lado/2, alto, 2*lado);

      glNormal3f (0, 1,0);	
      glVertex3f (3*lado/2, alto, 2*lado);

      glNormal3f (0, 1,0);	
      glVertex3f (2*lado, alto, lado);
      
      glNormal3f (0, 1,0);	
      glVertex3f (3*lado/2, alto, 0);
      
      glNormal3f (0, 1,0);	
      glVertex3f (lado/2, alto, 0);
    }
    glEnd ();

    glBegin (GL_QUAD_STRIP);
    {				/* caras */
      glNormal3f (-1, 0.0, -0.5);
      glVertex3f (lado/2, alto, 0);
      glVertex3f (lado/2, 0, 0);

      glVertex3f (0, alto, lado);
      glVertex3f (0, 0, lado);
      
      glNormal3f (-1, 0, 0.5);	
      glVertex3f (lado/2, alto, 2*lado);
      glVertex3f (lado/2, 0, 2*lado);

      glNormal3f (0, 0, 1);	
      glVertex3f (3*lado/2, alto, 2*lado);
      glVertex3f (3*lado/2, 0, 2*lado);

      glNormal3f (1, 0, 0.5);	
      glVertex3f (2*lado, alto, lado);
      glVertex3f (2*lado, 0, lado);
      
      glNormal3f (1, 0, -0.5);	
      glVertex3f (3*lado/2, alto, 0);
      glVertex3f (3*lado/2, 0, 0);
      
      glNormal3f (0, 0, -1);	
      glVertex3f (lado/2, alto, 0);
      glVertex3f (lado/2, 0, 0);

    }
    glEnd ();

  }
};

Cubo cubo(3);
Piramide piramide(3,5);
PrismaHex prismaHex(2,4);

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class Ejes:Objeto3D 
{ 
public: 
    float longitud = 30;
// Dibuja el objeto
void draw( )
{
  glDisable(GL_LIGHTING);
  glBegin (GL_LINES);
  
    glColor3f (0, 1, 0);
    glVertex3f (0, 0, 0);
    glVertex3f (0, longitud, 0);

    glColor3f (1, 0, 0);
    glVertex3f (0, 0, 0);
    glVertex3f (longitud, 0, 0);

    glColor3f (0, 0, 1);
    glVertex3f (0, 0, 0);
    glVertex3f (0, 0, longitud);
  
  glEnd();
  glEnable(GL_LIGHTING);

}
} ; 

Ejes ejesCoordenadas;


//Función para cambiar modo
void setModo(const int &M){
  modo = M;
  glPolygonMode (GL_FRONT_AND_BACK, modo ) ; //Modo de visualización
  if(modo != GL_FILL) ilum = true;
}

void cambiarIlum(){
  if(modo == GL_FILL){
    if(ilum) ilum = false;
    else ilum = true;
  }
}

/**	void Dibuja( void )
Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.
**/
void Dibuja (void)
{
  static GLfloat  pos[4] = { 5.0, 5.0, 10.0, 0.0 };	// Posicion de la fuente de luz
  float  color[4] = { 0.8, 0.0, 1, 1 };
  float  color2[4] = { 0.0, 1, 0.0, 1 };
  float  color3[4] = { 1, 0.0, 0.5, 1 };

  glPushMatrix ();		// Apila la transformacion geometrica actual
  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer
  transformacionVisualizacion ();	// Carga transformacion de visualizacion
  glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena
  
  ejesCoordenadas.draw();			// Dibuja los ejes  

  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  // Dibuja el modelo (A rellenar en prácticas 1,2 y 3)  
  ilum ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);    
  cubo.draw();

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,color2) ;
  glTranslatef(0,0,6);
  piramide.draw();

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,color3) ;
  glTranslatef(6,0,-6);
  prismaHex.draw();
  
  glPopMatrix ();		// Desapila la transformacion geometrica
  glutSwapBuffers ();		// Intercambia el buffer de dibujo y visualizacion
}


/**	void idle()
Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.
**/
void idle (int v)
{

  glutPostRedisplay ();		// Redibuja
  glutTimerFunc (30, idle, 0);	// Vuelve a activarse dentro de 30 ms
}
