/*	Prácticas de Informática Gráfica

	Grupo C					Curso 2023-24
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: Alvaro Lopez Vergara

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
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"
#include <cmath>
#include "auxiliares.h"
#include "objeto3d.h"
#include "malla.h"
#include "revolucion.h"
using namespace std;

/**	void initModel()
Inicializa el modelo y de las variables globales
**/
void
initModel () {}

/////////////////////////////////////////////////////////////////////////////
////////////////////// Funciones y variables auxiliares /////////////////////

int modo = GL_FILL; 
bool anima = true,ilum = true, suave = false, luz1 = true, luz2 = false;

// Funcion para alternar el encendido de las luces
void AlternarLuz(int n){
  switch(n)
  {
    case 1:
      if(luz1) luz1 = false; else luz1 = true;
      break;
    case 2:
      if(luz2) luz2 = false; else luz2 = true;
      break;
  }
  
}

//Función para cambiar modo
void setModo(const int &M){
  modo = M;
  glPolygonMode (GL_FRONT_AND_BACK, modo ) ; //Modo de visualización
  if(modo != GL_FILL) ilum = true;
}

void cambiarIlum(){
  //if(modo == GL_FILL){
    if(ilum) ilum = false; else ilum = true;
  //}
}

void cambiarSuavidad(){ if(suave) suave = false; else suave = true; }
void cambiarAnimacion(){ if(anima) anima = false; else anima = true; }


// Posicion
GLfloat posIni = 0.0f; // Posición vertical inicial
GLfloat MovimientoVel = 5.01f;     // Velocidad de movimiento 
GLfloat AltuMax = 5.0f;      // Altura máxima
GLfloat AltuMin = -2.0f;     // Altura mínima 
GLfloat direccionMov = 1.0f;      // Dirección de movimiento (1 arriba, -1 abajo)
void PosVelMas(){ if(MovimientoVel<40.0f) MovimientoVel+=0.2f; }
void PosVelMenos(){ if(MovimientoVel>0.2f) MovimientoVel-=0.2f; }
void PosicionMas(){ if(posIni<AltuMax) posIni+=0.2f; }
void PosicionMenos(){ if(posIni>AltuMin) posIni-=0.2f; }

// Escala
GLfloat EscalaIni = 1.0f;   // Escala inicial
GLfloat EscalaVel = 0.2f;   // Velocidad de cambio de escala
GLfloat AnchoMinax = 2.0f;      // Ancho máximo
GLfloat AnchoMin = 1.0f;      // Ancho mínimo
GLfloat direccionEscala = 1.0f;  // Dirección de escala (1.0 para ensanchar, -1.0 para decrecer)
void EscalaVelMas(){ if(EscalaVel<10.0f) EscalaVel+=0.1f; }
void EscalaVelMenos(){ if(EscalaVel>0.1f) EscalaVel-=0.1f; }
void EscalaMas(){ EscalaIni += 0.1f; }
void EscalaMenos(){ EscalaIni -= 0.1f; }

// Rotacion
GLfloat anguloIni1 = 0.0f; // Ángulo inicial
GLfloat anguloIni2 = 0.0f; // Ángulo inicial
GLfloat anguloIni3 = 0.0f; // Ángulo inicial
GLfloat rotacionVel = 4.0f; // Velocidad de rotación 
void RotVelMas(){ if(rotacionVel < 50.0f) rotacionVel += 0.5f; }
void RotVelMenos(){ if(rotacionVel > 0.5f) rotacionVel -= 0.5f; }
void AnguloMas1(){ anguloIni1 += 2; if(anguloIni1>360) anguloIni1 -= 360; }
void AnguloMenos1(){ anguloIni1 -= 2; if(anguloIni1<0) anguloIni1 += 360; }
void AnguloMas2(){ anguloIni2 += 2; if(anguloIni2>360) anguloIni2 -= 360; }
void AnguloMenos2(){ anguloIni2 -= 2; if(anguloIni2<0) anguloIni2 += 360; }
void AnguloMas3(){ anguloIni3 += 2; if(anguloIni3>360) anguloIni3 -= 360; }
void AnguloMenos3(){ anguloIni3 -= 2; if(anguloIni3<0) anguloIni3 += 360; }


