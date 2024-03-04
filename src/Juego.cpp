
#include "Juego.h"



// Funciones auxiliares de Juego() //

void Juego::crearMatrizTurnos(Nat N) {
    _matrizTurnos = vector<vector<Nat>>(N);
    for (vector<Nat> &f: _matrizTurnos) {
        f = vector<Nat>(N, 0);
    }
}



//- Esta función combina desencolarN, darFichas y reponerFichasAlJugador del TP1 para evitar hacer copias de las fichas
//- que se tienen que distribuir a cada jugador. Para ello, se elimina el parámetro de Repositorio, para así
//- usar directamente el iterador del Repositorio guardado como variable privada
// COMPLEJIDAD: O(cantFichas) * ( O(1) + O(1) + O(1) ) = O(cantFichas)
void Juego::darFichas(IdCliente i, Nat cantFichas) {

    for (int j=0; j < cantFichas; j++) {               // O(cantFichas)
        Letra letra = *_itRepositorioGeneral;          // O(1)
        _jugadores[i].repositorio[ord(letra)]++;    // O(1)
        ++_itRepositorioGeneral;                      // O(1)
    }
}



//- Aliasing: Se guarda una referencia no modificable del repositorio
//- y a la variante para cumplir con las complejidades.
Juego::Juego(Nat K, Variante &v, Repositorio &r): _tablero(v.vTamanoTablero()) {
    _turno = 0;
    //- Cambiado el valor inicial de ronda
    _ronda = 1;

    crearMatrizTurnos(v.vTamanoTablero());

    // Como la complejidad del constructor de Jugador es O(TAMANIO_ALFABETO), generar
    // un vector de jugadores de tamaño K tiene complejidad O(K * TAMANIO_ALFABETO)
    //- Por lo tanto, con esta línea de código se ahorra el primer ciclo for de la función de iniciarJuego
    _jugadores = vector<Jugador>(K);


    //- Se inicializa el repositorio general y su iterador para poder repartir fichas al inicio
    _repositorioGeneral = &r;
    _itRepositorioGeneral = _repositorioGeneral->begin();
    for (int i = 0; i < K; i++) {
        //- Como ahora darFichas es una función distinta cuya complejidad es O(cantFichas), la complejidad de esta
        //- operación pasa a ser O(v.vFichas) = O(K)
        darFichas(i, v.vFichas());
    }
    _variante = &v;
    LMAX = _variante->vTamPalabraMasLarga();
}




// Funciones auxiliares de esJugadaValida() //

//- Función cambiada debido a las correcciones hechas en el algoritmo del TP1
//- De esta manera se chequea que el jugador tenga la cantidad de fichas repetidas que desea ubicar
//- La complejidad pasa a ser O(Cardinal(occ) + TAMANIO_ALFABETO)
//- Considerando TAMANIO_ALFABETO como constante, la complejidad de algortimo queda igual a O(Cardinal(occ))
//- al igual que en el TP1
bool Juego::jugadorTieneLasFichas(IdCliente j, const Ocurrencia &occ) const {
    bool res = true;
    vector<Nat> repoAux = _jugadores[j].repositorio;    // O(TAMANIO_ALFABETO)
    for (const tuple<Nat, Nat, Letra> &ficha: occ) {
        Letra letra = get<2>(ficha);
        if (repoAux[ord(letra)] != 0) {
            repoAux[ord(letra)]--;
        }
        else {
            res = false;
        }
    }
    return res;
}



//- Algoritmo ligeramente cambiado para mayor claridad.
bool Juego::ocurrenciaEnTablero(const Ocurrencia &occ) const {
    bool res = true;
    for (const tuple<Nat, Nat, Letra> &ficha: occ) {
        Nat i = get<0>(ficha);
        Nat j = get<1>(ficha);
        res &= _tablero.enTablero(i, j);
    }
    return res;
}



