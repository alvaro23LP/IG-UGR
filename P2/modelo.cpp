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
#include <cmath>
using namespace std;

/**	void initModel()
Inicializa el modelo y de las variables globales
**/
void
initModel () {}

/////////////////////////////////////////////////////////////////////////////
////////////////////// Funciones y variables auxiliares /////////////////////

int modo = GL_FILL;
bool ilum = true;

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

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// P2 ///////////////////////////////////

// Estructura para representar un vector tridimensional
struct Vector3D {
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

//////////////// CLASE MALLA ///////////////
class Malla:Objeto3D {
  public:
    vector<float> vertices;
    vector<int> indices;
    vector<float> normales, normalesVer;

  Malla(){}

  Malla(const char *nombre){
    leerMallaPLY(nombre);
    calcularNormales();
  }
    
  void leerMallaVector(vector<float> v, vector<int> c){
    for (int i=0 ; i<v.size(); i=i+3){
      vertices.push_back(v[i]);
      vertices.push_back(v[i+1]);
      vertices.push_back(v[i+2]);
    }
    for (int i=0 ; i<c.size(); i=i+3){
      indices.push_back(c[i]);
      indices.push_back(c[i+1]);
      indices.push_back(c[i+2]);
    }
  }

  void leerMallaPLY(const char *nombre){
    vector<float> vertices_ply;
    vector<int> caras_ply;
    ply::read(nombre,vertices_ply,caras_ply);
    leerMallaVector(vertices_ply,caras_ply);
  }

  void calcularNormales(){          // Normales de las caras
    Vector3D v,v2,normal;
    for(int i=0 ; i<indices.size() ; i=i+3){
      v = calcularVector(vertices[3*indices[i]],vertices[3*indices[i]+1],vertices[3*indices[i]+2]
          ,vertices[3*indices[i+1]],vertices[3*indices[i+1]+1],vertices[3*indices[i+1]+2]);      
      v2 = calcularVector(vertices[3*indices[i]],vertices[3*indices[i]+1],vertices[3*indices[i]+2]
          ,vertices[3*indices[i+2]],vertices[3*indices[i+2]+1],vertices[3*indices[i+2]+2]);
            
      normal = productoVectorial(v.x, v.y, v.z, v2.x, v2.y, v2.z);
      normales.push_back(normal.x);
      normales.push_back(normal.y);
      normales.push_back(normal.z);
    }
  }

  void calcularNormalesVertices(){  // Normales de los vertices
    // Inicializo a 0
    for(int i=0 ; i<vertices.size()*3 ; i++){
      normalesVer.push_back(0);
    }
    // Para cada cara, sumo su normal a cada ver
    for(int i=0 ; i<indices.size() ; i=i+3){ 
      normalesVer[indices[i]*3] += normales[i];
      normalesVer[1+indices[i]*3] += normales[i+1];
      normalesVer[2+indices[i]*3] += normales[i+2];

      normalesVer[indices[i+1]*3] += normales[i];
      normalesVer[1+indices[i+1]*3] += normales[i+1];
      normalesVer[2+indices[i+1]*3] += normales[i+2];

      normalesVer[indices[i+2]*3] += normales[i];
      normalesVer[1+indices[i+2]*3] += normales[i+1];
      normalesVer[2+indices[i+2]*3] += normales[i+2];
    }
    // Normalizo la normal de cada vertice
    for(int i=0 ; i<normalesVer.size() ; i=i+3){
      Vector3D v = normalizar(normalesVer[i], normalesVer[i+1], normalesVer[i+2]);
      normalesVer[i] = v.x; normalesVer[i+1] = v.y; normalesVer[i+2] = v.z;
    }
  }

  void draw(){
    glShadeModel(GL_FLAT);
    glBegin (GL_TRIANGLES);
    for(int i=0 ; i<indices.size() ; i=i+3){
      glNormal3d(normales[i],normales[i+1],normales[i+2]);
      glVertex3d(vertices[3*indices[i]],vertices[3*indices[i]+1],vertices[3*indices[i]+2]);
      glVertex3d(vertices[3*indices[i+1]],vertices[3*indices[i+1]+1],vertices[3*indices[i+1]+2]);
      glVertex3d(vertices[3*indices[i+2]],vertices[3*indices[i+2]+1],vertices[3*indices[i+2]+2]);
    }
    glEnd();
  }
  
