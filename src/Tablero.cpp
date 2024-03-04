//
// Created by Ferri on 17/11/2022.
//

#include "Tablero.h"

char Tablero::NULL_CHAR = '#';

Tablero::Tablero(Nat N) {
    _tab = vector<vector<Letra>>(N);
    for (vector<Letra> &f: _tab) {
        f = vector<Letra>(N, NULL_CHAR);
    }
}

Nat Tablero::Tam() const {
    return _tab.size();
}

bool Tablero::hayLetra(Nat i, Nat j) const {
    return _tab[i][j] != NULL_CHAR;
}

bool Tablero::enTablero(Nat i, Nat j) const {
    return (0 <= i && i < Tam()) && (0 <= j && j < Tam());
}

Letra Tablero::letraEn(Nat i, Nat j) const {
    return _tab[i][j];
}

void Tablero::ponerLetra(Nat i, Nat j, Letra l) {
    _tab[i][j] = l;
}

ostream &operator<<(ostream &os, Tablero &t) {

    for (int i=0; i < t.Tam(); i++) {
        // Imprimo la línea que divide las filas
        os << '+';
        for (int j=0; j < t.Tam(); j++) {
            os << "---+";
        }
        os << endl;

        // Imprimo las letras que están en el tablero
        os << '|';
        for (int j=0; j < t.Tam(); j++) {
            os << ' ';
            if (t.hayLetra(i, j))
                os << t.letraEn(i, j);

            else
                os << ' ';
            os << ' ' << '|';
        }
        os << endl;
    }

    // Imprimo la última línea del tablero
    os << '+';
    for (int j=0; j < t.Tam(); j++) {
        os << "---+";
    }
    os << endl;
}