//- Algoritmo ligeramente cambiado para mayor claridad.
bool Juego::todasCeldasVacias(const Ocurrencia &occ) const {
    bool res = true;
    for (const tuple<Nat, Nat, Letra> &ficha: occ) {
        Nat i = get<0>(ficha);
        Nat j = get<1>(ficha);
        //- Como no está definida la función Libre? de Tablero, se reemplaza su llamado por su definición
        res &= !(_tablero.hayLetra(i,j));
    }
    return res;
}



// Los conjuntos no pueden tener elementos repetidos, por lo tanto, no voy a poder tener una ficha en la ocurrencia
// que tenga la mismas coordenadas y la misma letra que otra ficha que esté en la ocurrencia.
// Aun así, puede darse que fichas con letras distintas tengan las mismas coordenadas, por lo que esos son los casos
// en los que se produciría un solapamiento.
// El algoritmo crea un conjunto que contiene únicamente las coordenadas de las fichas de la ocurrencia. Si no hay
// solapamiento en la ocurrencia, entonces el tamaño del conjunto de coordenadas y el tamaño de la ocurrencia deberían
// ser iguales (pues si hay dos fichas con las mismas coordenadas, entonces el algoritmo solo agregará las coordenadas
// de una de las fichas, lo que provoca que el tamaño del conjunto de coordenadas y el de la ocurrencia sean diferentes)
// COMPLEJIDAD: [*] O(log(Cardinal(conjuntoCoordenadas))) está incluido en O(log(Cardinal(occ))), pues conjuntoCoordenadas
// tendrá menos o la misma cantidad de elementos que occ.
// Por lo tanto, la complejidad quedaría O(Cardinal(occ)) * ( O(1) + O(1) + O(log(Cardinal(occ))) ) = O(Cardinal(occ) * log(Cardinal(occ))).
bool Juego::seSolapanFichas(const Ocurrencia &occ) const {
    set<tuple<Nat, Nat>> conjuntoCoordenadas;
    for (const tuple<Nat, Nat, Letra> &ficha: occ) {       // O(Cardinal(occ))
        Nat i = get<0>(ficha);                          // O(1)
        Nat j = get<1>(ficha);                          // O(1)
        conjuntoCoordenadas.insert(make_tuple(i, j));   // O(log(Cardinal(conjuntoCoordenadas))) [*]
    }

    return conjuntoCoordenadas.size() != occ.size();       // O(1)
}



// dameUno devuelve siempre el primer elemento del conjunto
tuple<Nat, Nat, Letra> Juego::dameUno(const Ocurrencia &occ) const {
    return *(occ.begin());
}



//- Esta función es equivalente a las funciones ordenarPorFila y ordenarPorColumna.
// COMPLEJIDAD: las comparaciones se hacen en O(1). La estructura condicional tiene una función de sort
// en ambas ramas, por lo que la complejidad depende de eso. La complejidad de sort es
// O(|vectorOcc| * log(|vectorOcc|), que está incluido en O(|vectorOcc|^2).
void Juego::ordenar(vector<tuple<Nat, Nat, Letra>> &vectorOcc, Orientacion orientacion) const {
    // Se definen las comparaciones para poder ordenar las tuplas, según si se quiere
    // ordenar por fila o por columna
    struct {
        bool operator()(tuple<Nat, Nat, Letra> f1, tuple<Nat, Nat, Letra> f2) const {
            return get<0>(f1) < get<0>(f2);
        }
    } comparacionFila;

    struct {
        bool operator()(tuple<Nat, Nat, Letra> f1, tuple<Nat, Nat, Letra> f2) const {
            return get<1>(f1) < get<1>(f2);
        }
    } comparacionColumna;

    if (orientacion == Orientacion::Horizontal) {
        sort(vectorOcc.begin(), vectorOcc.end(), comparacionFila);
    }
    else {  // orientacion == Orientacion::Vertical
        sort(vectorOcc.begin(), vectorOcc.end(), comparacionColumna);
    }

}



