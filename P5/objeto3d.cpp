#include <cmath>
#include "objeto3d.h"
using namespace std;

void Objeto3D::asignarTex(const char *nombre){
    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);    
    
    textura = LeerArchivoJPEG(nombre, ancho, alto); 
    // Genera un identificador único para la textura
    glGenTextures(1, &texId);
    // Asocia la textura con el identificador generado
    glBindTexture(GL_TEXTURE_2D, texId);

    // Establece parámetros de envoltura de textura en la dirección S (horizontal)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    // Establece parámetros de envoltura de textura en la dirección T (vertical)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // Establece el filtro de textura para las operaciones de reducción (minificación)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Establece el filtro de textura para las operaciones de agrandamiento (magnificación)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Carga los datos de la imagen en la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, textura);    
}