/////////////////////////////////////////////////////////////////////////////

class Ejes:Objeto3D 
{ 
public: 
    float longitud = 70;
// Dibuja el objeto
  void draw( ){
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class Cubo: public Objeto3D
//Construye una caja con un vertice en origen y otro en (x,y,z)
{
  public:
  float lado = 10;

  Cubo(float l){
    lado = l;
  }

  void draw( )
  {
    glBegin (GL_QUADS);
    {				/* Costados */
      glNormal3f (1.0, 0.0, 0.0);
      glTexCoord2f(0.75f, 0.75f); glVertex3f (lado, 0, 0);
      glTexCoord2f(0.75f, 0.5f); glVertex3f (lado, lado, 0);
      glTexCoord2f(0.5f, 0.5f); glVertex3f (lado, lado, lado);
      glTexCoord2f(0.5f, 0.75f); glVertex3f (lado, 0, lado);
      glNormal3f (-1.0, 0.0, 0.0);
      glTexCoord2f(0.0f, 0.75f); glVertex3f (0, 0, 0);
      glTexCoord2f(0.25f, 0.75f); glVertex3f (0, 0, lado);
      glTexCoord2f(0.25f, 0.5f); glVertex3f (0, lado, lado);
      glTexCoord2f(0.0f, 0.5f); glVertex3f (0, lado, 0);
      /* Caras transversales */
      glNormal3f (0.0, 0.0, 1.0);	/*Vertical delantera */
      glTexCoord2f(0.5f, 0.5f); glVertex3f (lado, lado, lado);
      glTexCoord2f(0.25f, 0.5f); glVertex3f (0, lado, lado);
      glTexCoord2f(0.25f, 0.75f); glVertex3f (0, 0, lado);
      glTexCoord2f(0.5f, 0.75f); glVertex3f (lado, 0, lado);
      glNormal3f (0.0, -1.0, 0.0);	/*Inferior */
      glTexCoord2f(0.75f, 1.0f); glVertex3f (0, 0, 0);
      glTexCoord2f(0.75f, 0.75f); glVertex3f (lado, 0, 0);
      glTexCoord2f(0.5f, 0.75f); glVertex3f (lado, 0, lado);
      glTexCoord2f(0.5f, 1.0f); glVertex3f (0, 0, lado);
      glNormal3f (0.0, 0.0, -1.0);	/* Vertical hacia atras */
      glTexCoord2f(0.75f, 0.5f); glVertex3f (lado, lado, 0);
      glTexCoord2f(0.75f, 0.75f); glVertex3f (lado, 0, 0);
      glTexCoord2f(1.0f, 0.75f); glVertex3f (0, 0, 0);
      glTexCoord2f(1.0f, 0.5f); glVertex3f (0, lado, 0);
      glNormal3f (0.0, 1.0, 0.0);	/* Superior, horizontal */
      glTexCoord2f(0.5f, 0.5f); glVertex3f (lado, lado, lado);
      glTexCoord2f(0.75f, 0.5f); glVertex3f (lado, lado, 0);
      glTexCoord2f(0.75f, 0.25f); glVertex3f (0, lado, 0);
      glTexCoord2f(0.5f, 0.25f); glVertex3f (0, lado, lado);
    }
    glEnd ();

  }
};



// DECLARACIÓN DE EJES, MALLAS, OBJETOS DE REVOLUCION
Ejes ejesCoordenadas; 
Cubo ejemplo(5), ejemplo2(3);
Malla malla1("plys/ballIN.ply"); 
Revolucion lata(30,"plys/lata-pcue.ply"), tapaSup(30,"plys/lata-psup.ply"), tapaInf(30,"plys/lata-pinf.ply");

/**	void Dibuja( void )
Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.
**/
void Dibuja (void)
{
  static GLfloat  pos[4] = {7.0, 7.0, 7.0, 0.0 }, posDiffuse[4] = {1, 0.5, 0.0, 0.0 };	
  float  Negro[4] = { 0.0, 0.0, 0.0, 1 }, Verde[4] = { 0.0, 1.0, 0.1, 1 }, Rojo[4] = { 1, 0.0, 0.0, 1 },
       Blanco[4] = { 1.0, 1.0, 1.0, 1 }, Gris[4] = { 0.4, 0.4, 0.4, 1 };
  
  glPushMatrix ();		// Apila la transformacion geometrica actual
    glClearColor (0.1, 0.1, 0.1, 1.0);	// Fija el color de fondo 
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer
    transformacionVisualizacion ();	// Carga transformacion de visualizacion
    glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena
    glLightfv (GL_LIGHT1, GL_DIFFUSE, posDiffuse);	

    ejesCoordenadas.draw();			// Dibuja los ejes  
    glEnable(GL_NORMALIZE);

    // Activa o desactiva la iluminación  
    ilum ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);    

    // Altera la luces
    luz1 ? glEnable(GL_LIGHT0) : glDisable(GL_LIGHT0);    
    luz2 ? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);    

