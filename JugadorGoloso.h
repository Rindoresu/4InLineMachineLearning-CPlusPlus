#ifndef ALGO_3_TP3_JUGADORGOLOSO_H
#define ALGO_3_TP3_JUGADORGOLOSO_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <cmath>
#include <sys/stat.h>
#include <algorithm>

using namespace std;

struct game_rules { //Preferible inicializar antes sino es 4 en linea como jugador rojo.
    int columns=7, rows=6, c=4, p=21;
    string color="rojo", opponent_color="azul";
};


/* Agrega jugada suponiendo que hay espacio (caso contrario no hay que llamarla.) */
void modificarTablero(const game_rules & this_game, vector<vector<string> > & board, const int & move, bool MiJugada);


/* Deshace la jugada move suponiendo que la ultima jugada fue nuestra y fue realizada en la columna move. */
void deshacerTablero(const game_rules & this_game, vector<vector<string> > & board, const int & move);


/* Busca PseudoRectas Horizontales, Correspondiente al Criterio 2*/
void chequearRectasHorizontalesPosibles(const int & f, const int & c, const game_rules & this_game,
                                        const vector<vector<string> > & board, vector<int>& cantMiasPosibles,
                                        vector<int>& cantOponentePosibles, vector<vector<bool> >& tableroExaminado);

/* Busca PseudoRectas Verticales, Correspondiente al Criterio 2 */
void chequearRectasVerticalesPosibles(const int & f, const int & c, const game_rules & this_game,
                                      const vector<vector<string> > & board, vector<int>& cantMiasPosibles,
                                      vector<int>& cantOponentePosibles, vector<vector<bool> >& tableroExaminado2);

/* Busca PseudoRectas Diagonales Positivas y Negativas, Correspondiente al Criterio 2 */
void chequearRectasDiagonalesPosibles(const int & f, const int & c, const game_rules & this_game,
                                      const vector<vector<string> > & board, vector<int>& cantMiasPosibles, vector<int>& cantOponentePosibles,
                                      vector<vector<bool> >& tableroExaminado3, vector<vector<bool> >& tableroExaminado4);

/* Busca Rectas Horizontales de un mismo jugador que no esten cortadas por casillas vacias ni fichas del otro jugador, Correspondiente al criterio 1 */
void chequearRectasHorizontales(const int & f, const int & c, const game_rules & this_game,
                                const vector<vector<string> > & board, vector<int>& cant_rectas_longitud,
                                vector<int>& cant_rectas_no_tapadas_oponente_longitud,
                                vector<int>& cant_rectas_no_tapadas_dosExtremos_oponente_longitud, vector<vector<bool> >& tableroExaminado);

/* Busca Rectas Verticales de un mismo jugador que no esten cortadas por casillas vacias ni fichas del otro jugador, Correspondiente al criterio 1 */
void chequearRectasVerticales(const int & f, const int & c, const game_rules & this_game,
                              const vector<vector<string> > & board, vector<int>& cant_rectas_longitud,
                              vector<int>& cant_rectas_no_tapadas_oponente_longitud,
                              vector<int>& cant_rectas_no_tapadas_dosExtremos_oponente_longitud, vector<vector<bool> >& tableroExaminado2);


/* Busca Rectas Diagonales Positivas y Negativas de un mismo jugador que no esten cortadas por casillas vacias ni fichas del otro jugador, Correspondiente al criterio 1 */
void chequearRectasDiagonales(const int & f, const int & c, const game_rules & this_game,
                              const vector<vector<string> > & board, vector<int>& cant_rectas_longitud,
                              vector<int>& cant_rectas_no_tapadas_oponente_longitud,
                              vector<int>& cant_rectas_no_tapadas_dosExtremos_oponente_longitud, vector<vector<bool> >& tableroExaminado3,
                              vector<vector<bool> >& tableroExaminado4);

/* Busca Rectas Horizontales que a lo sumo tengan un espacio vacío y tengan al menos C-1 fichas, Correspondiente al criterio 4 *//*
void chequearRectasHorizontalesGanadoras(const int & f, const int & c, const game_rules & this_game,
                                         const vector<vector<string> > & board, int& cantRectasGanadoras,
                                         vector<vector<bool> >& tableroExaminado);

*//* Busca Rectas Diagonales Positivas y Negativas que a lo sumo tengan un espacio vacío y tengan al menos C-1 fichas, Correspondiente al criterio 4 *//*
void chequearRectasDiagonalesGanadoras(const int & f, const int & c, const game_rules & this_game,
                                       const vector<vector<string> > & board, int& cantRectasGanadoras,
                                       vector<vector<bool> >& tableroExaminado2, vector<vector<bool> >& tableroExaminado3);

*//* Busca Rectas Horizontales que a lo sumo tengan un espacio vacío y tengan al menos C-1 fichas, Correspondiente al criterio 4 *//*
void chequearRectasHorizontalesGanadoras(const int & f, const int & c, const game_rules & this_game,
                                         const vector<vector<string> > & board, int& cantRectasGanadoras,
                                         vector<vector<bool> >& tableroExaminado);*/
//Modificado Nahuel--------------------------------------------
/* Busca Rectas Diagonales Positivas que a lo sumo tengan un espacio vacío y tengan al menos C-1 fichas, Correspondiente al criterio 4 */
void chequearRectasDiagonalPGanadoras(const int & f, const int & c, const game_rules & this_game,
                                      const vector<vector<string> > & board, int& cantRectasGanadoras,
                                      vector<vector<bool> >& tableroExaminado);

