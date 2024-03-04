#include "Servidor.h"
#include "Juego.h"
#include "Tipos.h"
#include "Variante.h"
#include "Tablero.h"
using namespace std;




Servidor::Servidor(Nat K, Variante &v, Repositorio &r): _juego(K, v, r) {

    vector<DatosNotifIndividual> vectorVacio;

    _esperados = K;
    _conectados = 0;

    get<0>(_listaDeNotif) = new list<Notificacion>();
    get<1>(_listaDeNotif) = vectorVacio;

    _itFichasRepositorio = this->obtenerJuego().repositorioDeLetras()->begin();

}




Servidor::~Servidor() {
    delete get<0>(_listaDeNotif);
}




Nat Servidor::esperados() const {
    return _esperados;
}




Nat Servidor::conectados() const {
    return _conectados;
}




Juego Servidor::obtenerJuego() {
    return _juego;
}




void Servidor::conectarCliente() {
    Nat tamanioAnt = 0;

    list<Notificacion>* notifGrupales = get<0>(_listaDeNotif);
    vector<DatosNotifIndividual>* vectorDatosIndividuales = &get<1>(_listaDeNotif);
    queue<NotifIndividual> notifInd;
    auto it =  notifGrupales->begin();

    notifInd.push(NotifIndividual(Notificacion::nuevaIdCliente(_conectados), notifGrupales->size()));
    vectorDatosIndividuales->push_back(DatosNotifIndividual(tamanioAnt, it, notifInd));
    _conectados++;

    if (_conectados == _esperados) {
        notifGrupales->push_back(Notificacion::nuevaEmpezar(this->obtenerJuego().obtenerVariante().vTamanoTablero()));
        notifGrupales->push_back(Notificacion::nuevaTurnoDe(0));

        //- Se reparten las fichas al empezar la partida, ciclo que no está en el TP1.
        for (int j = 0; j < _esperados; j++) {
            Notificacion reposicion = hacerNotifReponer(this->obtenerJuego().obtenerVariante().vFichas());
            Nat tamGrup = notifGrupales->size();

            vectorDatosIndividuales->operator[](j)._colaIndividual.push(NotifIndividual(reposicion, tamGrup));
        }
    }
}





vector<Notificacion> Servidor::consultar(IdCliente id) {
    vector<Notificacion> mostrar;
    list<Notificacion>* notifGrupales = get<0>(_listaDeNotif);
    DatosNotifIndividual* datosIndividual = &get<1>(_listaDeNotif)[id];
    queue<NotifIndividual>* notifsIndividual = &datosIndividual->_colaIndividual;

    auto itNotifGrupales = &datosIndividual->_itNotifGrupal;

    // Iniciar el iterador de las notificaciones grupales cuando este está vacío trae el problema de que al agregar
    // un elemento a la lista grupal, este iterador apunta al elemento ANTERIOR al primero. Por tanto, se chequea
    // que si al incrementar el iterador este es igual a notifGrupales->begin(), se lo iguala a este valor (por seguridad)

    //- Cabe notar que el iterador no se decrementa luego de evaluar la condición. Esto es así debido al comportamiento
    //- que tiene el iterador una vez que este es igual a notifGrupales->end() (o sea, cuando se consultaron las
    //- notificaciones alguna vez). Ocurre que el iterador siempre será igual a notifGrupales->end() sin importar
    //- cuántas notificaciones se hayan agregado a la lista grupal. Por lo tanto, una vez consultada la lista de
    //- notificaciones, se decrementa el iterador ([*]) para que no se mantenga siempre en notifGrupales->end().
    //- Si bien esto rompe la función de representación de Servidor, es la manera que se nos ocurrió para remediar
    //- este problema, pues esto no ocurre en el iterador bidireccional teórico.
    if (!notifGrupales->empty() && ++(*itNotifGrupales) == notifGrupales->begin()) {
        *itNotifGrupales = notifGrupales->begin();
    }

    Nat contador = datosIndividual->_tamanioAnt;
    while(*itNotifGrupales != (*notifGrupales).end() && !notifsIndividual->empty() ){

        if (contador < notifsIndividual->front()._tamListaGrupalAlAgregarNotifIndividual){
            mostrar.push_back(**itNotifGrupales);
            contador++;
            ++*itNotifGrupales;
        }
        else {
            mostrar.push_back(notifsIndividual->front()._notif);
            notifsIndividual->pop();
        }
        datosIndividual->_tamanioAnt = contador;
    }

    while (*itNotifGrupales != (*notifGrupales).end()) {
        mostrar.push_back(**itNotifGrupales);
        contador++;
        ++*itNotifGrupales;
    }
    datosIndividual->_tamanioAnt = contador;

    while (!notifsIndividual->empty()) {
        mostrar.push_back(notifsIndividual->front()._notif);
        notifsIndividual->pop();
    }
    --*itNotifGrupales; // [*]

    return mostrar;
}