//- Esta función combina estáEnFila? y estáEnColumna?
// Si la ocurriencia no está alineada ni en fila ni en columna devuelve Orientacion::Ninguna.
// No puede ocurrir que la ocurrencia esté en fila y en columna a la vez, si así fuese la ocurrencia estaría solapada
// y se hubiese captado por la función seSolapanFichas(), por lo que no se llegaría a entrar a esta función.
// COMPLEJIDAD: Todas las operaciones fuera del ciclo for son O(1). El cuerpo del ciclo for también es O(1)
// siendo que el ciclo for hace Cardinal(occ) iteraciones en el peor (y mejor) caso, la complejidad de todo
// el algoritmo es O(Cardinal(occ)), al igual que los algoritmos de estáEnFila? y estáEnColumna? del TP1.
Juego::Orientacion Juego::alineacion(const Ocurrencia &occ) const {
    bool estaEnFila = true;
    bool estaEnColumna = true;
    const Nat fila = get<0>(dameUno(occ));
    const Nat col = get<1>(dameUno(occ));

    for (const tuple<Nat, Nat, Letra> &ficha: occ) {    // O(Cardinal(occ))
        estaEnFila &= get<0>(ficha) == fila;
        estaEnColumna &= get<1>(ficha) == col;
    }
    if (estaEnColumna) {
        return Orientacion::Vertical;
    }
    else if (estaEnFila) {
        return Orientacion::Horizontal;
    }
    else {
        return Orientacion::Ninguna;
    }
}

// COMPLEJIDAD: O(1)
Juego::Orientacion Juego::invertirOrientacion(Orientacion orientacion) const {
    if (orientacion == Orientacion::Horizontal)
        return Orientacion::Vertical;
    else    // orientacion == Orientacion::Vertical
        return Orientacion::Horizontal;
}



//- Función agregada, cuya funcionalidad estaba implícita en el algoritmo de palabraHorizontalSuperaLmax?
//- y palabraVerticalSuperaLmax?, pero no funcionaba como debería (además de que quedaba menos modularizado).
// COMPLEJIDAD: Ambas ramas condicionales hacen las mismas operaciones, por lo que la complejidad en ambas
// es la misma. Los ciclos for definen la complejidad. las operaciones del cuerpo de ambos son O(1). Quedando
// que la complejidad final de la función es O(colFinal - colInicial) o O(filaFinal - filaInicial). Cabe notar
// que estas diferencias no podrán ser mayores a Lmax, pues ese caso se contempla en las estructuras condicionales
// marcadas con [*], haciendo que la función retorne true (Puesto que si se llega a ejecutar esta función es que
// Cardinal(occOrd) <= Lmax y entonces, si la diferencia entre las posiciones de las fichas de los extremos de
// la ocurrencia es mayor a LMAX, entonces inevitablemente habrá agujeros entre ambas fichas).
bool Juego::hayHuecosEnLaJugada(const vector<tuple<Nat, Nat, Letra>> &occOrd, Orientacion orientacion) const {
    bool res = false;
    tuple<Nat, Nat, Letra> primeraFicha = occOrd[0];
    tuple<Nat, Nat, Letra> ultimaFicha = occOrd[occOrd.size()-1];


    if (orientacion == Orientacion::Horizontal) {
        Nat fila = get<0>(primeraFicha);
        Nat colInicial = get<1>(primeraFicha);
        Nat colFinal = get<1>(ultimaFicha);

        if (colFinal - colInicial > LMAX) {     // [*]
            return true;
        }

        Nat desplazamiento = colInicial;
        auto itOccOrd = occOrd.begin();
        while (desplazamiento < colFinal && !res) {                 // O(colFinal - colInicial)
            if (_tablero.hayLetra(fila, desplazamiento));
            else if (desplazamiento == get<1>(*itOccOrd)) {
                ++itOccOrd;
            }
            else {
                res = true;
            }
            desplazamiento++;
        }
    }
    else {      // orientacion == Orientacion::Vertical
        Nat col = get<1>(primeraFicha);
        Nat filaInicial = get<0>(primeraFicha);
        Nat filaFinal = get<0>(ultimaFicha);

        if (filaFinal - filaInicial > LMAX) {   // [*]
            return true;
        }

        Nat desplazamiento = filaInicial;
        auto itOccOrd = occOrd.begin();
        while (desplazamiento < filaFinal && !res) {
            if (_tablero.hayLetra(desplazamiento, col));
            else if (desplazamiento == get<0>(*itOccOrd)) {     // O(filaFinal - filaInicial)
                ++itOccOrd;
            }
            else {
                res = true;
            }
            desplazamiento++;
        }
    }
    return res;

}



