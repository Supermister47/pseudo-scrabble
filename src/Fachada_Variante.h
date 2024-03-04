#ifndef FACHADA_VARIANTE_H
#define FACHADA_VARIANTE_H

#include "Tipos.h"
#include "Letra.h"
#include "Variante.h"



class Fachada_Variante {
public:
    /**
     * Construye una Fachada_Variante a partir de los parametros de las variantes
     */
    Fachada_Variante(
            Nat tamanoTab,
            Nat cantFichas,
            const map<Letra, Nat>& puntajes,
            const set<vector<Letra>>& palabrasLegitimas
    );

    //Constructor por copia
    Fachada_Variante(const Fachada_Variante& fvCopiar);



    ~Fachada_Variante();

    /**
     * Retorna el tamano del tablero de la variante
     */
    Nat tamanoTablero() const;

    /**
     * Retorna la cantidad de fichas que cada jugador deberia tener en su mano.
     */
    Nat fichas() const;

    /**
     * Retorna el puntaje de la letra l
     */
    Nat puntajeLetra(Letra l) const;

    /**
     * Indica si una palabra es legitima o no
     */
    bool palabraLegitima(const Palabra& palabra) const;

    Nat tamPalabraMasLarga() const;

    Variante devolver_variante() const;

private:
    Variante var;


/**
     * Hace la conversion del tipo de dato "Palabra" (vector<Letra>)
     * al tipo lPalabra (list<Letra>)
     */



};

#endif //FACHADA_VARIANTE_H
