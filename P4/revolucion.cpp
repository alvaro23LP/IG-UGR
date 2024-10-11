#include <cmath>
#include "revolucion.h"
#include "malla.h"


using namespace std;

  // Constructor 
  Revolucion::Revolucion(int n, const char *nombre, int v){   
    leerMallaPLY(nombre); 
    if(n > 4) ncopias = n;                            
    distanciaVertice();            
    aniadirCoordTex();
    generarRev2();
    calcularNormales();
  }

  // Función para generar vertices con revolucion
  void Revolucion::generarRev2(){
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

  // Función para añadir tapas a las figuras (P2, en desuso)
  void Revolucion::añadirTapas(){
    vertices.insert(vertices.begin(), 0.0); //z
    vertices.insert(vertices.begin(), vertices[2]); //y
    vertices.insert(vertices.begin(), 0.0); //x
    float aux = *(vertices.end()-2);
    vertices.push_back(0); //x
    vertices.push_back(aux); //y
    vertices.push_back(0); //z
  }

 /////// Guardar las coordenadas de textura
 void Revolucion::aniadirCoordTex(){
    float u=0, v=0;
    for(int i=0 ; i<ncopias ; i++){
      u = ((2*M_PI*i)/(ncopias-1))/(2*M_PI);
      for ( int j=0 ; j<distancias.size() ; j++){
        coordTex.push_back(u);
        v = distancias[j];
        coordTex.push_back(1-v);
      }           
    }
  }
  
/////// Calcular la di de cada vertice
void Revolucion::distanciaVertice(){
  vector<float> otro;
  for(int i=0 ; i<vertices.size() ; i=i+3){
    distancias.push_back(calcularDistancia(vertices[0], vertices[1], vertices[2], vertices[i],vertices[i+1],vertices[i+2]));
  }
  for(int i=0 ; i<distancias.size() ; i++){ otro.push_back(distancias[i]/ *(distancias.end()-1)); }
  distancias = otro;
}

////// Override de lass funciones de dibujado para poner coordenadas de textura
void Revolucion::draw(){
    glMaterialfv( GL_FRONT, GL_AMBIENT, color ) ;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);

    glShadeModel(GL_FLAT);
    glBegin (GL_TRIANGLES);
    for(int i=0 ; i<indices.size() ; i=i+3){
      glNormal3d(normales[i],normales[i+1],normales[i+2]);
      glTexCoord2f(coordTex[2*indices[i]], coordTex[2*indices[i]+1]);
      glVertex3d(vertices[3*indices[i]],vertices[3*indices[i]+1],vertices[3*indices[i]+2]);
      glTexCoord2f(coordTex[2*indices[i+1]], coordTex[2*indices[i+1]+1]);
      glVertex3d(vertices[3*indices[i+1]],vertices[3*indices[i+1]+1],vertices[3*indices[i+1]+2]);
      glTexCoord2f(coordTex[2*indices[i+2]], coordTex[2*indices[i+2]+1]);
      glVertex3d(vertices[3*indices[i+2]],vertices[3*indices[i+2]+1],vertices[3*indices[i+2]+2]);
    }
    glEnd();
  }

    void Revolucion::drawSMOOTH(){
    glMaterialfv( GL_FRONT, GL_AMBIENT, color ) ;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);
    glShadeModel(GL_SMOOTH);
    glBegin (GL_TRIANGLES);
    calcularNormalesVertices();
    for(int i=0 ; i<indices.size() ; i=i+3) {
      glNormal3d(normalesVer[3*indices[i]],normalesVer[3*indices[i]+1],normalesVer[3*indices[i]+2]);
      glTexCoord2f(coordTex[2*indices[i]], coordTex[2*indices[i]+1]);
      glVertex3d(vertices[3*indices[i]],vertices[3*indices[i]+1],vertices[3*indices[i]+2]);

      glNormal3d(normalesVer[3*indices[i+1]],normalesVer[3*indices[i+1]+1],normalesVer[3*indices[i+1]+2]);
      glTexCoord2f(coordTex[2*indices[i+1]], coordTex[2*indices[i+1]+1]);
      glVertex3d(vertices[3*indices[i+1]],vertices[3*indices[i+1]+1],vertices[3*indices[i+1]+2]);

      glNormal3d(normalesVer[3*indices[i+2]],normalesVer[3*indices[i+2]+1],normalesVer[3*indices[i+2]+2]);
      glTexCoord2f(coordTex[2*indices[i+2]], coordTex[2*indices[i+2]+1]);
      glVertex3d(vertices[3*indices[i+2]],vertices[3*indices[i+2]+1],vertices[3*indices[i+2]+2]);
    }
    glEnd();
  }