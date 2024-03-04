//
// Created by Ferri on 17/11/2022.
//

#ifndef TP_JUEGODEPALABRAS_TABLERO_H
#define TP_JUEGODEPALABRAS_TABLERO_H

#include "Tipos.h"
#include <ostream>


class Tablero {
    public:

        //- Equivalente a NuevoTablero(in: N: nat)
        /**
         * Construye un tablero vacío de tamaño N*N.
         * @param N: Cantidad de celdas de una fila o columna
         */
        explicit Tablero(Nat N);

        //- Equivalente a Tamaño(in t: Tablero)
        /**
         * Devuelve el tamaño del tablero.
         */
        Nat Tam() const;

        /**
         *  Se fija si hay alguna letra colocada en la posición (i, j) del tablero.
         *
         *  @pre @code 0 &lt= i &lt Tamaño(this) && 0 &lt= j &lt Tamaño(this)  @endcode
         *  @param i: fila del tablero
         *  @param j: columna del tablero
         *
         */
         bool hayLetra(Nat i, Nat j) const;


        /**
        *  Se fija si la posición (i, j) está dentro del tablero.
        *
        *  @param i: fila del tablero
        *  @param j: columna del tablero
        *
        */
         bool enTablero(Nat i, Nat j) const;

         //- Equivalente a Letra(in t: tab, in i: nat, in j: nat)
        /**
        *  Devuelve la letra ubicada en la posición (i, j) del tablero.
        *
        *  @pre @code enTablero?(i, j) &&L hayLetra(i, j)  @endcode
        *  @param i: fila del tablero
        *  @param j: columna del tablero
        *
        */
        Letra letraEn(Nat i, Nat j) const;

        /**
        *  Ubica la letra en la posición <i>(i, j)</i> del tablero.
        *
        *  @pre @code enTablero?(i, j) &&L !hayLetra(i, j)  @endcode
        *  @param i: fila del tablero
        *  @param j: columna del tablero
        *
        */
        void ponerLetra(Nat i, Nat j, ::Letra l);

        friend ostream& operator<<(ostream& os, Tablero& l);


private:
        //- Usamos vector en vez de array porque podemos declarar su tamaño
        //- en tiempo de ejecución
        vector<vector<Letra>> _tab;
        static char NULL_CHAR;
};

#endif //TP_JUEGODEPALABRAS_TABLERO_H