//- Se combinan las funciones palabraHorizontalSuperaLmax? y palabraVerticalSuperaLmax?
//- Se cambia el orden de los parámetros y el tipo de dato de occOrd a vector.
//- Se cambia un poco la idea de la función. Como ahora se chequea con anterioridad si hay huecos en la
//- jugada, el tamaño inicial de la palabra será la diferencia de posiciones entre las fichas extremas de la ocurrencia
//- a partir de allí se va incrementando el tamaño de la palabra con las fichas que haya puestas en el tablero por
//- fuera de los extremos de la ocurrencia.
//- Se cambia la manera de ir obteniendo las fichas del tablero en los dos ciclos while, pues podían darse
//- casos en los que se agregaban caracteres nulos a la palabra.
// Si bien la función tiene estos agregados, la complejidad del algoritmo se mantiene igual a la del TP1.
bool Juego::palabraSuperaLmax(const vector<tuple<Nat, Nat, Letra>> &occOrd, Orientacion orientacion) const {
    bool puedoSeguirAUnLado;
    bool puedoSeguirAlOtroLado;

    tuple<Nat, Nat, Letra> primeraFicha = occOrd[0];
    tuple<Nat, Nat, Letra> ultimaFicha = occOrd[occOrd.size()-1];
    Nat tamPalabra;

    if (orientacion == Orientacion::Horizontal) {
        Nat fila = get<0>(primeraFicha);
        const Nat colInicial = get<1>(primeraFicha);
        const Nat colFinal = get<1>(ultimaFicha);
        tamPalabra = colFinal - colInicial + 1;

        int colActual = colInicial-1;
        puedoSeguirAUnLado = colActual >= 0 && tamPalabra <= LMAX && _tablero.hayLetra(fila, colActual);

        while (puedoSeguirAUnLado) {
            colActual--;
            tamPalabra++;
            puedoSeguirAUnLado = colActual >= 0 && tamPalabra <= LMAX && _tablero.hayLetra(fila, colActual);
        }

        colActual = colFinal+1;
        puedoSeguirAlOtroLado = colActual < _tablero.Tam() && tamPalabra <= LMAX && _tablero.hayLetra(fila, colActual);

        while (puedoSeguirAlOtroLado) {
            colActual++;
            tamPalabra++;
            puedoSeguirAlOtroLado = colActual < _tablero.Tam() && tamPalabra <= LMAX && _tablero.hayLetra(fila, colActual);
        }
    }
    else {      // orientacion == Orientacion::Vertical
        Nat col = get<1>(primeraFicha);
        const Nat filaInicial = get<0>(primeraFicha);
        const Nat filaFinal = get<0>(ultimaFicha);
        tamPalabra = filaFinal - filaInicial + 1;

        int filaActual = filaInicial-1;
        puedoSeguirAUnLado = filaActual >= 0 && tamPalabra <= LMAX && _tablero.hayLetra(filaActual, col);

        while (puedoSeguirAUnLado) {
            filaActual--;
            tamPalabra++;
            puedoSeguirAUnLado = filaActual >= 0 && tamPalabra <= LMAX && _tablero.hayLetra(filaActual, col);
        }

        filaActual = filaFinal+1;
        puedoSeguirAlOtroLado = filaActual < _tablero.Tam() && tamPalabra <= LMAX && _tablero.hayLetra(filaActual, col);

        while (puedoSeguirAlOtroLado) {
            filaActual++;
            tamPalabra++;
            puedoSeguirAlOtroLado = filaActual < _tablero.Tam() && tamPalabra <= LMAX && _tablero.hayLetra(filaActual, col);
        }
    }

    return tamPalabra > LMAX;
}



