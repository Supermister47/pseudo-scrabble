### Trabajo práctico de implementación
Para este trabajo nos pidieron implementar un juego similar al Scrabble. Para comenzar, tuvimos que diseñar algoritmos en un lenguaje abstracto similar a C++ que cumplan ciertos requisitos de complejidad.

Luego, en este trabajo nos pidieron implementar los algoritmos en C++.

El Juego se desarrolla por turnos, cada jugador es representado por un cliente, y todos ellos son manejados por un Servidor, que además se encarga de manejar el Tablero. En el turno de cada jugador, este puede hacer una jugada en el tablero. Si la jugada es válida, el Servidor le asigna un puntaje y la ubica en el Tablero, caso contrario rechaza la jugada. Cada cliente tiene una cola de notificaciones, que utiliza para enterarse de las jugadas de los otros jugadores (pues ningún cliente tiene acceso directo al estado del tablero). De esta manera, cada jugador puede reconstruir el estado actual del juego consultando su cola de notificaciones. 

Se proveen una serie de tests para comprobar el correcto funcionamiento del trabajo. Puede también ejecutarse con Valgrind para verificar que no se pierde memoria.