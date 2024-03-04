//
// Created by Ferri on 22/11/2022.
//
#include "Fachada_Juego.h"

Fachada_Juego::Fachada_Juego(Nat k, const Fachada_Variante &v, const Repositorio &r): _fVar(v), _var(v.devolver_variante()), _repo(r), _juego(k, _var, _repo) {}


void Fachada_Juego::ubicar(const Ocurrencia &o) {
    _juego.ubicarFichas(_juego.turno(), o);
}

IdCliente Fachada_Juego::turno() {
    return _juego.turno();
}

const Fachada_Variante &Fachada_Juego::variante() {
    return _fVar;
}

bool Fachada_Juego::jugadaValida(const Ocurrencia &o) {
    return _juego.esJugadaValida(_juego.turno(), o);
}

bool Fachada_Juego::hayLetra(Nat x, Nat y) {
    //? Si bien estoy llamando a contenido en coordenada sin cumplir con el Pre, lo hago sabiendo que si las coordenadas
    //? están en el tablero la función devolverá el caracter '#' si no hay ninguna letra puesta en esa coordenada
    Letra l = _juego.contenidoEnCoordenada(make_tuple(x, y));
    if (l != '#') {
        return true;
    }
    else {
        return false;
    }
}

Letra Fachada_Juego::letra(Nat i, Nat j) {
    return _juego.contenidoEnCoordenada(make_tuple(i, j));
}

Nat Fachada_Juego::puntaje(IdCliente id) {
    return _juego.puntaje(id);
}

Nat Fachada_Juego::cantFicha(IdCliente id, Letra l) {
    return _juego.cantFichasDeLetra(id, l);
}

void Fachada_Juego::mostrar(ostream &os, bool mostrarPuntaje) {
    _juego.mostrar(os, mostrarPuntaje);
}



