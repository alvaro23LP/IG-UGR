#include <cmath>
#include "revolucion.h"
#include "malla.h"

using namespace std;

  // Constructor 
  Revolucion::Revolucion(int n, const char *nombre, int v){   //Si no se contruye muelle v será = a 1
    leerMallaPLY(nombre); 
    if(n > 4) ncopias = n;                            //No menos de 4 perfiles
    if(v == 1) añadirTapas();                         //Si no contruimos un muelle, añadimos las tapas
    vueltas = v;       
    generarRev2();
    calcularNormales();
  };
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
  //Función para añadir tapas a las figuras 
  void Revolucion::añadirTapas(){
    vertices.insert(vertices.begin(), 0.0); //z
    vertices.insert(vertices.begin(), vertices[2]); //y
    vertices.insert(vertices.begin(), 0.0); //x
    float aux = *(vertices.end()-2);
    vertices.push_back(0); //x
    vertices.push_back(aux); //y
    vertices.push_back(0); //z
  }

