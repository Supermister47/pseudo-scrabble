#include "diccTrie.h"


/*
 * Varias de las implementaciones de los algoritmos en diccTrie eran erróneas, por lo tanto fueron cambiadas.
 * */


diccTrie::diccTrie(): _raiz(nullptr), _tamPalabraMasLarga(0) {}

diccTrie::diccTrie(const diccTrie& aCopiar) : diccTrie() { *this = aCopiar; }


diccTrie& diccTrie::operator=(const diccTrie& d) {
    asignarRec(_raiz, d._raiz);
    _tamPalabraMasLarga = d._tamPalabraMasLarga;
    return *this;
}

//Funcion auxiliar para recursión
void diccTrie::asignarRec(Nodo*& nodo,Nodo* nodo2) {
    if(nodo2 != nullptr){
        if(nodo == nullptr){
            nodo = new Nodo();
        }
        if(nodo2->definicion != nullptr){
            nodo->definicion = new lPalabra(*nodo2->definicion);
        }
        for(int i = 0; i < nodo2->siguientes.size(); i++){
            asignarRec(nodo->siguientes[i], nodo2->siguientes[i]);
        }

    }
}


diccTrie::~diccTrie(){
    destruirNodos(_raiz);
    _raiz = nullptr;
    _tamPalabraMasLarga = 0;
}

//Funcion auxiliar para recursión
void diccTrie:: destruirNodos(Nodo* nodo){
    if(nodo != nullptr){
        for(Nodo*& n : nodo->siguientes){
            if(n != nullptr){
                destruirNodos(n);
            }
        }
        if(nodo->definicion != nullptr){
            delete nodo->definicion;
            nodo->definicion = nullptr;
        }
        delete nodo;
        nodo = nullptr;
    }

}



/////////////////////////////////

void diccTrie:: insertar(const lPalabra& palabra){

    if(_raiz == nullptr){
        _raiz = new Nodo();
    }

    Nodo** actual = &_raiz;
    for(const Letra& l : palabra) {

        Nodo** prox = &(*actual)->siguientes[ord(l)];

        if(*prox == nullptr){
            *prox = new Nodo();
        }

        actual = &(*prox);


    }

    delete (*actual)->definicion;
    (*actual)->definicion = new lPalabra(palabra);



    if(palabra.size() > _tamPalabraMasLarga){
        _tamPalabraMasLarga = palabra.size();
    }
}



bool diccTrie::esta(const lPalabra& palabra) const{
    bool encontrado = false;

    lPalabra::const_iterator it = palabra.begin();
    Nodo* actual = _raiz;

    while(actual != nullptr && !encontrado) {
        if(it == palabra.cend()){
            if(actual->definicion != nullptr) {
                encontrado = true;
            } else {
                actual = nullptr;
            }
        } else {
            Letra letraActual = *it;
            actual = actual->siguientes[ord(letraActual)];
            ++it;
        }
    }
    return encontrado;
}

Nat diccTrie::tamPalabraMasLarga() const{
    return _tamPalabraMasLarga;
}


