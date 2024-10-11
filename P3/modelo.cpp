/*	Prácticas de Informática Gráfica

	Grupo C					Curso 2022-23
 	
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
bool ilum = true, suave = false;

//Función para cambiar modo
void setModo(const int &M){
  modo = M;
  glPolygonMode (GL_FRONT_AND_BACK, modo ) ; //Modo de visualización
  if(modo != GL_FILL) ilum = true;
}

void cambiarIlum(){
  if(modo == GL_FILL){
    if(ilum) ilum = false; else ilum = true;
  }
}

void cambiarSuavidad(){
  if(suave) suave = false; else suave = true;
}

/////////////////////////////////////////////////////////////////////////////

class Ejes:Objeto3D 
{ 
public: 
    float longitud = 70;
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
bool anima = true;

void cambiarAnimacion(){
  if(anima) anima = false; else anima = true;
}

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

//Clase Figura
class Figura{
  public: 
    Malla cilindro, ballIn, gear;
    bool animacion = false;
    float  color[4] = { 0.0, 0.8, 0.3, 1 }, color2[4] = { 0.2, 0.4, 0.7, 1 }, color3[4] = { 1, 0.0, 0.0, 1 },
      color4[4] = { 1, 0.6, 0.2, 1 }, color5[4] = { 0.6, 0.6, 0.7, 1 };

    Figura(bool smooth){
      cilindro.leerMallaPLY("plys/CylinderLiger.ply");
      cilindro.calcularNormales();
      ballIn.leerMallaPLY("plys/ballIN.ply");
      ballIn.calcularNormales();
      gear.leerMallaPLY("plys/gear.ply");
      gear.calcularNormales();
    }

    void updateRotation( bool animacion, GLfloat &anguloIni ) {
      if(animacion){
        anguloIni += rotacionVel * 0.16f; 
        if (anguloIni >= 360.0f) { anguloIni -= 360.0f; }
      }
    }

    float updatePosition( bool animacion ) {
        if(animacion){
          posIni += MovimientoVel * direccionMov * 0.016f; 
          if (posIni >= AltuMax) {
              posIni = AltuMax;
              direccionMov = -1.0f; // Cambiar la dirección a bajar
          } else if (posIni <= AltuMin) {
              posIni = AltuMin;
              direccionMov = 1.0f; // Cambiar la dirección a subir
          }
        }
        return posIni;
    }

    float updateScale( bool animacion ) {
        if(animacion){
          EscalaIni += EscalaVel * direccionEscala * 0.016f; 
          if (EscalaIni >= AnchoMinax) {
              EscalaIni = AnchoMinax;
              direccionEscala = -1.0f; // Cambiar la dirección a decrecer
          } else if (EscalaIni <= AnchoMin) {
              EscalaIni = AnchoMin;
              direccionEscala = 1.0f; // Cambiar la dirección a ensanchar
          }
        }
        return EscalaIni;
    }

    void contruirFigura(bool smooth, bool animacion ){ //Figura
      glPushMatrix ();
        // Esc1 / Cubo
        glScaled(updateScale(animacion), 1.0, 3-updateScale(animacion)); // Actualiza el escalado
        baseDraw();  
      glPopMatrix ();

      glPushMatrix ();
        // Tu1 / R1 / T1 / C1 / T2
        glTranslated(0.0, updatePosition(animacion), 0.0);
        updateRotation(animacion, anguloIni1);  // Actualiza el ángulo de rotación
        glRotatef(anguloIni1, 0.0, 1.0, 0.0);  // Aplica rotación al objeto en torno al eje Y
        glTranslated(0.0, 17.5, 0.0);
        cilidroDraw(smooth);
        glTranslated(0.0, 8, 0.0);

        glPushMatrix();
          // Rot1 / C2
          glRotated(90, 0, 0, 1); // Rotación de 90 grados
          cilidroDraw(smooth);
        glPopMatrix();

        glPushMatrix();
          // R2 / T3 / Rot2 / Sierra
          updateRotation(animacion, anguloIni2);  // Actualiza el ángulo de rotación
          glRotatef(anguloIni2, 1.0, 0.0, 0.0);  // Aplica rotación al objeto en torno al eje X
          glTranslated(8.0, 0.0, 0.0);
          glRotated(90, 0, 1, 0); // Rotación de 90 grados
          gearDraw(smooth);
        glPopMatrix();

        glPushMatrix();
          // R3 / T4 / BallIn 
          updateRotation(animacion, anguloIni3);  // Actualiza el ángulo de rotación
          glRotatef(anguloIni3, -1.0, 0.0, 0.0);  // Aplica rotación en torno al eje X, en sentido contrario
          glTranslated(-9.0, 0.0, 0.0);
          ballInDraw(smooth);
        glPopMatrix();
      glPopMatrix();
    }

// Las siguientes funciones son para dibujar las figuras correctamente, 
// ya sea por directivas de OpenGL o por que al leerlas del ply correspondiente 
// salen rotadas, pequeñas, etc
    void base(int lado){
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

    void baseDraw(){
      glPushMatrix();
        glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        glScalef(2.0f, 4.0f, 2.0f); 
        glEnable(GL_NORMALIZE);
        glTranslated(-2, 0, -2);
        base(4); 
      glPopMatrix();
    }

    void cilidroDraw(bool smooth){
      glPushMatrix();
        glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
        glRotated(90.0, 1.0, 0.0, 0.0);
        glScalef(80.0f, 80.0f, 300.0f);
        glEnable(GL_NORMALIZE);
        smooth ? cilindro.drawSMOOTH() : cilindro.draw();
      glPopMatrix();
    }

    void ballInDraw(bool smooth){
      glPushMatrix();
        glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color3);
        glScalef(0.12f, 0.12f, 0.12f);
        glEnable(GL_NORMALIZE);
        smooth ? ballIn.drawSMOOTH() : ballIn.draw();
      glPopMatrix();
    }

    void gearDraw(bool smooth){
      glPushMatrix();
        glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color5);
        glRotated(24.0, 0.0, 1.0, 0.0);
        glRotated(1.7, 1.0, 0.0, 0.0);
        glScalef(0.12f, 0.12f, 0.12f);
        glEnable(GL_NORMALIZE);
        smooth ? gear.drawSMOOTH() : gear.draw();
      glPopMatrix();
    }
};

// DECLARACIÓN DE EJES, MALLAS, OBJETOS DE REVOLUCION
Ejes ejesCoordenadas;
Figura a(false);

/**	void Dibuja( void )
Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.
**/
void Dibuja (void)
{
  static GLfloat  pos[4] = { 0.0, 8.0, 10.0, 0.0 };	// Posicion de la fuente de luz

  glPushMatrix ();		// Apila la transformacion geometrica actual
    glClearColor (0.1, 0.1, 0.1, 1.0);	// Fija el color de fondo 
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer
    transformacionVisualizacion ();	// Carga transformacion de visualizacion
    glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena
    ejesCoordenadas.draw();			// Dibuja los ejes  
    
    // Activa o desactiva la iluminación  
    ilum ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);    
    
    //DIBUJA MALLAS 
    anima ? a.contruirFigura(suave, anima) : a.contruirFigura(suave, anima);
    
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
