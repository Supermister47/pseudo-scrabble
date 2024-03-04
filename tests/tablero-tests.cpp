#include "gtest-1.8.1/gtest.h"
#include "../src/Tablero.h"

#include "utils/PalabrasComunes.h"
#include "../src/Letra.h"



TEST(TableroTest, tablero_vacio) {
    Nat tamanoTab = 22;
    Tablero t = Tablero(tamanoTab);
    for (int i=0; i < tamanoTab; i++) {
        for (int j=0; j < tamanoTab; j++) {
            EXPECT_EQ(t.hayLetra(i, j), false);
        }
    }
};

TEST(TableroTest, tablero_1_1) {
    Nat tamanoTab = 1;
    Tablero t = Tablero(tamanoTab);
    for (int i=0; i < tamanoTab; i++) {
        for (int j=0; j < tamanoTab; j++) {
            EXPECT_EQ(t.hayLetra(i, j), false);
        }
    }
    //cout << t;
};

TEST(TableroTest, tablero_chico_con_una_letra) {
    Nat tamanoTab = 8;
    Tablero t = Tablero(tamanoTab);
    t.ponerLetra(4,6, 'a');
    for (int i=0; i < tamanoTab; i++) {
        for (int j=0; j < tamanoTab; j++) {
            if (i == 4 && j == 6) {
                EXPECT_EQ(t.hayLetra(i, j), true);
                EXPECT_EQ(t.letraEn(i, j), 'a');
            }
            else
                EXPECT_EQ(t.hayLetra(i, j), false);
        }
    }
    cout << t;
};

TEST(TableroTest, tablero_con_una_letra) {
    Nat tamanoTab = 22;
    Tablero t = Tablero(tamanoTab);
    t.ponerLetra(4,7, 'a');
    for (int i=0; i < tamanoTab; i++) {
        for (int j=0; j < tamanoTab; j++) {
            if (i == 4 && j == 7) {
                EXPECT_EQ(t.hayLetra(i, j), true);
                EXPECT_EQ(t.letraEn(i, j), 'a');
            }
            else
                EXPECT_EQ(t.hayLetra(i, j), false);
        }
    }
};

TEST(TableroTest, llenar_tablero) {
    Nat tamanoTab = 22;
    Tablero t = Tablero(tamanoTab);
    t.ponerLetra(4,7, 'a');

    for (int i=0; i < tamanoTab; i++) {
        for (int j=0; j < tamanoTab; j++) {
            t.ponerLetra(i, j, inversaDeOrd(i));
        }
    }

    for (int i=0; i < tamanoTab; i++) {
        for (int j=0; j < tamanoTab; j++) {
            EXPECT_EQ(t.hayLetra(i, j), true);
            EXPECT_EQ(t.letraEn(i, j), inversaDeOrd(i));
        }
    }
    cout << t;
    EXPECT_EQ(t.Tam(), 22);
};
