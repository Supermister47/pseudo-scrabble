#ifndef TP_JUEGODEPALABRAS_VARIANTE_H
#define TP_JUEGODEPALABRAS_VARIANTE_H

#include "Tipos.h"
#include "Letra.h"
#include "diccTrie.h"

class Variante {
public:
    /**
     * Construye una variante a partir de los parametros de las variantes
     * @param tamanoTab: tamaño del tablero.
     * @param cantFichas: cantidad de fichas totales de la variante.
     * @param puntajes: diccionario con los puntajes de cada letra.
     * @param palabrasLegitimas: conjunto de las palabras legitimas.
     */

    Variante(Nat tamanoTab, Nat cantFichas, const map<Letra, Nat>& puntajes, const set<vector<Letra>>& palabrasLegitimas);

    /**
    * Construye una variante copiando los parametros de otra variante.
    * @param vCopiar: Variante a copiar.
    */
    Variante(const Variante& vCopiar);

    /**
     * Se define un operador de asignacion para variante.
     * @param v: Variante cuyos parametros seran utilizados para la asignacion.
     */
    Variante& operator=(const Variante& v);

    /**
    * Destruye la variante.
    */
    ~Variante();

    /**
     * Retorna el tamano del tablero de la variante
     */
    Nat vTamanoTablero() const;

    /**
     * Retorna la cantidad de fichas que cada jugador debería tener en su mano.
     */
    Nat vFichas() const;

    /**
     * Retorna el puntaje de la letra l
     * @param l: Letra cuyo puntaje se quiere conocer.
     **/

    Nat vPuntajeLetra(Letra l) const;

    /**
     * Indica si una palabra es legítima o no
     * @param palabra: palabra a chequear.
     */
    bool vPalabraLegitima(const lPalabra& palabra) const;

    Nat vTamPalabraMasLarga() const;

private:
    Nat _tamTablero;
    Nat _cantFichas;
    array<Nat, TAMANIO_ALFABETO> _puntajes;
    diccTrie _palabrasLegitimas;

    /**
     * Hace la conversion del tipo de dato de la variable "puntajes"
     * recibido en los parámetros del constructor, al tipo de dato de la variable
     * "_puntajes" que se encuentra en la parte privada de la clase.
     */
    array<Nat, TAMANIO_ALFABETO> convPuntajes(const map<Letra, Nat>& puntajes);

    /**
     * Hace la conversion del tipo de dato de la variable "palabrasLegitimas"
     * recibido en los parámetros del constructor, al tipo de dato de la variable
     * "_palabrasLegitimas" que se encuentra en la parte privada de la clase.
     */
    diccTrie convPalabrasLegitimas(const set<vector<Letra>>& palabrasLegitimas);




};

#endif //TP_JUEGODEPALABRAS_VARIANTE_H
