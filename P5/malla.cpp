#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"
#include <cmath>
#include "malla.h"
using namespace std;


struct Vector3D{
    double x, y, z;
};


  Malla::Malla(){}

  Malla::Malla(const char *nombre){
    leerMallaPLY(nombre);
    calcularNormales();
  }

                                                                    //MATERIALES
  void Malla::setAmbient(GLfloat v[4]){    
    color[0] = v[0]; color[1] = v[1];
    color[2] = v[2]; color[3] = v[3]; 
  }
  void Malla::setSpecular(GLfloat v[4]){    
    material_specular[0] = v[0];  material_specular[1] = v[1];
    material_specular[2] = v[2];  material_specular[3] = v[3]; 
  }
  void Malla::setDiffuse(GLfloat v[4]){    
    material_diffuse[0] = v[0];  material_diffuse[1] = v[1];
    material_diffuse[2] = v[2];  material_diffuse[3] = v[3]; 
  }
  void Malla::setShininess(GLfloat v){ material_shininess = v; }

                                                                      //LEER PLY
  void Malla::leerMallaVector(vector<float> v, vector<int> c){
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

  void Malla::leerMallaPLY(const char *nombre){
    vector<float> vertices_ply;
    vector<int> caras_ply;
    ply::read(nombre,vertices_ply,caras_ply);
    leerMallaVector(vertices_ply,caras_ply);
  }

                                                                      //DIBUJO
  void Malla::calcularNormales(){          // Normales de las caras
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

  void Malla::calcularNormalesVertices(){  // Normales de los vertices
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

  void Malla::draw(){

    glMaterialfv( GL_FRONT, GL_AMBIENT, color ) ;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);

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
  
  void Malla::drawSMOOTH(){
    glMaterialfv( GL_FRONT, GL_AMBIENT, color ) ;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);
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