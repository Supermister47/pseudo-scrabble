#ifndef TP_JUEGODEPALABRAS_SERVIDOR_H
#define TP_JUEGODEPALABRAS_SERVIDOR_H

#include "Notificacion.h"
#include "Tipos.h"
#include "Juego.h"
#include "Variante.h"

class Servidor{
public:

    /**
     * Se crea un nuevo Servidor.
     * @param K: cantidad de jugadores deseada
     * @param v: variante a jugar
     * @param r: repositorio de letras con el que se jugará
     */
    Servidor(Nat K, Variante &v, Repositorio &r);

    ~Servidor();

    /**
     * Devuelve la cantidad de jugadores esperados para empezar el juego.
     */
    Nat esperados() const;

    /**
     * Devuelve la cantidad de jugadores conectados al servidor actualmente.
     */
    Nat conectados() const;

    /**
     * Devuelve el Juego.
     * @pre @code this.empezo()  @endcode
     */
    Juego obtenerJuego();

    /**
     * Conecta un cliente al Servidor.
     * @pre @code !this.empezo()  @endcode
     */
    void conectarCliente();

    /**
     * Consulta la cola de notificaciones del jugador. Además, borra su cola de notificaciones.
     * @pre @code id &lt this.conectados()  @endcode
     * @param id: número de jugador
     */
    vector<Notificacion> consultar(IdCliente id);

    /**
     * Permite que el cliente comunique su jugada y reciba una o varias notificaciones dependiendo de si su jugada es válida o no.
     * @pre @code id &lt this.conectados()  @endcode
     * @param id: número de jugador
     * @param o: ocurrencia que se quiere jugar
     */
    void recibirMensaje(IdCliente id, const Ocurrencia &o);

    /**
     * Determina si el juego comenzó.
     */
    bool empezo() const;

    // Función auxiliar hecha para debuggear los tests en algunos casos.
    void mostrar(ostream& os);

private:

    struct NotifIndividual {

        Notificacion _notif;

        Nat _tamListaGrupalAlAgregarNotifIndividual;

        NotifIndividual(Notificacion n, Nat tam) : _notif(n), _tamListaGrupalAlAgregarNotifIndividual(tam) { }
    };

    struct DatosNotifIndividual{
        Nat _tamanioAnt;

        list<Notificacion>::const_iterator _itNotifGrupal;

        queue<NotifIndividual> _colaIndividual;

        DatosNotifIndividual(Nat tam, list<Notificacion>::const_iterator it, queue<NotifIndividual> cola): _tamanioAnt(tam), _itNotifGrupal(it), _colaIndividual(cola) { }

    };


    tuple<list<Notificacion>*, vector<DatosNotifIndividual>> _listaDeNotif;

    Nat _esperados;

    Nat _conectados;

    Juego _juego;

    Repositorio::const_iterator _itFichasRepositorio;

    Notificacion hacerNotifReponer(Nat cantFichas);


};

#endif //TP_JUEGODEPALABRAS_SERVIDOR_H