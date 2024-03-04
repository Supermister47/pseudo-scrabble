#include "Fachada_Servidor.h"
#include "Fachada_Variante.h"




Fachada_Servidor::Fachada_Servidor(Nat cantJugadores, const Fachada_Variante &variante, const Repositorio &r) : _fVar(variante),_vHeap(variante.devolver_variante()), _rHeap(r), _serv(cantJugadores, _vHeap, _rHeap) { }

void Fachada_Servidor::recibirMensaje(IdCliente id, const Ocurrencia &o) {
    _serv.recibirMensaje(id, o);
}

IdCliente Fachada_Servidor::conectarCliente() {
    _serv.conectarCliente();
    return _serv.conectados()-1;
}

std::list<Notificacion> Fachada_Servidor::notificaciones(IdCliente id) {
    vector<Notificacion> n = _serv.consultar(id);
    return std::list<Notificacion>(n.begin(), n.end());
}

Nat Fachada_Servidor::jugadoresConectados() {
    return _serv.conectados();
}

Nat Fachada_Servidor::jugadoresEsperados() {
    return _serv.esperados();
}

void Fachada_Servidor::mostrar(ostream &os) {
    _serv.mostrar(os);
}