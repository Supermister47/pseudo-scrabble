#include "gtest-1.8.1/gtest.h"
#include "../src/Fachada_Servidor.h"

#include "utils/PalabrasComunes.h"
#include "utils/Repositorio.h"
#include "utils/Puntajes.h"

class JuegoServidorEjemplosTests : public testing::Test {
protected:
    Nat cantJugadores = 4;
    Nat tamanoTab = 8;
    Nat cantFichas = 147;

    map<Letra, Nat> puntajes;
    set<vector<Letra> > palabrasLegitimas;
    Repositorio repo;

    Fachada_Variante variante;
    Fachada_Servidor fachada;

    JuegoServidorEjemplosTests():
            puntajes(puntajesIngles),
            palabrasLegitimas(begin(palabrasComunes), end(palabrasComunes)),
            repo(RepositorioGrande::repositorioGrande()),
            variante(tamanoTab, cantFichas, puntajes, palabrasLegitimas),
            fachada(cantJugadores, variante, repo)
    {
        for (Nat i = 0; i < cantJugadores; i++){
            fachada.conectarCliente();
        }
    }

};

// Si bien no se usa ningún EXPECT, se hace este test para chequear visualmente con la función mostrar
TEST_F(JuegoServidorEjemplosTests, juego_servidor_segundo_ejemplo_especificacion){



    Ocurrencia jugada = {{0, 0, 't'}, {0, 1, 'o'}, {0, 2, 'm'}, {0, 3, 'a'}, {0, 4, 'r'}, {0, 5, 'e'}};
    Nat id = 0;

    fachada.recibirMensaje(id, jugada);
    id++;
    //// juego.mostrar(cout, false);

    jugada = {{2, 0, 'j'}, {2, 1, 'i'}, {2, 2, 'r'}, {2, 3, 'a'}, {2, 4, 'f'}, {2, 5, 'a'}};
    fachada.recibirMensaje(id, jugada);
    id++;
    //// juego.mostrar(cout, false);

    jugada = {{4, 0, 'b'}, {4, 1, 'o'}, {4, 2, 'd'}, {4, 3, 'e'}, {4, 4, 'g'}, {4, 5, 'a'}};
    fachada.recibirMensaje(id, jugada);
    id++;
    //// juego.mostrar(cout, false);

    // JUgador 3 tendrá un MAL
    jugada = {{4, 0, 'b'}, {4, 1, 'o'}, {4, 2, 'd'}, {4, 3, 'e'}, {4, 4, 'g'}, {4, 5, 'a'}};
    fachada.recibirMensaje(id, jugada);


    jugada = {{1, 0, 'a'}, {3, 0, 'a'}, {5, 0, 'a'}};
    fachada.recibirMensaje(id, jugada);
    id = 0;
    //// juego.mostrar(cout, false);

    jugada = {{1, 2, 'o'}, {3, 2, 'a'}, {5, 2, 'a'}};
    fachada.recibirMensaje(id, jugada);
    id++;
    //// juego.mostrar(cout, false);

    jugada = {{1, 4, 'a'}, {3, 4, 'a'}, {5, 4, 'a'}};
    fachada.recibirMensaje(id, jugada);
    id++;
    //// juego.mostrar(cout, false);

    jugada = {{1, 1, 's'}, {3, 1, 't'}, {5, 1, 's'}};
    fachada.recibirMensaje(id, jugada);
    id++;
    //// juego.mostrar(cout, false);

    jugada = {{1, 3, 'm'}, {1, 5, 's'}, {1, 6, 't'}, {1, 7, 'e'}};
    fachada.recibirMensaje(id, jugada);
    id = 0;
    // juego.mostrar(cout, true);

    jugada = {};
    fachada.recibirMensaje(id, jugada);
    id++;

    fachada.mostrar(cout);
};