  void drawSMOOTH(){
    glShadeModel(GL_SMOOTH);
    glBegin (GL_TRIANGLES);
    calcularNormalesVertices();
    for(int i=0 ; i<indices.size() ; i=i+3){
      glNormal3d(normalesVer[3*indices[i]],normalesVer[3*indices[i]+1],normalesVer[3*indices[i]+2]);
      glVertex3d(vertices[3*indices[i]],vertices[3*indices[i]+1],vertices[3*indices[i]+2]);

      glNormal3d(normalesVer[3*indices[i+1]],normalesVer[3*indices[i+1]+1],normalesVer[3*indices[i+1]+2]);
      glVertex3d(vertices[3*indices[i+1]],vertices[3*indices[i+1]+1],vertices[3*indices[i+1]+2]);

      glNormal3d(normalesVer[3*indices[i+2]],normalesVer[3*indices[i+2]+1],normalesVer[3*indices[i+2]+2]);
      glVertex3d(vertices[3*indices[i+2]],vertices[3*indices[i+2]+1],vertices[3*indices[i+2]+2]);
    }
    glEnd();
  }
};

/////////////// CLASE REVOLUCION //////////////
class Revolucion: public Malla
{
  public:
  float ncopias = 4, vueltas = 1;
  
  // Constructor 
  Revolucion(int n, const char *nombre, int v = 1){   //Si no se contruye muelle v será = a 1
    leerMallaPLY(nombre); 
    if(n > 4) ncopias = n;                            //No menos de 4 perfiles
    if(v == 1) añadirTapas();                         //Si no contruimos un muelle, añadimos las tapas
    vueltas = v;       
    generarRev2();
    calcularNormales();
  };
  // Función para generar vertices con revolucion
  void generarRev2(){
    float p, s=vertices.size(), k=0;
    for(int i=1 ; i<(ncopias*vueltas) ; i++){
      for ( int j=0 ; j<s ; j=j+3){
        p = cos((2*M_PI*i)/(ncopias-1))*vertices[j];
        vertices.push_back(p);
        p = vertices[j+1]+k;
        vertices.push_back(p);
        p = -1*sin((2*M_PI*i)/(ncopias-1))*vertices[j];
        vertices.push_back(p);
      }
      if(vueltas != 1) k += 10/ncopias;               //Constante para aumentar la altura (valor y)
    }
    //Inicializo el vector de indices
    indices.clear();
    s /= 3;
    //Calculo los indices de cada cara
    for(int i=0 ; i<(ncopias-1)*vueltas ; i++){
      for ( int j=0 ; j<s-1 ; j++){
        k = i*s+j;
        indices.push_back(k);
        indices.push_back(k+s);
        indices.push_back(k+s+1);
        indices.push_back(k);
        indices.push_back(k+s+1);
        indices.push_back(k+1);
      }
    }
  }
  //Función para añadir tapas a las figuras 
  void añadirTapas(){
    vertices.insert(vertices.begin(), 0.0); //z
    vertices.insert(vertices.begin(), vertices[2]); //y
    vertices.insert(vertices.begin(), 0.0); //x
    float aux = *(vertices.end()-2);
    vertices.push_back(0); //x
    vertices.push_back(aux); //y
    vertices.push_back(0); //z
  }
};

/////////////////////////////////////////////////////////////////////////////
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

// DECLARACIÓN DE EJES, MALLAS, OBJETOS DE REVOLUCION
Ejes ejesCoordenadas;
Malla malla1("plys/beethoven.ply"), malla2("plys/big_dodge.ply");
Revolucion peon(50,"plys/perfil.ply"), reloj(20,"plys/arena.ply"), muelle(30,"plys/muelle.ply",4);


/**	void Dibuja( void )
Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.
**/
void Dibuja (void)
{
  static GLfloat  pos[4] = { 0.0, 5.0, 10.0, 0.0 };	// Posicion de la fuente de luz
  float  color[4] = { 0.8, 0.0, 1, 1 }, color2[4] = { 0.5, 1, 0.0, 1 }, color3[4] = { 1, 0.2, 0.2, 1 },
      color4[4] = { 1, 0.8, 0.2, 1 }, color5[4] = { 0.2, 0.2, 0.2, 1 };

  glPushMatrix ();		// Apila la transformacion geometrica actual
  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer
  transformacionVisualizacion ();	// Carga transformacion de visualizacion
  glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena
  ejesCoordenadas.draw();			// Dibuja los ejes  
  
  // Activa o desactiva la iluminación  
  //ilum = false;
  ilum ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);    
  
  //DIBUJA MALLAS 
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  malla1.drawSMOOTH();
  
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  glTranslatef(13,0,0);
  malla2.draw(); 

  glPushMatrix ();
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color4);
  glTranslatef(-22,0,0);
  glScalef(2.5f, 2.5f, 2.5f);
  peon.drawSMOOTH();
  glPopMatrix ();

  glPushMatrix ();
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color3);
  glTranslatef(13,-3.5,0);
  glScalef(0.7f, 0.7f, 0.7f);
  reloj.draw();
  glPopMatrix ();

  glPushMatrix ();
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color5);
  glTranslatef(-30,-10,0);
  glScalef(0.5f, 0.5f, 0.5f);
  muelle.drawSMOOTH();
  glPopMatrix (); 

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