//- Se combinan las funciones obtenerOcurrenciaHorizontal y obtenerOcurrenciaVertical//- uso del parámetro de la orientación de la palabra que está ubicada en el tablero.
//- Se cambia la manera de ir obteniendo las fichas del tablero en los dos ciclos while, pues podían darse
//- casos en los que se agregaban caracteres nulos a la palabra.
// Si bien la función tiene estos agregados, la complejidad del algoritmo se mantiene igual a la del TP1.
lPalabra Juego::obtenerPalabra(const vector<tuple<Nat, Nat, Letra>> &occOrd, Orientacion orientacion) const {
    lPalabra res;
    tuple<Nat, Nat, Letra> ficha = occOrd[0];
    bool puedoSeguirAUnLado;
    bool puedoSeguirAlOtroLado;

    res.push_front(get<2>(ficha));
    int i = 1;  //- Reemplazaría el Desencolar, para así recorrer el vector de occOrd.

    int filaActual;
    int colActual;

    if (orientacion == Orientacion::Horizontal) {
        filaActual = get<0>(ficha);
        colActual = get<1>(ficha)-1;
    }
    else {  // orientacion == Orientacion::Vertical
        filaActual = get<0>(ficha)-1;
        colActual = get<1>(ficha);
    }

    puedoSeguirAUnLado = siguePalabra(occOrd, i, filaActual, colActual);

    while (puedoSeguirAUnLado) {
        res.push_front(_tablero.letraEn(filaActual, colActual));
        if (orientacion == Orientacion::Horizontal) {
            colActual--;
        }
        else {
            filaActual--;
        }
        puedoSeguirAUnLado = siguePalabra(occOrd, i, filaActual, colActual);
    }

    if (orientacion == Orientacion::Horizontal) {
        filaActual = get<0>(ficha);
        colActual = get<1>(ficha)+1;
    }
    else {  // orientacion == Orientacion::Vertical
        filaActual = get<0>(ficha)+1;
        colActual = get<1>(ficha);
    }

    puedoSeguirAlOtroLado = siguePalabra(occOrd, i, filaActual, colActual);


    while (puedoSeguirAlOtroLado) {
        if (seUbicaAlgunaLetra(occOrd, i, filaActual, colActual)) {
            res.push_back(get<2>(occOrd[i]));
            i++;
        }
        else {
            res.push_back(_tablero.letraEn(filaActual, colActual));
        }
        if (orientacion == Orientacion::Horizontal)
            colActual++;
        else
            filaActual++;
        puedoSeguirAlOtroLado = siguePalabra(occOrd, i, filaActual, colActual);
    }

    return res;
}



//- Se cambia el tipo de dato de occOrd por vector, se agrega i como parámetro para poder acceder a occOrd.
//- Se cambia el cuerpo de la función, pues la implementación del TP1 llevaba a errores de acceso a índices fuera de rango
// COMPLEJIDAD: O(1)
bool Juego::siguePalabra(const vector<tuple<Nat, Nat, Letra>> &occOrd, int i, int fila, int col) const {
    bool res = false;
    if (fila >= 0 && col >= 0 && _tablero.enTablero(fila, col)) {
        res = _tablero.hayLetra(fila, col);            // O(1)
        res |= seUbicaAlgunaLetra(occOrd, i, fila, col);    // O(1)
    }
    return res;
}



//- Se cambia el tipo de dato de occOrd por vector y el orden de los parámetros
//- Se cambia el cuerpo de la función, pues la implementación del TP1 llevaba a errores de acceso a índices fuera de rango
// COMPLEJIDAD: O(1)
bool Juego::seUbicaAlgunaLetra(const vector<tuple<Nat, Nat, Letra>> &occOrd, int i, Nat fila, Nat col) const {
    bool res;
    //- Agregada condición para que i no acceda a una posición inexistente de occOrd
    if (occOrd.empty() || i >= occOrd.size()) {
        res = false;
    }
    else {
        tuple<Nat, Nat, Letra> ficha = occOrd[i];
        res = get<0>(ficha) == fila && get<1>(ficha) == col;
    }
    return res;
}