/* Busca Rectas Diagonales Negativas que a lo sumo tengan un espacio vacío y tengan al menos C-1 fichas, Correspondiente al criterio 4 */
void chequearRectasDiagonalNGanadoras(const int & f, const int & c, const game_rules & this_game,
                                      const vector<vector<string> > & board, int& cantRectasGanadoras,
                                      vector<vector<bool> >& tableroExaminado);
//--------------------------------------------------------------
/* Calcula si hay al menos una recta del criterio 4, en cuyo caso devuelve true*/
bool oponenteVaAGanar(const game_rules & this_game, const vector<vector<string> > & board);

/* Valuado de la jugada en O(NM) con el criterio 1 y 3 */
double valuarJugada(const game_rules & this_game, const vector<vector<string> > & board, const int & move,
                    const vector<int> & C_parametros, const vector<int> & C_parametros_oponente,
                    const vector<int> & C_parametros_oponente_doble,
                    const vector<int> & C_parametros_peso_particionar);

/* Valuado de la jugada en O(NM) con el criterio 2 y 3 */
double valuarJugada2(const game_rules & this_game, const vector<vector<string> > & board, const int & move,
                     const vector<int> & peso_mis_jugadas, const vector<int> & peso_jugadas_contrincante,
                     const vector<int> & C_parametros_peso_particionar);

/* Valuado de la jugada en O(NM) con el criterio 1, 2 y 3 */
double valuarJugada3(const game_rules & this_game, const vector<vector<string> > & board, const int & move,
                     const vector<int> & peso_mis_jugadas, const vector<int> & peso_jugadas_contrincante,
                     const vector<int> & C_parametros, const vector<int> & C_parametros_oponente,
                     const vector<int> & C_parametros_oponente_doble,
                     const vector<int> & C_parametros_peso_particionar);

/* Valuado de la jugada en O(NM) con el criterio 1, 2, 3 y 4 */
double valuarJugada4(const game_rules & this_game, const vector<vector<string> > & board, const int & move,
                     const vector<int> & peso_mis_jugadas, const vector<int> & peso_jugadas_contrincante,
                     const vector<int> & C_parametros, const vector<int> & C_parametros_oponente,
                     const vector<int> & C_parametros_oponente_doble,
                     const vector<int> & C_parametros_peso_particionar, const int & parametro_oponente_gana);
/* Realiza la mejor jugada posible evaluando con valuarJugada las m jugadas posibles, en caso de haber más de una iguales, toma una de ellas aleatoriamente.
 * Usa 4C parametros*/
int jugadaGolosa(const game_rules & this_game, vector<vector<string> > board, const vector<int> & C_parametros,
                 const vector<int> & C_parametros_oponente, const vector<int> & C_parametros_oponente_doble,
                 const vector<int> & C_parametros_peso_particionar);

/* Realiza la mejor jugada posible evaluando con valuarJugada2 las m jugadas posibles, en caso de haber más de una iguales, toma una de ellas aleatoriamente.
 * Usa 3C parametros*/
int jugadaGolosa2(const game_rules & this_game, vector<vector<string> > board,
                  const vector<int> & peso_mis_jugadas, const vector<int> & peso_jugadas_contrincante,
                  const vector<int> & C_parametros_peso_particionar);

/* Realiza la mejor jugada posible evaluando con valuarJugada3 las m jugadas posibles, en caso de haber más de una iguales, toma una de ellas aleatoriamente.
 * Usa 6C parametros*/
int jugadaGolosa3(const game_rules & this_game, vector<vector<string> > board,
                  const vector<int> & C_parametros, const vector<int> & C_parametros_oponente, const vector<int> & C_parametros_oponente_doble,
                  const vector<int> & peso_mis_jugadas, const vector<int> & peso_jugadas_contrincante,
                  const vector<int> & C_parametros_peso_particionar);

/* Realiza la mejor jugada posible evaluando con valuarJugada4 las m jugadas posibles, en caso de haber más de una iguales, toma una de ellas aleatoriamente.
 * Usa 6C + 1 parámetro*/
int jugadaGolosa4(const game_rules & this_game, vector<vector<string> > board,
                  const vector<int> & C_parametros, const vector<int> & C_parametros_oponente, const vector<int> & C_parametros_oponente_doble,
                  const vector<int> & peso_mis_jugadas, const vector<int> & peso_jugadas_contrincante,
                  const vector<int> & C_parametros_peso_particionar,
                  const int & parametro_oponente_gana);

/* Jugador Goloso que utiliza el primero Conjunto de Criterios segun jugadaGolosa, y juega en su turno basado en el random player de los profes. */
void jugadorGoloso1();

/* Jugador Goloso que utiliza el segundo Conjunto de Criterios segun jugadaGolosa2, y juega en su turno basado en el random player de los profes. */
void jugadorGoloso2();

/* Jugador Goloso que utiliza el tercero Conjunto de Criterios segun jugadaGolosa3, y juega en su turno basado en el random player de los profes. */
void jugadorGoloso3();

/* Jugador Goloso que utiliza el cuarto Conjunto de Criterios segun jugadaGolosa4, y juega en su turno basado en el random player de los profes. */
void jugadorGoloso4();

#endif //ALGO_3_TP3_JUGADORGOLOSO_H
