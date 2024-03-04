#include "Variante.h"




Variante::Variante(Nat tamanioTab, Nat cantidadFichas,const map<Letra, Nat>& puntajes, const set<vector<Letra>>& palabrasLegitimas):
            _tamTablero(tamanioTab), _cantFichas(cantidadFichas), _puntajes(convPuntajes(puntajes)), _palabrasLegitimas(convPalabrasLegitimas(palabrasLegitimas)){};

Variante::Variante(const Variante& vCopiar) {
    *this = vCopiar;
}


Variante& Variante::operator=(const Variante& v) {
    _tamTablero = v._tamTablero;
    _cantFichas = v._cantFichas;
    _puntajes = v._puntajes;
    _palabrasLegitimas = v._palabrasLegitimas;
}


Variante::~Variante() = default;


Nat Variante::vTamanoTablero() const {
    return _tamTablero;
}

Nat Variante::vFichas() const {
    return _cantFichas;
}

Nat Variante::vPuntajeLetra(Letra l) const {
    return _puntajes[ord(l)];
}

bool Variante::vPalabraLegitima(const lPalabra& palabra) const{
    return _palabrasLegitimas.esta(palabra);
}


Nat Variante::vTamPalabraMasLarga() const{
    return _palabrasLegitimas.tamPalabraMasLarga();
}



array<Nat, TAMANIO_ALFABETO> Variante:: convPuntajes(const map<Letra, Nat>& puntajes){
    array<Nat, TAMANIO_ALFABETO>  res{};
    for (int i = 0; i < res.size(); i++) {
        res[i] = 1;
    }
    for (pair<Letra, Nat> p : puntajes){
        res[ord(p.first)] = p.second;
    }
    return res;
}


diccTrie Variante::convPalabrasLegitimas(const set<vector<Letra>>& palabrasLegitimas){
    diccTrie diccionario;
    for (const vector<Letra>& palabra : palabrasLegitimas) {
        lPalabra p = convALista(palabra);
        diccionario.insertar(p);
    }
    return diccionario;
}


