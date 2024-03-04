#ifndef TIPOS_H
#define TIPOS_H

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <set>
#include <tuple>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <queue>

using namespace std;

using Nat = unsigned int;

using Letra = char;

using Repositorio = list<Letra>;

using IdCliente = Nat;

using Ocurrencia = set<tuple<Nat, Nat, Letra>>;

using Palabra = vector<Letra>;

using lPalabra = list<Letra>;



/**
    * Hace la conversion del tipo de dato "Palabra" (vector<Letra>)
    * al tipo lPalabra (list<Letra>)
    */
    lPalabra convALista(const Palabra& palabra);

    /** Función auxiliar hecha para poder ordenar la ocurrencia (que ya no sería un conjunto)
     */
    vector<tuple<Nat, Nat, Letra>> set2Vector(const Ocurrencia &occ);


#endif // TIPOS_H