    // DADO
    glPushMatrix();
    glTranslated(-2.5f,-2.5f,-2.5f);
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, Blanco);
      ejemplo.asignarTex("dado.jpg");
      ejemplo.draw();
    glPopMatrix();

    // OBJETO MATERIALES
    glPushMatrix();
      glTranslated(10.0f,0.0f,0.0f);
      glScalef(0.1f,0.1f,0.1f);
      malla1.setAmbient(Negro);
      malla1.setDiffuse(Negro);
      malla1.setSpecular(Blanco);
      malla1.setShininess(8); //0 - 128
      suave ? malla1.drawSMOOTH() : malla1.draw(); 
    glPopMatrix();

    glPushMatrix();
      glTranslated(20.0f,0.0f,0.0f);
      glScalef(0.1f,0.1f,0.1f);
      malla1.setAmbient(Blanco);
      malla1.setDiffuse(Gris);
      malla1.setSpecular(Verde);
      malla1.setShininess(5); //0 - 128
      suave ? malla1.drawSMOOTH() : malla1.draw(); 
    glPopMatrix();

    glPushMatrix();
      glTranslated(30.0f,0.0f,0.0f);
      glScalef(0.1f,0.1f,0.1f);
      malla1.setAmbient(Rojo);
      malla1.setDiffuse(Negro);
      malla1.setSpecular(Rojo);
      malla1.setShininess(10); //0 - 128
      suave ? malla1.drawSMOOTH() : malla1.draw(); 
    glPopMatrix();
 
    // LATA
    glPushMatrix();
      glTranslated(-8.0f,-5.0f,0.0f);
      glScalef(10.0f, 10.0f, 10.0f); 
      
      tapaSup.setDiffuse(Gris);
      tapaSup.setSpecular(Blanco);
      tapaSup.setShininess(4);
      tapaInf.asignarTex("tapasup.jpg");
      suave ? tapaSup.drawSMOOTH() : tapaSup.draw();

      lata.setDiffuse(Gris);
      lata.setAmbient(Blanco);
      lata.setSpecular(Blanco);
      lata.setShininess(5); 
      lata.asignarTex("eti1.jpg");
      suave ? lata.drawSMOOTH() : lata.draw();  

      tapaInf.setDiffuse(Gris);
      tapaInf.setSpecular(Blanco);
      tapaInf.setShininess(5);
      tapaInf.asignarTex("tapaInf.jpg");
      suave ? tapaInf.drawSMOOTH() : tapaInf.draw();
    glPopMatrix();


    glDisable(GL_TEXTURE_2D);
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
