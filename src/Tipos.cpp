#include "Tipos.h"

lPalabra convALista(const Palabra& palabra) {
    lPalabra res;
    for (Letra l : palabra) {
        res.push_back(l);
    }
    return res;
}


vector<tuple<Nat, Nat, Letra>> set2Vector(const Ocurrencia &occ) {
    vector<tuple<Nat, Nat, Letra>> vectorOcc;
    for (const tuple<Nat, Nat, Letra> &ficha: occ) {
        vectorOcc.push_back(ficha);
    }
    return vectorOcc;
}