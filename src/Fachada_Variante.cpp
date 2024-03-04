#include "Tipos.h"
#include "Fachada_Variante.h"



Fachada_Variante::Fachada_Variante(Nat tamanoTab, Nat cantFichas, const map<Letra, Nat>& puntajes, const set<vector<Letra>>& palabrasLegitimas):
                        var(Variante(tamanoTab, cantFichas, puntajes, palabrasLegitimas)){};

Fachada_Variante::Fachada_Variante(const Fachada_Variante& fvCopiar):var(Variante(fvCopiar.var)){};




Fachada_Variante::~Fachada_Variante()= default;




Nat Fachada_Variante::tamanoTablero() const {
    return var.vTamanoTablero();
}

Nat Fachada_Variante::fichas() const{
    return var.vFichas();
}

Nat Fachada_Variante::puntajeLetra(Letra l) const{
    return var.vPuntajeLetra(l);
}

bool Fachada_Variante::palabraLegitima(const Palabra& palabra) const{
    lPalabra p = convALista(palabra);
    return var.vPalabraLegitima(p);
}

Nat Fachada_Variante::tamPalabraMasLarga() const {
    var.vTamPalabraMasLarga();
}

Variante Fachada_Variante::devolver_variante() const {
    return var;
}







