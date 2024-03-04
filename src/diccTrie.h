#ifndef DICCTRIE_H_
#define DICCTRIE_H_

#include <string>
#include <vector>
#include "Letra.h"


using namespace std;

/*
 * La implementación de diccTrie se realizó en base al Labo 9 de Tries.
 * Nos pareció que de esta manera era más práctico. Nuestra implementación en el tp1 era muy rebuscada.
 *
 * También decidimos unificar los dos modulos en uno solo.
 * */


class diccTrie {
public:

    /**
     *  Crea un nuevo diccionario de trie.
     */
    diccTrie();


    /**
      *  Crea un nuevo diccionario de trie por copia.
      *  @param aCopiar: diccionario de tries que se va a copiar.
      */
    diccTrie(const diccTrie& aCopiar);

    /**
      *  Se define un operador de asignacion para el diccionario.
      *  @param d: diccionario de tries que se a usar para la asignación.
      */
    diccTrie& operator=(const diccTrie& d);


    /**
      *  Destruye el diccionario de tries.
      */
    ~diccTrie();

    /**
     *  Inserta una palabra en el diccionario.
     *  @param palabra: palabra que se va a insertar en el diccionario.
     */
    void insertar(const lPalabra& p);

    /**
      *  Busca si una palabra esta definida en el diccionario.
      *  @param palabra: palabra que se va a buscar en el diccionario.
      */
    bool esta(const lPalabra& palabra) const;


    /**
      *  Devuelve el tamaño de la palabra más larga definida en el diccionario.
      */
    Nat tamPalabraMasLarga() const;




private:

    struct Nodo {
        vector<Nodo*> siguientes;
        lPalabra* definicion;
        Nodo(): siguientes(vector<Nodo*> (TAMANIO_ALFABETO, nullptr)), definicion(nullptr){};
    };

    Nodo* _raiz;
    Nat _tamPalabraMasLarga;

    void destruirNodos(Nodo* nodo);
    void asignarRec(Nodo*& nodo, Nodo* nodo2);

};



#endif // DICCTRIE_H_