// Si bien se cambiaron varias funciones y agregadas otras, la complejidad de esJugadaValida se mantiene igual, pues
// como la complejidad de la estructura condicional [*] es O(Lmax^2), que sumada a la complejidad de la nueva función
// seSolapanFichas que es O(Lmax * log(Lmax)) sigue siendo O(Lmax^2).
bool Juego::esJugadaValida(IdCliente j, const Ocurrencia &occ) const {
    bool res = true;
    if (occ.size() > LMAX) {
        return false;
    }

    if (occ.empty()) {
        return true;
    }

    if (!jugadorTieneLasFichas(j, occ)) {
        return false;
    }

    if (!ocurrenciaEnTablero(occ)) {
        return false;
    }

    if (!todasCeldasVacias(occ)) {
        return false;
    }

    // O(Cardinal(occ) * log(Cardinal(occ))), que en este contexto es O(Lmax * log(Lmax))
    if (seSolapanFichas(occ)) {
        return false;
    }


    //- Se cambia la manera de chequear la alineación de la ocurrencia para ahorrar funciones
    vector<tuple<Nat, Nat, Letra>> occOrd = set2Vector(occ);
    Orientacion orientacionPrincipal = alineacion(occ);

    if (orientacionPrincipal != Orientacion::Ninguna) {         // [*]
        //- La complejidad de la siguente función es O(Cardinal(occ)^2), que en este contexto es O(Lmax^2)
        ordenar(occOrd, orientacionPrincipal);
        //- Agregada condición faltante, cuya complejidad es O(Lmax) en el peor caso
        if (hayHuecosEnLaJugada(occOrd, orientacionPrincipal)) {
            return false;
        }
        if (!palabraSuperaLmax(occOrd, orientacionPrincipal)) {
            lPalabra ocurrenciaPrincipal = obtenerPalabra(occOrd, orientacionPrincipal);
            res &= _variante->vPalabraLegitima(ocurrenciaPrincipal);

            int i = 0;  //- índice para recorrer la "ocurrencia" ordenada
            Orientacion orientacionSecundaria = invertirOrientacion(orientacionPrincipal);
            while (i < occOrd.size() && res) {
                //- Como palabraSuperaLmax ya no recibe una cola sino un vector como ocurrencia, se
                //- cambia el tipo de la variable colaAux del TP1
                vector<tuple<Nat, Nat, Letra>> vectorAux(1);
                vectorAux[0] = occOrd[i];
                if (!palabraSuperaLmax(vectorAux, orientacionSecundaria)) {
                    lPalabra ocurrenciaSecundaria = obtenerPalabra(vectorAux, orientacionSecundaria);
                    res &= _variante->vPalabraLegitima(ocurrenciaSecundaria);
                } else {
                    res = false;
                }
                i++;
            }
        } else {
            return false;
        }
    }
    else {
        return false;
    }

    return res;
}




// Funciones auxiliares de ubicarFichas //

void Juego::pasarTurno() {
    //- Condición del if cambiada por ser incorrecta
    if (_turno == _jugadores.size() - 1) {
        _turno = 0;
        _ronda++;
    }
    else {
        _turno++;
    }
}



//- Algoritmo cambiado ligeramente por claridad
void Juego::sacarFichasDelRepositorioDelJugador(IdCliente j, const Ocurrencia &occ) {
    for (const tuple<Nat, Nat, Letra> &ficha: occ) {
        Letra l = get<2>(ficha);
        _jugadores[j].repositorio[ord(l)]--;
    }
}



void Juego::agregarTurnoEnMatriz(Nat turnoTotal, Nat fila, Nat col) {
    _matrizTurnos[fila][col] = turnoTotal;
}




