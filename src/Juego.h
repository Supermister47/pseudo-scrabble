
#ifndef TP_JUEGODEPALABRAS_JUEGO_H
#define TP_JUEGODEPALABRAS_JUEGO_H

#include "Tipos.h"
#include "Letra.h"
#include "Tablero.h"
#include "Variante.h"
#include <algorithm>

using namespace std;

class Juego {
    public:
        //- Se cambia el repositorio para que sea de tipo Repositorio en vez de cola(letra)
        //- Equivalente a IniciarJuego
        /**
         *  Inicia un nuevo juego.
         *  @param K: cantidad de jugadores deseada
         *  @param v: variante a jugar
         *  @param r: repositorio de letras con el que se jugará
         *
         */
        Juego(Nat K, Variante &v, Repositorio &r);

        //- Se cambia el orden de los parámetros
        /**
         *  Evalúa si el posicionamiento de una ocurrencia sobre el tablero forma todas palabras válidas,
         *  además de chequear que el posicionamiento de cada letra no exceda los límites del tablero
         *  ni ocupe lugares en los que ya hay una letra puesta.
         *
         *  @param j: número de jugador
         *  @param o: ocurrencia que se quiere posicionar
         *
         */
        bool esJugadaValida(IdCliente j, const Ocurrencia &occ) const;

        /**
         *  Ubica la ocurrencia <i>occ</i> sobre el tablero.
         *
         *  @pre @code esJugadaValida(j, occ)  @endcode
         *  @param j: número de jugador
         *  @param occ: ocurrencia que se quiere posicionar
         *
         */
        void ubicarFichas(IdCliente j, const Ocurrencia &occ);

        /**
         * Devuelve la variante con la que se está jugando.
         */
        Variante obtenerVariante() const;

        /**
         * Devuelve el número del jugador al que le toca jugar.
         */
        IdCliente turno() const;

        //- Se cambia el nombre del parámetro i por j
        /**
         * Devuelve el puntaje.
         * @pre @code j &lt #jugadores(this)  @endcode
         * @param j: número de jugador
         */
        Nat puntaje(IdCliente j);

        /**
         *  Devuelve la letra ubicada en la posición <i>(i, j)</i> del tablero.
         *
         *  @pre @code enTablero?(coord.first) && enTablero?(coord.second) &&L hayLetra?(this._tablero, coord.first, coord.second)  @endcode
         *  @param coord: coordenada válida de la forma (i, j)
         *
         */
        Letra contenidoEnCoordenada(tuple<Nat, Nat> coord) const;

         //- Se cambia el nombre del parámetro i por j
        /**
        * Devuelve la #fichas que tiene el jugador <i>j</i> de la letra <i>x</i>.
        * @pre @code j &lt #jugadores(this)  @endcode
        * @param j: número de jugador
        */
        Nat cantFichasDeLetra(IdCliente j, Letra x) const;

        // Función auxiliar hecha para debuggear los tests en algunos casos.
        void mostrar(ostream& os, bool mostrarPuntaje);

        //- Función agregada que no incluimos en la interfaz del TP1, pero es necesaria para Servidor.
        const Repositorio* repositorioDeLetras() const;


    private:

        enum Orientacion {Horizontal, Vertical, Ninguna};

        struct OcurrenciaColocada {
            Ocurrencia occ;
            Nat turnoTotalColocado;
            Orientacion orientacionPalabra;
            OcurrenciaColocada(const Ocurrencia &occ, Nat tTC, Orientacion orientacion): occ(occ), turnoTotalColocado(tTC), orientacionPalabra(orientacion) {};
        };

        //- El PuntajeTotal de cada jugador se crea en la función iniciarJugador del TP1
        struct PuntajeTotal {
            vector<OcurrenciaColocada> ultimasOcurrenciasColocadas;
            Nat puntajeAnterior;
            PuntajeTotal(Nat puntaje): ultimasOcurrenciasColocadas(), puntajeAnterior(puntaje) {};
        };

        //- El contructor de Jugador es equivalente a iniciarJugador
        // Su complejidad es O(TAMANIO_ALFABETO)
        struct Jugador {
            vector<Nat> repositorio;
            PuntajeTotal puntaje;
            Jugador(): repositorio(TAMANIO_ALFABETO), puntaje(0) {};
        };

        //- Agregado LMAX como una constante de Juego, inicializado en el constructor
        Nat LMAX{};

        Nat _ronda{};
        IdCliente _turno{};
        Variante* _variante{};
        Tablero _tablero;
        vector<vector<Nat>> _matrizTurnos;
        //- Agregado un iterador para repartir fichas a los jugadores, para así no modificar el repositorio, a diferencia
        //- de cómo se implementa en el TP1, en donde cada ficha repartida se quita de la referencia guardada del repositorio
        Repositorio::const_iterator _itRepositorioGeneral;
        //- Cambiado el nombre de repositorioLetras a repositorioGeneral
        const Repositorio* _repositorioGeneral{};
        vector<Jugador> _jugadores;


        // Funciones auxiliares //
        tuple<Nat, Nat, Letra> dameUno(const Ocurrencia &occ) const;
        void ordenar(vector<tuple<Nat, Nat, Letra>> &vectorOcc, Orientacion orientacion) const;

        // De Juego //
        void crearMatrizTurnos(Nat N);
        void darFichas(Nat i, Nat cantFichas);


        // De esJugadaValida //
        bool jugadorTieneLasFichas(Nat j, const Ocurrencia &occ) const;
        bool ocurrenciaEnTablero(const Ocurrencia &occ) const;
        bool todasCeldasVacias(const Ocurrencia &occ) const;
        bool seSolapanFichas(const Ocurrencia &occ) const;
        Orientacion alineacion(const Ocurrencia &occ) const;
        Orientacion invertirOrientacion(Orientacion orientacion) const;
        bool hayHuecosEnLaJugada(const vector<tuple<Nat, Nat, Letra>> &occOrd, Orientacion orientacion) const;
        bool palabraSuperaLmax(const vector<tuple<Nat, Nat, Letra>> &occOrd, Orientacion orientacion) const;
        lPalabra obtenerPalabra(const vector<tuple<Nat, Nat, Letra>> &occOrd, Orientacion orientacion) const;
        bool siguePalabra(const vector<tuple<Nat, Nat, Letra>> &occOrd, int i, int fila, int col) const;
        bool seUbicaAlgunaLetra(const vector<tuple<Nat, Nat, Letra>> &occOrd, int i, Nat fila, Nat col) const;


        // De ubicarFichas //
        void pasarTurno();
        void sacarFichasDelRepositorioDelJugador(Nat j, const Ocurrencia &occ);
        void agregarTurnoEnMatriz(Nat turnoTotal, Nat fila, Nat col);


        // De puntaje //
        lPalabra obtenerPalabraDeJugada(tuple<Nat, Nat, Letra> ficha, Nat turnoTotal, Orientacion orientacion) const;
        Nat obtenerPuntajePalabra(const lPalabra &palabra) const;
        bool sigueJugada(Nat turnoTotal, int fila, int col) const;

};

#endif //TP_JUEGODEPALABRAS_JUEGO_H
