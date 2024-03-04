#include "gtest-1.8.1/gtest.h"
#include "../src/Fachada_Juego.h"

#include "utils/PalabrasComunes.h"
#include "utils/Repositorio.h"
#include "utils/Puntajes.h"

class JugadasValidasTests : public testing::Test {
protected:
    Nat cantJugadores = 3;
    Nat tamanoTab = 8;
    Nat cantFichas = 147;

    map<Letra, Nat> puntajes;
    set<vector<Letra> > palabrasLegitimas;
    Repositorio repo;

    Fachada_Variante variante;

    JugadasValidasTests():
            puntajes(puntajesIngles),
            palabrasLegitimas(begin(palabrasComunes), end(palabrasComunes)),
            repo(RepositorioGrande::repositorioGrande()),
            variante(tamanoTab, cantFichas, puntajes, palabrasLegitimas)
    {}

};



TEST_F(JugadasValidasTests, juego_una_letra_valida_en_todo_el_tablero){

    Fachada_Juego juego(cantJugadores, variante, repo);

    // juego.mostrar(cout, false);

    for (int i=0; i < juego.variante().tamanoTablero(); i++) {
        for (int j=0; j < juego.variante().tamanoTablero(); j++) {
            EXPECT_TRUE(juego.jugadaValida({{i, j, 'a'}}));
        }
    }
};

TEST_F(JugadasValidasTests, juego_palabra_horizontal_tres_letras_en_todo_el_tablero){

    Fachada_Juego juego(cantJugadores, variante, repo);

    // juego.mostrar(cout, false);

    for (int i=0; i < juego.variante().tamanoTablero(); i++) {
        for (int j=0; j < juego.variante().tamanoTablero()-2; j++) {
            EXPECT_TRUE(juego.jugadaValida({{i, j+2, 's'}, {i, j, 'l'}, {i, j+1, 'o'}}));
        }
    }
};

TEST_F(JugadasValidasTests, juego_palabra_vertical_tres_letras_en_todo_el_tablero){

    Fachada_Juego juego(cantJugadores, variante, repo);

    // juego.mostrar(cout, false);

    for (int i=0; i < juego.variante().tamanoTablero()-2; i++) {
        for (int j=0; j < juego.variante().tamanoTablero(); j++) {
            EXPECT_TRUE(juego.jugadaValida({{i+2, j, 's'}, {i, j, 'l'}, {i+1, j, 'o'}}));
        }
    }
};