void Juego::ubicarFichas(IdCliente j, const Ocurrencia &occ) {
    if (occ.empty()) {
        pasarTurno();
        return;
    }
    for (const tuple<Nat, Nat, Letra> &ficha: occ) {
        //- Cuerpo del ciclo cambiado por claridad.
        Nat fila = get<0>(ficha);
        Nat col = get<1>(ficha);
        Letra l = get<2>(ficha);
        _tablero.ponerLetra(fila, col, l);
        //- Cambiada la manera en que se calcula el turnoTotal. Con la suma _ronda + _turno podían darse casos
        //- en los que se consideren letras que fueron puestas en un turno posterior.
        agregarTurnoEnMatriz(_ronda * _jugadores.size() + _turno, fila, col);
    }

    // La función alineacion nunca devolverá Orientacion::Ninguna, pues la ocurrencia será una jugada válida.
    Orientacion orientacion = alineacion(occ);
    sacarFichasDelRepositorioDelJugador(j, occ);
    darFichas(j, occ.size());
    OcurrenciaColocada ocurrenciaColocada = OcurrenciaColocada(occ, _ronda * _jugadores.size() + _turno, orientacion);
    _jugadores[j].puntaje.ultimasOcurrenciasColocadas.push_back(ocurrenciaColocada);
    pasarTurno();
}




// Aliasing: Devuelve una referencia modificable de Variante
Variante Juego::obtenerVariante() const {
    return *_variante;
}




IdCliente Juego::turno() const {
    return _turno;
}



//- Se cambia el cuerpo de la función para evitar indefiniciones, pero la complejidad se mantiene igual.
bool Juego::sigueJugada(Nat turnoTotal, int fila, int col) const {
    bool res = false;
    if (fila >= 0 && col >= 0 && _tablero.enTablero(fila, col)) {
        res = _tablero.hayLetra(fila, col);
        res &= _matrizTurnos[fila][col] <= turnoTotal;
    }
    return res;
}


//- Se combinan las funciones obtenerPalabraHorizontalDeJugada y obtenerPalabraVerticalDeJugada, haciendo
//- uso del parámetro de la orientación de la palabra que está ubicada en el tablero.
//- Se cambia la manera de ir obteniendo las fichas del tablero en los dos ciclos while, pues podían darse
//- casos en los que se agregaban caracteres nulos a la palabra.
// Si bien la función tiene estos agregados, la complejidad del algoritmo se mantiene igual a la del TP1
lPalabra Juego::obtenerPalabraDeJugada(tuple<Nat, Nat, Letra> ficha, Nat turnoTotal, Juego::Orientacion orientacion) const {
    lPalabra res;
    bool puedoSeguirAUnLado;
    bool puedoSeguirAlOtroLado;

    res.push_front(get<2>(ficha));
    int filaActual;
    int colActual;

    if (orientacion == Orientacion::Horizontal) {
        filaActual = get<0>(ficha);
        colActual = get<1>(ficha)-1;
    }
    else {  // orientacion == Orientacion::Vertical
        filaActual = get<0>(ficha)-1;
        colActual = get<1>(ficha);
    }

    puedoSeguirAUnLado = sigueJugada(turnoTotal, filaActual, colActual);

    while (puedoSeguirAUnLado) {
        if (orientacion == Orientacion::Horizontal) {
            res.push_front(_tablero.letraEn(filaActual, colActual));
            colActual--;
            puedoSeguirAUnLado = sigueJugada(turnoTotal, filaActual, colActual);
        }
        else {
            res.push_front(_tablero.letraEn(filaActual, colActual));
            filaActual--;
            puedoSeguirAUnLado = sigueJugada(turnoTotal, filaActual, colActual);
        }
    }

    if (orientacion == Orientacion::Horizontal) {
        filaActual = get<0>(ficha);
        colActual = get<1>(ficha)+1;
    }
    else {  // orientacion == Orientacion::Vertical
        filaActual = get<0>(ficha)+1;
        colActual = get<1>(ficha);
    }

    puedoSeguirAlOtroLado = sigueJugada(turnoTotal, filaActual, colActual);

    while (puedoSeguirAlOtroLado) {
        if (orientacion == Orientacion::Horizontal) {

            res.push_back(_tablero.letraEn(filaActual, colActual));
            colActual++;
            puedoSeguirAlOtroLado = sigueJugada(turnoTotal, filaActual, colActual);

        }
        else {
            res.push_back(_tablero.letraEn(filaActual, colActual));
            filaActual++;
            puedoSeguirAlOtroLado = sigueJugada(turnoTotal, filaActual, colActual);
        }
    }

    return res;
}