Notificacion Servidor::hacerNotifReponer(Nat cantFichas) {
    multiset<Letra> reposicion;
    for (int i = 0; i < cantFichas; i++) {
        reposicion.insert(*_itFichasRepositorio);
        ++_itFichasRepositorio;
    }
    return Notificacion::nuevaReponer(reposicion);
}



void Servidor::recibirMensaje(IdCliente id, const Ocurrencia &o) {
    list<Notificacion>* notifGrupales = get<0>(_listaDeNotif);
    DatosNotifIndividual* datosIndividual = &get<1>(_listaDeNotif)[id];

    if (this->empezo() && this->obtenerJuego().turno() == id && _juego.esJugadaValida(id, o)){
        multiset<Letra> reposicion;

        // Obtenemos el puntaje del jugador antes de hacer la jugada y ubicamos las fichas, agregando la notificación
        // UBICAR a la lista grupal
        notifGrupales->push_back(Notificacion::nuevaUbicar(id, o));
        Nat puntajeAnterior = _juego.puntaje(id);
        _juego.ubicarFichas(id,o);

        // No se considera el caso en el que el repositorio tenga menos fichas que el tamaño de la ocurrencia.
        Nat cantFichas = o.size();

        // Ahora se agrega la notificación de SUMAPUNTOS.
        Notificacion sumaPuntos = Notificacion::nuevaSumaPuntos(id,_juego.puntaje(id) - puntajeAnterior);
        Notificacion reponerNotif = hacerNotifReponer(cantFichas);

        notifGrupales->push_back(sumaPuntos);

        // Se agrega la notificación de REPONER a la cola individual.
        Nat tamanioGrup = notifGrupales->size();
        datosIndividual->_colaIndividual.push(NotifIndividual(reponerNotif, tamanioGrup));

        // Finalmente, se agrega la notificación de TURNODE.
        notifGrupales->push_back(Notificacion::nuevaTurnoDe(_juego.turno()));
    }
    else {
        Nat tamanioGrup = get<0>(_listaDeNotif)->size();
        datosIndividual->_colaIndividual.push(NotifIndividual(Notificacion::nuevaMal(), tamanioGrup));
    }
}




bool Servidor::empezo() const {
    return this->_esperados == this->_conectados;
}




void Servidor::mostrar(ostream &os) {
    os << "NOTIFICACIONES: " << endl;
    for (int i=0; i < _conectados; i++) {
        os << "Jugador " << i << endl;
        os << "        Su cola de notificaciones es: " << endl;
        os << "                [";
        vector<Notificacion> notifs = consultar(i);
        for (Notificacion n: notifs) {
            switch (n.tipoNotificacion()) {
                case TipoNotificacion::IdCliente:
                    os << "{IdCliente: " << n.idCliente() << "}, ";
                    break;
                case TipoNotificacion::Empezar:
                    os << "{Empezar: " << n.empezar() << "}, ";
                    break;
                case TipoNotificacion::TurnoDe:
                    os << "{TurnoDe: " << n.turnoDe() << "}, ";
                    break;
                case TipoNotificacion::Ubicar:
                    os << "{Ubicar" << "}, ";
                    break;
                case TipoNotificacion::Reponer:

                    os << "{Reponer: {";
                    for (Letra l: n.reponer()) {
                        os << l << ", ";
                    }
                    os << "}}, ";

                    break;
                case TipoNotificacion::SumaPuntos:
                    os << "{SumaPuntos: " << get<1>(n.sumaPuntos()) << "}, ";
                    break;
                case TipoNotificacion::Mal:
                    os << "{Mal}, ";
                    break;
            }
        }
        os << "]" << endl;
    }
}