// Si bien no se usa ningún EXPECT, se hace este test para chequear visualmente con la función mostrar
TEST_F(JugadasValidasTests, juego_segundo_ejemplo_especificacion){

    Fachada_Juego juego(cantJugadores, variante, repo);

    cout << endl;
    cout << endl;
    cout << "juego_segundo_ejemplo_especificacion" << endl;




    Ocurrencia jugada = {{0, 0, 't'}, {0, 1, 'o'}, {0, 2, 'm'}, {0, 3, 'a'}, {0, 4, 'r'}, {0, 5, 'e'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
    //// juego.mostrar(cout, false);

    jugada = {{2, 0, 'j'}, {2, 1, 'i'}, {2, 2, 'r'}, {2, 3, 'a'}, {2, 4, 'f'}, {2, 5, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
    //// juego.mostrar(cout, false);

    jugada = {{4, 0, 'b'}, {4, 1, 'o'}, {4, 2, 'd'}, {4, 3, 'e'}, {4, 4, 'g'}, {4, 5, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
    //// juego.mostrar(cout, false);

    jugada = {{4, 0, 'b'}, {4, 1, 'o'}, {4, 2, 'd'}, {4, 3, 'e'}, {4, 4, 'g'}, {4, 5, 'a'}};
    EXPECT_FALSE(juego.jugadaValida(jugada));


    jugada = {{1, 0, 'a'}, {3, 0, 'a'}, {5, 0, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
    //// juego.mostrar(cout, false);

    jugada = {{1, 2, 'o'}, {3, 2, 'a'}, {5, 2, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
    //// juego.mostrar(cout, false);

    jugada = {{1, 4, 'a'}, {3, 4, 'a'}, {5, 4, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
    //// juego.mostrar(cout, false);

    jugada = {{1, 1, 's'}, {3, 1, 't'}, {5, 1, 's'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
    //// juego.mostrar(cout, false);

    jugada = {{1, 3, 'm'}, {1, 5, 's'}, {1, 6, 't'}, {1, 7, 'e'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
    juego.mostrar(cout, true);
};

// Si bien no se usa ningún EXPECT, se hace este test para chequear visualmente con la función mostrar, además, se chequea
// que el puntaje de cada jugador coincida con el test anterior (juego_segundo_ejemplo_especificacion)
TEST_F(JugadasValidasTests, juego_segundo_ejemplo_especificacion_viendo_puntajes_en_cada_turno){

    Fachada_Juego juego(cantJugadores, variante, repo);

    cout << endl;
    cout << endl;
    cout << "juego_segundo_ejemplo_especificacion_viendo_puntajes_en_cada_turno" << endl;

    juego.mostrar(cout, false);



    Ocurrencia jugada = {{0, 0, 't'}, {0, 1, 'o'}, {0, 2, 'm'}, {0, 3, 'a'}, {0, 4, 'r'}, {0, 5, 'e'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
     juego.mostrar(cout, true);

    jugada = {{2, 0, 'j'}, {2, 1, 'i'}, {2, 2, 'r'}, {2, 3, 'a'}, {2, 4, 'f'}, {2, 5, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
     juego.mostrar(cout, true);

    jugada = {{4, 0, 'b'}, {4, 1, 'o'}, {4, 2, 'd'}, {4, 3, 'e'}, {4, 4, 'g'}, {4, 5, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
     juego.mostrar(cout, true);

    jugada = {{4, 0, 'b'}, {4, 1, 'o'}, {4, 2, 'd'}, {4, 3, 'e'}, {4, 4, 'g'}, {4, 5, 'a'}};
    EXPECT_FALSE(juego.jugadaValida(jugada));


    jugada = {{1, 0, 'a'}, {3, 0, 'a'}, {5, 0, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
     juego.mostrar(cout, true);

    jugada = {{1, 2, 'o'}, {3, 2, 'a'}, {5, 2, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
     juego.mostrar(cout, true);

    jugada = {{1, 4, 'a'}, {3, 4, 'a'}, {5, 4, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
     juego.mostrar(cout, true);

    jugada = {{1, 1, 's'}, {3, 1, 't'}, {5, 1, 's'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
     juego.mostrar(cout, true);

    jugada = {{1, 3, 'm'}, {1, 5, 's'}, {1, 6, 't'}, {1, 7, 'e'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
     juego.mostrar(cout, true);
};



TEST_F(JugadasValidasTests, juego_test_puntaje){

    Fachada_Juego juego(cantJugadores, variante, repo);

    // juego.mostrar(cout, false);

    cout << endl;
    cout << endl;
    cout << "juego_segundo_ejemplo_especificacion_viendo_puntajes_en_cada_turno" << endl;
    Ocurrencia jugada = {{0, 0, 'a'}, {0, 1, 's'}, {0, 2, 'a'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    juego.ubicar(jugada);
    juego.mostrar(cout, false);

    jugada = {{0, 3, 'd'}, {0, 4, 'o'}};
    EXPECT_TRUE(juego.jugadaValida(jugada));
    // Si bien la jugada no es válida (pues la palabra 'asado' no está en PalabrasComunes), se la ubica para ver que los puntajes sean correctos
    juego.ubicar(jugada);
    juego.mostrar(cout, true);

    EXPECT_EQ(juego.puntaje(0), 6);
    EXPECT_EQ(juego.puntaje(1), 9);

};

TEST_F(JugadasValidasTests, juego_ocurrencias_fuera_del_tablero){

    Fachada_Juego juego(cantJugadores, variante, repo);

    Ocurrencia jugada = {{0, 8, 'a'}, {0, 9, 's'}, {0, 10, 'a'}};
    EXPECT_FALSE(juego.jugadaValida(jugada));

    jugada = {{0, 6, 'a'}, {0, 7, 's'}, {0, 8, 'a'}};
    EXPECT_FALSE(juego.jugadaValida(jugada));
};

TEST_F(JugadasValidasTests, juego_ocurrencias_solapadas){

    Fachada_Juego juego(cantJugadores, variante, repo);

    Ocurrencia jugada = {{0, 0, 'a'}, {0, 0, 's'}, {0, 0, 'a'}};
    EXPECT_FALSE(juego.jugadaValida(jugada));

    jugada = {{0, 0, 'a'}, {0, 1, 's'}, {0, 1, 'a'}};
    EXPECT_FALSE(juego.jugadaValida(jugada));
};