Nat Juego::obtenerPuntajePalabra(const lPalabra &palabra) const {
    Nat puntajePalabra = 0;
    for (const Letra &caracter: palabra) {
        Nat puntajeLetra = _variante->vPuntajeLetra(caracter);
        puntajePalabra += puntajeLetra;
    }
    return puntajePalabra;
}



Nat Juego::puntaje(IdCliente j) {
    const vector<OcurrenciaColocada>* jugadasHechas = &_jugadores[j].puntaje.ultimasOcurrenciasColocadas;
    Nat nuevoPuntaje = _jugadores[j].puntaje.puntajeAnterior;
    //- Cuerpo del ciclo cambiado para hacer uso de las funciones que reciben la orientación
    //- de la palabra principal como parámetro
    for (const OcurrenciaColocada &jugada: *jugadasHechas) {

        Orientacion orientacionPrincipal = alineacion(jugada.occ);
        Orientacion orientacionSecundaria = invertirOrientacion(orientacionPrincipal);

        tuple<Nat, Nat, Letra> algunaFicha = dameUno(jugada.occ);
        lPalabra palabra = obtenerPalabraDeJugada(algunaFicha, jugada.turnoTotalColocado, orientacionPrincipal);
        Nat puntajePalabra = obtenerPuntajePalabra(palabra);
        nuevoPuntaje += puntajePalabra;

        for (const tuple<Nat, Nat, Letra> &ficha: jugada.occ) {
            palabra = obtenerPalabraDeJugada(ficha, jugada.turnoTotalColocado, orientacionSecundaria);
            puntajePalabra = obtenerPuntajePalabra(palabra);
            nuevoPuntaje += puntajePalabra;
        }

    }
    // Borro las jugadas hechas para no volver a sumar los puntos de ellas al volver a llamar a puntaje
    _jugadores[j].puntaje = PuntajeTotal(nuevoPuntaje);

    return nuevoPuntaje;
}




Letra Juego::contenidoEnCoordenada(tuple<Nat, Nat> coord) const {
    return _tablero.letraEn(get<0>(coord), get<1>(coord));
}




Nat Juego::cantFichasDeLetra(IdCliente j, Letra x) const {
    return _jugadores[j].repositorio[ord(x)];
}




void Juego::mostrar(ostream &os, bool mostrarPuntaje) {
    os << "JUEGO" << endl;
    os << "Estado del tablero:" << endl;
    os << _tablero;
    os << endl;

    os << "Es la ronda " << _ronda << " y el turno del jugador " << _turno << endl;
    os << endl;

    os << "El repositorio general tiene " << _repositorioGeneral->size() << " fichas." << endl;
    os << "Las primeras 10 fichas son: ";
    auto itRepo = _repositorioGeneral->begin();
    for (int i=0; i < 10 && itRepo != _repositorioGeneral->end(); i++) {
        os << "'" << *itRepo << "', ";
        ++itRepo;
    }
    os << endl;
    os << "Jugadores: " << endl;
    for (int i=0; i < _jugadores.size(); i++) {
        os << "Jugador " << i << endl;
        os << "        Su repositorio tiene las fichas: " << endl;
        os << "                {";
        for (int j=0; j < TAMANIO_ALFABETO; j++) {
            if (_jugadores[i].repositorio[j] != 0) {
                os << "'" << inversaDeOrd(j) << "': "<< _jugadores[i].repositorio[j] << ", ";
            }
        }
        os << "}" << endl;
        if (mostrarPuntaje) {
            os << "        Su puntaje es " << puntaje(i) << endl;
        }
    }

}




const Repositorio* Juego::repositorioDeLetras() const {
    return _repositorioGeneral;
}