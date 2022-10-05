#include "JugadorGoloso.h"
#include <chrono>

std::random_device rd2;
std::mt19937 generator2(rd2());


struct jugador_goloso {
    game_rules reglas_aprendidas;
    vector<vector<int> > parametrosVectoriales; // Vectores con todos los parametros preferidos por este jugador
    vector<int> parametrosIndividuales; // Parametros que no se encuentran en un vector;

    bool usaElCriterio1 = true; // Si se usa, se debe colocar los vectores de parametros en parametrosVectorial en primer lugar.
    bool usaElCriterio2 = true; // Si se usa, estos parametros deben colocarse en segundo lugar en parametrosVectorial.
    bool usaElCriterio3 = true; // Si se usa, estos parametros van en tercer lugar en parametrosVectorial.
    bool usaElCriterio4 = true; // Si se usa, parametrosIndividuales requiereSuValor.

    int partidas_ganadas = 0;
    int partidas_empatadas = 0;
    int partidas_perdidas = 0;

    int partidas_ganadas_vs_random = 0;
    int partidas_empatadas_vs_random = 0;
    int partidas_perdidas_vs_random = 0;

    int puntos_clasificacion_actual = 0;
};


//Tomo el jugador que pensamos matematicamente y lo agrego manualmente, despues creo 255 jugadores random

vector<jugador_goloso> CrearJugadoresTorneoDeGrupos(const game_rules & reglas_a_aprender = game_rules(),
        bool usaElCriterio1 = true, bool usaElCriterio2 = true, bool usaElCriterio3 = true, bool usaElCriterio4 = true,
        bool JugadorPrehechoMatematicamente = true){

    vector<jugador_goloso> JugadoresTorneo;
    if(JugadorPrehechoMatematicamente) {
        vector<int> mi_peso(reglas_a_aprender.c);
        vector<int> peso_contrincante(reglas_a_aprender.c);
        vector<int> C_parametros(reglas_a_aprender.c);
        vector<int> C_parametros_oponente(reglas_a_aprender.c);
        vector<int> C_parametros_oponente_doble(reglas_a_aprender.c);
        vector<int> C_parametros_peso_particionar(reglas_a_aprender.c);
        double golden_ratio = (1.0+pow(5.0,1.0/2.0))/2.0;
        double square_root_of_2 = pow(2.0,1.0/2.0);
        int parametro_oponente_gana = static_cast<int>(-pow(2.0, static_cast<double>(reglas_a_aprender.c+1.0))*500.0);

        for(int i = 0; i < reglas_a_aprender.c; i++) {
            mi_peso[i] = static_cast<int>(pow(2.0, static_cast<double>(i))*500.0);
            peso_contrincante[i] = static_cast<int>(-pow(2.0, static_cast<double>(i+1.0))*500.0);
            C_parametros[i] = static_cast<int>(pow(2.0, static_cast<double>(i))*500.0);
            C_parametros_oponente[i] = static_cast<int>(-pow(2.0, static_cast<double>(i+1.0))*500.0);
            C_parametros_oponente_doble[i] = static_cast<int>(-pow(2.0, static_cast<double>(i+1.0))*500.0);
            C_parametros_peso_particionar[i] = static_cast<int>(pow(golden_ratio, static_cast<double>(i))*500.0);
        }
        C_parametros[reglas_a_aprender.c-1] = static_cast<int>(pow(2.0, static_cast<double>(reglas_a_aprender.c+2.0))*500.0);

        jugador_goloso JugadorActual;

        JugadorActual.reglas_aprendidas = reglas_a_aprender;
        JugadorActual.usaElCriterio1 = usaElCriterio1;
        JugadorActual.usaElCriterio2 = usaElCriterio2;
        JugadorActual.usaElCriterio3 = usaElCriterio3;
        JugadorActual.usaElCriterio4 = usaElCriterio4;
        if(usaElCriterio1) {
            JugadorActual.parametrosVectoriales.push_back(C_parametros);
            JugadorActual.parametrosVectoriales.push_back(C_parametros_oponente);
            JugadorActual.parametrosVectoriales.push_back(C_parametros_oponente_doble);
        }
        if(usaElCriterio2) {
            JugadorActual.parametrosVectoriales.push_back(mi_peso);
            JugadorActual.parametrosVectoriales.push_back(peso_contrincante);
        }
        if(usaElCriterio3) {
            JugadorActual.parametrosVectoriales.push_back(C_parametros_peso_particionar);
        }
        if(usaElCriterio4) {
            JugadorActual.parametrosIndividuales.push_back(parametro_oponente_gana);
        }
        JugadoresTorneo.push_back(JugadorActual);
        for(int i = 0; i<255; i++) {
            JugadorActual.parametrosVectoriales = vector<vector<int> >();
            JugadorActual.parametrosIndividuales = vector<int>();
            uniform_int_distribution<int> random_positive(0, int(pow(2.0,15.0)));
            uniform_int_distribution<int> random_negative(int(-pow(2.0,15.0)), 0);
            parametro_oponente_gana = random_negative(generator2);

            for(int j = 0; j < reglas_a_aprender.c; j++) {
                mi_peso[j] = random_positive(generator2);
                peso_contrincante[j] = random_negative(generator2);
                C_parametros[j] = random_positive(generator2);
                C_parametros_oponente[j] = random_negative(generator2);
                C_parametros_oponente_doble[j] = random_negative(generator2);
                C_parametros_peso_particionar[j] = random_positive(generator2);

            }

            JugadorActual.reglas_aprendidas = reglas_a_aprender;
            JugadorActual.usaElCriterio1 = usaElCriterio1;
            JugadorActual.usaElCriterio2 = usaElCriterio2;
            JugadorActual.usaElCriterio3 = usaElCriterio3;
            JugadorActual.usaElCriterio4 = usaElCriterio4;
            if(usaElCriterio1) {
                JugadorActual.parametrosVectoriales.push_back(C_parametros);
                JugadorActual.parametrosVectoriales.push_back(C_parametros_oponente);
                JugadorActual.parametrosVectoriales.push_back(C_parametros_oponente_doble);
            }
            if(usaElCriterio2) {
                JugadorActual.parametrosVectoriales.push_back(mi_peso);
                JugadorActual.parametrosVectoriales.push_back(peso_contrincante);
            }
            if(usaElCriterio3) {
                JugadorActual.parametrosVectoriales.push_back(C_parametros_peso_particionar);
            }
            if(usaElCriterio4) {
                JugadorActual.parametrosIndividuales.push_back(parametro_oponente_gana);
            }
            JugadoresTorneo.push_back(JugadorActual);
        }
    } else {
        jugador_goloso JugadorActual;
        vector<int> mi_peso(reglas_a_aprender.c);
        vector<int> peso_contrincante(reglas_a_aprender.c);
        vector<int> C_parametros(reglas_a_aprender.c);
        vector<int> C_parametros_oponente(reglas_a_aprender.c);
        vector<int> C_parametros_oponente_doble(reglas_a_aprender.c);
        vector<int> C_parametros_peso_particionar(reglas_a_aprender.c);
        int parametro_oponente_gana;


        for(int i = 0; i<256; i++) {
            JugadorActual.parametrosVectoriales = vector<vector<int> >();
            JugadorActual.parametrosIndividuales = vector<int>();
            uniform_int_distribution<int> random_positive(0, int(pow(2.0,15.0)));
            uniform_int_distribution<int> random_negative(int(-pow(2.0,15.0)), 0);
            parametro_oponente_gana = random_negative(generator2);

            for(int j = 0; j < reglas_a_aprender.c; j++) {
                mi_peso[j] = random_positive(generator2);
                peso_contrincante[j] = random_negative(generator2);
                C_parametros[j] = random_positive(generator2);
                C_parametros_oponente[j] = random_negative(generator2);
                C_parametros_oponente_doble[j] = random_negative(generator2);
                C_parametros_peso_particionar[j] = random_positive(generator2);

            }

            JugadorActual.reglas_aprendidas = reglas_a_aprender;
            JugadorActual.usaElCriterio1 = usaElCriterio1;
            JugadorActual.usaElCriterio2 = usaElCriterio2;
            JugadorActual.usaElCriterio3 = usaElCriterio3;
            JugadorActual.usaElCriterio4 = usaElCriterio4;
            if(usaElCriterio1) {
                JugadorActual.parametrosVectoriales.push_back(C_parametros);
                JugadorActual.parametrosVectoriales.push_back(C_parametros_oponente);
                JugadorActual.parametrosVectoriales.push_back(C_parametros_oponente_doble);
            }
            if(usaElCriterio2) {
                JugadorActual.parametrosVectoriales.push_back(mi_peso);
                JugadorActual.parametrosVectoriales.push_back(peso_contrincante);
            }
            if(usaElCriterio3) {
                JugadorActual.parametrosVectoriales.push_back(C_parametros_peso_particionar);
            }
            if(usaElCriterio4) {
                JugadorActual.parametrosIndividuales.push_back(parametro_oponente_gana);
            }
            JugadoresTorneo.push_back(JugadorActual);
        }
    }

    return JugadoresTorneo;
}


// Genero los hijos de los 4 ganadores del torneo tomando las 256 distintas formas de elegir los C/4 primeros, segundos
// terceros y cuartos parametros del primero, segundo, tercero y cuarto mejor jugador (4x4x4x4=256 jugadores)
// Además hago pequeñas mutaciones haciendo una potencia por un numero entre 1/3 y 3 (con media en 1)
// La mutacion es un numero entre 0 y 1 que determina cuanto se muta, 1 significa que puede mutarse el rango completo,
// 0 significa que no hay mutacion, cualquier valor en el medio seria un porcentaje.
vector<jugador_goloso> GenerarHijos(const vector<jugador_goloso> & Grupo_Ganador, double porcentajeMutacion, double probabilidadMutar) {
    game_rules reglas_usadas = Grupo_Ganador[0].reglas_aprendidas;
    vector<jugador_goloso> nuevosJugadoresTorneo;
    uniform_real_distribution<double> mutationChance(0,1);

    //uniform_int_distribution<int> random_integer(INT32_MIN, INT32_MAX);

    for (int i = 0; i < 4; ++i) { //Primer cuarto
        for (int j = 0; j < 4; ++j) { //Segundo cuarto
            for (int k = 0; k < 4; ++k) { //Tercer cuarto
                for (int l = 0; l < 4; ++l) { //Cuarto cuarto
                    jugador_goloso nuevoJugador;

                    nuevoJugador.usaElCriterio1 = Grupo_Ganador[0].usaElCriterio1;
                    nuevoJugador.usaElCriterio2 = Grupo_Ganador[0].usaElCriterio2;
                    nuevoJugador.usaElCriterio3 = Grupo_Ganador[0].usaElCriterio3;
                    nuevoJugador.usaElCriterio4 = Grupo_Ganador[0].usaElCriterio4;
                    nuevoJugador.reglas_aprendidas = Grupo_Ganador[0].reglas_aprendidas;
                    if(Grupo_Ganador[0].usaElCriterio1){
                        nuevoJugador.parametrosVectoriales.emplace_back(vector<int>(reglas_usadas.c));
                        nuevoJugador.parametrosVectoriales.emplace_back(vector<int>(reglas_usadas.c));
                        nuevoJugador.parametrosVectoriales.emplace_back(vector<int>(reglas_usadas.c));
                    }
                    if(Grupo_Ganador[0].usaElCriterio2){
                        nuevoJugador.parametrosVectoriales.emplace_back(vector<int>(reglas_usadas.c));
                        nuevoJugador.parametrosVectoriales.emplace_back(vector<int>(reglas_usadas.c));
                    }
                    if(Grupo_Ganador[0].usaElCriterio3){
                        nuevoJugador.parametrosVectoriales.emplace_back(vector<int>(reglas_usadas.c));
                    }
                    if(Grupo_Ganador[0].usaElCriterio4){
                        nuevoJugador.parametrosIndividuales.emplace_back(int());
                    }
                    for (int m = 0; m < reglas_usadas.c/4; ++m) {
                        for (size_t n = 0; n < nuevoJugador.parametrosVectoriales.size(); ++n) {
                            nuevoJugador.parametrosVectoriales[n][m] = Grupo_Ganador[i].parametrosVectoriales[n][m];
                        }
                    }
                    for (int m = reglas_usadas.c/4; m < reglas_usadas.c/2; ++m) {
                        for (size_t n = 0; n < nuevoJugador.parametrosVectoriales.size(); ++n) {
                            nuevoJugador.parametrosVectoriales[n][m] = Grupo_Ganador[j].parametrosVectoriales[n][m];
                        }
                    }
                    for (int m = reglas_usadas.c/2; m < reglas_usadas.c/2 + reglas_usadas.c/4; ++m) {
                        for (size_t n = 0; n < nuevoJugador.parametrosVectoriales.size(); ++n) {
                            nuevoJugador.parametrosVectoriales[n][m] = Grupo_Ganador[k].parametrosVectoriales[n][m];
                        }
                    }
                    for (int m = reglas_usadas.c/2 + reglas_usadas.c/4; m < reglas_usadas.c; ++m) {
                        for (size_t n = 0; n < nuevoJugador.parametrosVectoriales.size(); ++n) {
                            nuevoJugador.parametrosVectoriales[n][m] = Grupo_Ganador[l].parametrosVectoriales[n][m];
                        }
                        if(Grupo_Ganador[0].usaElCriterio4) {
                            if(Grupo_Ganador[0].usaElCriterio1) {
                                nuevoJugador.parametrosIndividuales[0] = Grupo_Ganador[l].parametrosIndividuales[0];
                            }
                        }
                    }

                    //Mutaciones
                    if(i != j or j != k or k != l) { //Mutamos si no es alguno de los ganadores, si es uno de los ganadores lo dejamos igual
                        int m = 0;
                        if(nuevoJugador.usaElCriterio1) {

                            int range_parameters = abs(*max_element(nuevoJugador.parametrosVectoriales[m].begin(), nuevoJugador.parametrosVectoriales[m].end())
                                    - *min_element(nuevoJugador.parametrosVectoriales[m].begin(),nuevoJugador.parametrosVectoriales[m].end()));
                            uniform_int_distribution<int> random_integer(-range_parameters-1, range_parameters+1);

                            int range_parameters2 = abs(*max_element(nuevoJugador.parametrosVectoriales[m+1].begin(), nuevoJugador.parametrosVectoriales[m+1].end())
                                                       - *min_element(nuevoJugador.parametrosVectoriales[m+1].begin(),nuevoJugador.parametrosVectoriales[m+1].end()));
                            uniform_int_distribution<int> random_integer2(-range_parameters2-1, range_parameters2+1);

                            int range_parameters3 = abs(*max_element(nuevoJugador.parametrosVectoriales[m+2].begin(), nuevoJugador.parametrosVectoriales[m+2].end())
                                                       - *min_element(nuevoJugador.parametrosVectoriales[m+2].begin(),nuevoJugador.parametrosVectoriales[m+2].end()));
                            uniform_int_distribution<int> random_integer3(-range_parameters3-1, range_parameters3+1);

                            for (size_t n = 0; n < nuevoJugador.parametrosVectoriales[m].size(); ++n) {

                                int mutate = mutationChance(generator2) <= probabilidadMutar? 1 : 0; //Si el numero aleatorio da menor o igual entonces muto, sino no.

                                long int parametroMutado = nuevoJugador.parametrosVectoriales[m][n] + static_cast<int>
                                (random_integer(generator2) * porcentajeMutacion * mutate);
                                if (parametroMutado > long(pow(2.0,15.0))) {// Primer vector es CParametros Mios
                                    parametroMutado = long(pow(2.0,15.0));
                                }
                                if (parametroMutado < 0) {
                                    parametroMutado = 0;
                                }
                                nuevoJugador.parametrosVectoriales[m][n] = parametroMutado;

                                mutate = mutationChance(generator2) <= probabilidadMutar? 1 : 0; //Si el numero aleatorio da menor o igual entonces muto, sino no.

                                long int parametroMutado2 = nuevoJugador.parametrosVectoriales[m+1][n] + static_cast<int>
                                (random_integer2(generator2) * porcentajeMutacion * mutate);
                                if (parametroMutado2 > 0) {// Segundo vector es CParametros Oponente
                                    parametroMutado2 = 0;
                                }
                                if (parametroMutado2 < long(-pow(2.0,15.0))) {
                                    parametroMutado2 = long(-pow(2.0,15.0));
                                }
                                nuevoJugador.parametrosVectoriales[m+1][n] = parametroMutado2;

                                mutate = mutationChance(generator2) <= probabilidadMutar? 1 : 0; //Si el numero aleatorio da menor o igual entonces muto, sino no.

                                long int parametroMutado3 = nuevoJugador.parametrosVectoriales[m+2][n] + static_cast<int>
                                (random_integer3(generator2) * porcentajeMutacion * mutate);
                                if (parametroMutado3 > 0) { // Tercer vector es CParametros Oponente Doble
                                    parametroMutado3 = 0;
                                }
                                if (parametroMutado3 < long(-pow(2.0,15.0))) {
                                    parametroMutado3 = long(-pow(2.0,15.0));
                                }

                                nuevoJugador.parametrosVectoriales[m+2][n] = parametroMutado3;
                            }
                            m+=3; //Hice los primeros tres vectores
                        }
                        if(nuevoJugador.usaElCriterio2) {

                            int range_parameters = abs(*max_element(nuevoJugador.parametrosVectoriales[m].begin(), nuevoJugador.parametrosVectoriales[m].end())
                                                   - *min_element(nuevoJugador.parametrosVectoriales[m].begin(),nuevoJugador.parametrosVectoriales[m].end()));
                            uniform_int_distribution<int> random_integer(-range_parameters-1, range_parameters+1);

                            int range_parameters2 = abs(*max_element(nuevoJugador.parametrosVectoriales[m+1].begin(), nuevoJugador.parametrosVectoriales[m+1].end())
                                                       - *min_element(nuevoJugador.parametrosVectoriales[m+1].begin(),nuevoJugador.parametrosVectoriales[m+1].end()));
                            uniform_int_distribution<int> random_integer2(-range_parameters2-1, range_parameters2+1);

                            for (size_t n = 0; n < nuevoJugador.parametrosVectoriales[m].size(); ++n) {

                                int mutate = mutationChance(generator2) <= probabilidadMutar? 1 : 0; //Si el numero aleatorio da menor o igual entonces muto, sino no.

                                long int parametroMutado = nuevoJugador.parametrosVectoriales[m][n] + static_cast<int>
                                (random_integer(generator2) * porcentajeMutacion * mutate);
                                if (parametroMutado > long(pow(2.0,15.0))) {// Primer vector es MiPeso
                                    parametroMutado = long(pow(2.0,15.0));
                                }
                                if (parametroMutado < 0) {
                                    parametroMutado = 0;
                                }
                                nuevoJugador.parametrosVectoriales[m][n] = parametroMutado;

                                mutate = mutationChance(generator2) <= probabilidadMutar? 1 : 0; //Si el numero aleatorio da menor o igual entonces muto, sino no.

                                long int parametroMutado2 = nuevoJugador.parametrosVectoriales[m+1][n] + static_cast<int>
                                (random_integer2(generator2) * porcentajeMutacion * mutate);
                                if (parametroMutado2 > 0) {// Segundo vector es PesoOponente
                                    parametroMutado2 = 0;
                                }
                                if (parametroMutado2 < long(-pow(2.0,15.0))) {
                                    parametroMutado2 = long(-pow(2.0,15.0));
                                }
                                nuevoJugador.parametrosVectoriales[m+1][n] = parametroMutado2;
                            }
                            m+=2; //Hice los vectores del segundo criterio
                        }
                        if(nuevoJugador.usaElCriterio3) {

                            int range_parameters = abs(*max_element(nuevoJugador.parametrosVectoriales[m].begin(), nuevoJugador.parametrosVectoriales[m].end())
                                                       - *min_element(nuevoJugador.parametrosVectoriales[m].begin(),nuevoJugador.parametrosVectoriales[m].end()));
                            uniform_int_distribution<int> random_integer(-range_parameters-1, range_parameters+1);

                            for (size_t n = 0; n < nuevoJugador.parametrosVectoriales[m].size(); ++n) {

                                int mutate = mutationChance(generator2) <= probabilidadMutar? 1 : 0; //Si el numero aleatorio da menor o igual entonces muto, sino no.

                                long int parametroMutado = nuevoJugador.parametrosVectoriales[m][n] + static_cast<int>
                                (random_integer(generator2) * porcentajeMutacion * mutate);
                                if (parametroMutado > long(pow(2.0,15.0))) {// El vector es de CParametrosParticiones
                                    parametroMutado = long(pow(2.0,15.0));
                                }
                                if (parametroMutado < 0) {
                                    parametroMutado = 0;
                                }
                                nuevoJugador.parametrosVectoriales[m][n] = parametroMutado;
                            }
                            m+=1; //Hice el vector del tercer criterio
                        }
                        if(nuevoJugador.usaElCriterio4) { //Suponemos que se usa ademas el criterio 1

                            int range_parameters = abs(*max_element(nuevoJugador.parametrosVectoriales[1].begin(), nuevoJugador.parametrosVectoriales[1].end())
                                                       - *min_element(nuevoJugador.parametrosVectoriales[1].begin(),nuevoJugador.parametrosVectoriales[1].end())); //Uso el rango de las rectas del oponente del criterio 1, con el fin de que varie con la misma proporción (ya que es del oponente la ganadora del criterio 4)
                            uniform_int_distribution<int> random_integer(-range_parameters-1, range_parameters+1);

                            int mutate = mutationChance(generator2) <= probabilidadMutar? 1 : 0; //Si el numero aleatorio da menor o igual entonces muto, sino no.

                            long int parametroMutado = nuevoJugador.parametrosIndividuales[0] + static_cast<int>
                            (random_integer(generator2) * porcentajeMutacion * mutate);
                            if (parametroMutado > 0) {// El vector es de CParametrosParticiones
                                parametroMutado = 0;
                            }
                            if (parametroMutado < long(-pow(2.0,15.0))) {
                                parametroMutado = long(-pow(2.0,15.0));
                            }
                            nuevoJugador.parametrosIndividuales[0] = parametroMutado;
                        }
                    }


                    nuevosJugadoresTorneo.push_back(nuevoJugador);
                }
            }
        }
    }

    return nuevosJugadoresTorneo;
}

void chequearRectasHorizontalesC(const int & f, const int & c, const game_rules & this_game,
                                const vector<vector<string> > & board, bool & hayRectasGanadoras, int & JugadorGanador,
                                 vector<vector<bool> >& tableroExaminado) {

    if(board[f][c] != "-") { //Si hay alguna jugada en la casilla.
        int c2;
        for(c2 = c; c2 < this_game.columns and board[f][c2] == board[f][c]; c2++){
            tableroExaminado[f][c2] = true;
        }
        int c3;
        for(c3 = c; c3 > -1 and board[f][c3] == board[f][c]; c3--){
            tableroExaminado[f][c3] = true;
        }
        if (c2-c3-1 >= this_game.c) {//c3 y c2 estan en los bordes por fuera de la Recta, por tanto la recta mide c2-c3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0)
            hayRectasGanadoras = true;
        }
        if(hayRectasGanadoras) {
            JugadorGanador = (this_game.color == board[f][c] ? 1 : 2);
        }
    }

}

void chequearRectasVerticalesC(const int & f, const int & c, const game_rules & this_game,
                              const vector<vector<string> > & board, bool & hayRectasGanadoras, int & JugadorGanador,
                               vector<vector<bool> >& tableroExaminado2) {

    if(board[f][c] != "-") { //Si hay alguna jugada en la casilla.
        int f2;
        for(f2 = f; f2 < this_game.rows and board[f2][c] == board[f][c]; f2++){
            tableroExaminado2[f2][c] = true;
        }
        int f3;
        for(f3 = f; f3 > -1 and board[f3][c] == board[f][c]; f3--){
            tableroExaminado2[f3][c] = true;
        }
        if (f2-f3-1 >= this_game.c) {//f3 y f2 estan en los bordes por fuera de la Recta, por tanto la recta mide f2-f3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0)
            hayRectasGanadoras = true;
        }
        if(hayRectasGanadoras) {
            JugadorGanador = (this_game.color == board[f][c] ? 1 : 2);
        }

    }
}

void chequearRectasDiagonalesPositivasC(const int & f, const int & c, const game_rules & this_game,
                              const vector<vector<string> > & board, bool & hayRectasGanadoras, int & JugadorGanador,
                              vector<vector<bool> >& tableroExaminado3) {

    if(board[f][c] != "-") { //Si hay alguna jugada en la casilla.
        int f2;
        int c2;
        for(f2 = f, c2 = c; f2 < this_game.rows and c2 < this_game.columns and board[f2][c2] == board[f][c]; f2++, c2++){
            tableroExaminado3[f2][c2] = true;
        }
        int f3;
        int c3;
        for(f3 = f, c3 = c; f3 > -1 and c3 > -1 and board[f3][c3] == board[f][c]; f3--, c3--){
            tableroExaminado3[f3][c3] = true;
        }
        if (f2-f3-1 >= this_game.c) {//c3 y c2 estan en los bordes por fuera de la Recta, por tanto la recta mide c2-c3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0)
            hayRectasGanadoras = true;
        }
        if(hayRectasGanadoras) {
            JugadorGanador = (this_game.color == board[f][c] ? 1 : 2);
        }
    }
}

void chequearRectasDiagonalesNegativasC(const int & f, const int & c, const game_rules & this_game,
                               const vector<vector<string> > & board, bool & hayRectasGanadoras, int & JugadorGanador,
                               vector<vector<bool> >& tableroExaminado4) {

    if(board[f][c] != "-") { //Si hay alguna jugada en la casilla.
        int f2;
        int c2;
        int f3;
        int c3;

        //Lo rehacemos pero para la diagonal de la esquina superior derecha a la inferior izquierda.
        //Fila aumenta, columna disminuye.
        for(f2 = f, c2 = c; f2 < this_game.rows and c2 > -1 and board[f2][c2] == board[f][c]; f2++, c2--){
            tableroExaminado4[f2][c2] = true;
        }

        //Fila disminuye, columna aumenta.
        for(f3 = f, c3 = c; f3 > -1 and c3 < this_game.columns and board[f3][c3] == board[f][c]; f3--, c3++){
            tableroExaminado4[f3][c3] = true;
        }
        if (f2-f3-1 >= this_game.c) {//c3 y c2 estan en los bordes por fuera de la Recta, por tanto la recta mide c2-c3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0)
            hayRectasGanadoras = true;
        }
        if(hayRectasGanadoras) {
            JugadorGanador = this_game.color == board[f][c] ? 1 : 2;
        }
    }
}

bool chequearHayGanadores(const vector<vector<string> > &board, const game_rules &reglas, int &JugadorGanador) {

    bool hayRectasQueGanaron = false;

    vector<vector<bool> > tableroExaminadoH(reglas.rows, vector<bool>(reglas.columns, false)); //Todavia no examine el tablero horizontalmente.
    vector<vector<bool> > tableroExaminadoV(reglas.rows, vector<bool>(reglas.columns, false)); //Todavia no examine el tablero verticalmente.
    vector<vector<bool> > tableroExaminadoDP(reglas.rows, vector<bool>(reglas.columns, false)); //Todavia no examine el tablero diagonalmente superior izquierda inferior derecha.
    vector<vector<bool> > tableroExaminadoDN(reglas.rows, vector<bool>(reglas.columns, false)); //Todavia no examine el tablero diagonalmente superior derecha inferior izquierda.


    for(int f = 0; f < reglas.rows and !hayRectasQueGanaron; f++) {
        for(int c = 0; c < reglas.columns and !hayRectasQueGanaron; c++) {//Si una recta no esta examinada entonces AL MENOS una celda de la misma no fue examinada.
            if (!hayRectasQueGanaron and !tableroExaminadoH[f][c]){
                chequearRectasHorizontalesC(f, c, reglas, board, hayRectasQueGanaron, JugadorGanador, tableroExaminadoH);
                tableroExaminadoH[f][c] = true;
            }
            if (!hayRectasQueGanaron and !tableroExaminadoV[f][c]) {
                chequearRectasVerticalesC(f, c, reglas, board, hayRectasQueGanaron, JugadorGanador, tableroExaminadoV);
                tableroExaminadoV[f][c] = true;
            }
            if (!hayRectasQueGanaron and !tableroExaminadoDP[f][c]) {
                chequearRectasDiagonalesPositivasC(f, c, reglas, board, hayRectasQueGanaron, JugadorGanador, tableroExaminadoDP);
                tableroExaminadoDP[f][c] = true;
            }
            if (!hayRectasQueGanaron and !tableroExaminadoDN[f][c]) {
                chequearRectasDiagonalesNegativasC(f, c, reglas, board, hayRectasQueGanaron, JugadorGanador, tableroExaminadoDN);
                tableroExaminadoDN[f][c] = true;
            }
        }
    }

    return hayRectasQueGanaron;
}

void PartidaRandomVsGoloso(jugador_goloso & Jugador) {
    if(Jugador.usaElCriterio1 and Jugador.usaElCriterio2 and Jugador.usaElCriterio3 and Jugador.usaElCriterio4) {

        vector<int> Criterio1A = Jugador.parametrosVectoriales[0];
        vector<int> Criterio1B = Jugador.parametrosVectoriales[1];
        vector<int> Criterio1C = Jugador.parametrosVectoriales[2];
        vector<int> Criterio2A = Jugador.parametrosVectoriales[3];
        vector<int> Criterio2B = Jugador.parametrosVectoriales[4];
        vector<int> Criterio3 = Jugador.parametrosVectoriales[5];
        int Criterio4 = Jugador.parametrosIndividuales[0];

        game_rules reglas_Random; // Va a jugar con las reglas del otro jugador
        reglas_Random.color = "rojo";
        reglas_Random.opponent_color = "azul";

        reglas_Random.columns = Jugador.reglas_aprendidas.columns;
        reglas_Random.rows = Jugador.reglas_aprendidas.rows;
        reglas_Random.c = Jugador.reglas_aprendidas.c;
        reglas_Random.p = Jugador.reglas_aprendidas.p;


        game_rules reglas_partida = reglas_Random; // Las reglas de la partida son las mismas que las del jugador 1 ya que el color del 1 es el color y el color del jugador 2 es del oponente del 1.

        /*vector<int> Criterio1A_J2 = Jugador2.parametrosVectoriales[0];
        vector<int> Criterio1B_J2 = Jugador2.parametrosVectoriales[1];
        vector<int> Criterio1C_J2 = Jugador2.parametrosVectoriales[2];
        vector<int> Criterio2A_J2 = Jugador2.parametrosVectoriales[3];
        vector<int> Criterio2B_J2 = Jugador2.parametrosVectoriales[4];
        vector<int> Criterio3_J2 = Jugador2.parametrosVectoriales[5];
        int Criterio4_J2 = Jugador2.parametrosIndividuales[0];*/

        game_rules reglas_Jugador;
        reglas_Jugador.color = "azul";
        reglas_Jugador.opponent_color = "rojo";

        reglas_Jugador.columns = Jugador.reglas_aprendidas.columns;
        reglas_Jugador.rows = Jugador.reglas_aprendidas.rows;
        reglas_Jugador.c = Jugador.reglas_aprendidas.c;
        reglas_Jugador.p = Jugador.reglas_aprendidas.p;

        vector<vector<string> > board(reglas_partida.rows, vector<string>(reglas_partida.columns, "-"));


        bool hayGanador = false;
        int JugadorGanador = -1;
        for (int turno = 0; turno < reglas_partida.p and !hayGanador; ++turno) {
            vector<int> possible_moves;
            for (int i = 0; i < reglas_Random.columns; ++i) {
                if(board[0][i] == "-") { // Si la primera casilla esta vacia, es una movida posible.
                    possible_moves.push_back(i);
                }
            }
            std::uniform_int_distribution<int> do_move(0, possible_moves.size() - 1);
            int movida_random = do_move(generator2);
            modificarTablero(reglas_Random, board, possible_moves[movida_random], true); //modifico el tablero con la jugada del primer jugador

            hayGanador = chequearHayGanadores(board, reglas_partida, JugadorGanador);

            if(!hayGanador){
                int movida_jugador = jugadaGolosa4(reglas_Jugador, board, Criterio1A, Criterio1B, Criterio1C,
                                                    Criterio2A, Criterio2B, Criterio3, Criterio4);
                modificarTablero(reglas_Jugador, board, movida_jugador, true); //modifico el tablero con la jugada del segundo jugador

                hayGanador = chequearHayGanadores(board, reglas_partida, JugadorGanador); //Usa las reglas del jugador 1 para detectar quien es el primero y segundo jugador
            }
        }

        if(!hayGanador) {
            Jugador.partidas_empatadas_vs_random++;
        } else {
            if (JugadorGanador == 1) { // El jugador 1 es el random
                Jugador.partidas_perdidas_vs_random++;
            } else {
                Jugador.partidas_ganadas_vs_random++;
            }
        }

    }
}

void PartidaGolosos(jugador_goloso & Jugador1, jugador_goloso & Jugador2) {
    if(Jugador1.usaElCriterio1 and Jugador1.usaElCriterio2 and Jugador1.usaElCriterio3 and Jugador1.usaElCriterio4) {

        vector<int> Criterio1A_J1 = Jugador1.parametrosVectoriales[0];
        vector<int> Criterio1B_J1 = Jugador1.parametrosVectoriales[1];
        vector<int> Criterio1C_J1 = Jugador1.parametrosVectoriales[2];
        vector<int> Criterio2A_J1 = Jugador1.parametrosVectoriales[3];
        vector<int> Criterio2B_J1 = Jugador1.parametrosVectoriales[4];
        vector<int> Criterio3_J1 = Jugador1.parametrosVectoriales[5];
        int Criterio4_J1 = Jugador1.parametrosIndividuales[0];

        game_rules reglas_Jugador1;
        reglas_Jugador1.color = "rojo";
        reglas_Jugador1.opponent_color = "azul";

        reglas_Jugador1.columns = Jugador1.reglas_aprendidas.columns;
        reglas_Jugador1.rows = Jugador1.reglas_aprendidas.rows;
        reglas_Jugador1.c = Jugador1.reglas_aprendidas.c;
        reglas_Jugador1.p = Jugador1.reglas_aprendidas.p;


        game_rules reglas_partida = reglas_Jugador1; // Las reglas de la partida son las mismas que las del jugador 1 ya que el color del 1 es el color y el color del jugador 2 es del oponente del 1.

        vector<int> Criterio1A_J2 = Jugador2.parametrosVectoriales[0];
        vector<int> Criterio1B_J2 = Jugador2.parametrosVectoriales[1];
        vector<int> Criterio1C_J2 = Jugador2.parametrosVectoriales[2];
        vector<int> Criterio2A_J2 = Jugador2.parametrosVectoriales[3];
        vector<int> Criterio2B_J2 = Jugador2.parametrosVectoriales[4];
        vector<int> Criterio3_J2 = Jugador2.parametrosVectoriales[5];
        int Criterio4_J2 = Jugador2.parametrosIndividuales[0];

        game_rules reglas_Jugador2;
        reglas_Jugador2.color = "azul";
        reglas_Jugador2.opponent_color = "rojo";

        reglas_Jugador2.columns = Jugador2.reglas_aprendidas.columns;
        reglas_Jugador2.rows = Jugador2.reglas_aprendidas.rows;
        reglas_Jugador2.c = Jugador2.reglas_aprendidas.c;
        reglas_Jugador2.p = Jugador2.reglas_aprendidas.p;

        vector<vector<string> > board(reglas_partida.rows, vector<string>(reglas_partida.columns, "-"));


        bool hayGanador = false;
        int JugadorGanador = -1;
        for (int turno = 0; turno < reglas_partida.p and !hayGanador; ++turno) {
            int movida_jugador1 = jugadaGolosa4(reglas_Jugador1, board, Criterio1A_J1, Criterio1B_J1, Criterio1C_J1,
                    Criterio2A_J1, Criterio2B_J1, Criterio3_J1, Criterio4_J1);
            modificarTablero(reglas_Jugador1, board, movida_jugador1, true); //modifico el tablero con la jugada del primer jugador

            hayGanador = chequearHayGanadores(board, reglas_partida, JugadorGanador);

            if(!hayGanador){
                int movida_jugador2 = jugadaGolosa4(reglas_Jugador2, board, Criterio1A_J2, Criterio1B_J2, Criterio1C_J2,
                                                    Criterio2A_J2, Criterio2B_J2, Criterio3_J2, Criterio4_J2);
                modificarTablero(reglas_Jugador2, board, movida_jugador2, true); //modifico el tablero con la jugada del segundo jugador

                hayGanador = chequearHayGanadores(board, reglas_partida, JugadorGanador); //Usa las reglas del jugador 1 para detectar quien es el primero y segundo jugador
            }
        }

        if(!hayGanador) {
            Jugador1.partidas_empatadas++;
            Jugador1.puntos_clasificacion_actual++;
            Jugador2.partidas_empatadas++;
            Jugador2.puntos_clasificacion_actual++;
        } else {
            if (JugadorGanador == 1) {
                Jugador1.partidas_ganadas++;
                Jugador1.puntos_clasificacion_actual += 3;
                Jugador2.partidas_perdidas++;
            } else {
                Jugador2.partidas_ganadas++;
                Jugador2.puntos_clasificacion_actual += 3;
                Jugador1.partidas_perdidas++;
            }
        }

    }
}

void hacerCompetirGrupo(vector<jugador_goloso> & Grupo) {
    for (size_t jugador_1 = 0; jugador_1 < Grupo.size(); ++jugador_1) {
        for (size_t jugador_2 = 0; jugador_2 < Grupo.size(); ++jugador_2) {
            if (jugador_1 != jugador_2) {
                PartidaGolosos(Grupo[jugador_1],Grupo[jugador_2]);
            }
        }
    }
}

vector<jugador_goloso> TorneoDeGrupos(const vector<jugador_goloso> & Jugadores){
    // Tomo los jugadores hijos de los ganadores del anterior torneo o los generados por CrearJugadoresTorneoDeGrupos
    // La idea es que son grupos de 4 y el mejor de cada grupo lo hago pasar al siguiente grupo hasta que quede un
    // ultimo grupo de 4 y me quedo con todos ellos para generar 256 hijos tomando C/4 parametros de los vectores de cada uno en todas las 256 formas. (4x4x4x4)
    vector<vector<jugador_goloso> > Grupos(Jugadores.size()/4);

    for (size_t i = 0; i < Jugadores.size(); ++i) {
        Grupos[i/4].push_back(Jugadores[i]);
    }
    while(Grupos.size() > 1) {
        for (size_t i = 0; i < Grupos.size(); ++i) { //Los hago competir dentro del grupo y guardo sus puntajes
            hacerCompetirGrupo(Grupos[i]);
        }
        vector<jugador_goloso> Ganadores;
        for (size_t i = 0; i < Grupos.size(); ++i) { // Calculo los ganadores de cada grupo
            vector<jugador_goloso> mejoresJugadores;
            int mejorPuntaje = INT32_MIN;
            for (const jugador_goloso & Jugador : Grupos[i]) {
                if(mejorPuntaje < Jugador.puntos_clasificacion_actual) {
                    mejoresJugadores.erase(mejoresJugadores.begin(), mejoresJugadores.end());
                    mejoresJugadores.push_back(Jugador);
                    mejorPuntaje = Jugador.puntos_clasificacion_actual;
                } else if(mejorPuntaje == Jugador.puntos_clasificacion_actual) {
                    mejoresJugadores.push_back(Jugador);
                }
            }
            uniform_int_distribution<int> select_player(0, mejoresJugadores.size() - 1);
            int selected_player = select_player(generator2); //Selecciono random entre cualquiera de los mejores jugadores del grupo con igual puntaje.
            Ganadores.push_back(mejoresJugadores[selected_player]);
        } // Ahora tengo un cuarto de los jugadores que tenia en los Ganadores, los pongo en Grupos de vuelta.
        Grupos = vector<vector<jugador_goloso> >(Ganadores.size()/4);
        for (size_t i = 0; i < Ganadores.size(); ++i) {
            Ganadores[i].puntos_clasificacion_actual = 0;
            Grupos[i/4].push_back(Ganadores[i]);
        }
    }

    return Grupos[0];
}

void torneo_h_horas(int h){
    game_rules reglas_a_perfeccionar;
    cout << "Inserte cantidad de filas:" << endl;
    cin >> reglas_a_perfeccionar.rows;
    cout << "Inserte cantidad de columnas:" << endl;
    cin >> reglas_a_perfeccionar.columns;
    cout << "Inserte valor C:" << endl;
    cin >> reglas_a_perfeccionar.c;
    cout << "Inserte valor P:" << endl;
    cin >> reglas_a_perfeccionar.p;

    chrono::steady_clock::time_point t0;
    chrono::steady_clock::time_point t1;

    t0 = chrono::steady_clock::now();

    vector<jugador_goloso> Jugadores = CrearJugadoresTorneoDeGrupos(reglas_a_perfeccionar);
    vector<jugador_goloso> Ganadores = TorneoDeGrupos(Jugadores);


    t1 = chrono::steady_clock::now();
    chrono::duration<double, std::milli> delta_t = (t1 - t0);
    int Torneos = 1;
    cout << Torneos << endl;


    while(delta_t < chrono::hours(h)) {
        Jugadores = GenerarHijos(Ganadores,0.05, 0.5);
        Ganadores = TorneoDeGrupos(Jugadores);

        t1 = chrono::steady_clock::now();
        delta_t = t1 - t0;
        Torneos++;
        cout << Torneos << endl;
    }
    ofstream bestPlayers("bestPlayerParameters.output");
    for (size_t i = 0; i < Ganadores.size(); ++i) {
        bestPlayers << "Parametros Jugador" << i+1 << ": " << endl;
        for (size_t j = 0; j < Ganadores[i].parametrosVectoriales.size(); ++j) {
            for (size_t k = 0; k < Ganadores[i].parametrosVectoriales[j].size() - 1; ++k) {
                bestPlayers << Ganadores[i].parametrosVectoriales[j][k] << ", ";
            }
            bestPlayers << Ganadores[i].parametrosVectoriales[j][Ganadores[i].parametrosVectoriales[j].size() - 1] << endl;
        }
        bestPlayers << Ganadores[i].parametrosIndividuales[0] << endl << endl;
    }
    bestPlayers.close();
}
vector<jugador_goloso> torneo_h_horas_Inicial(int h, int VecesRealizado, double porcentajeVariacion, double probabilidadMutar, const game_rules & reglas_a_perfeccionar){

    chrono::steady_clock::time_point t0;
    chrono::steady_clock::time_point t1;

    t0 = chrono::steady_clock::now();

    /*
     * vector<jugador_goloso> Jugadores;
     * vector<jugador_goloso> Ganadores = Grupo_entrenado;
     */
    vector<jugador_goloso> Jugadores = CrearJugadoresTorneoDeGrupos(reglas_a_perfeccionar, true, true, true, true, false);
    vector<jugador_goloso> Ganadores = TorneoDeGrupos(Jugadores);


    t1 = chrono::steady_clock::now();
    chrono::duration<double, std::milli> delta_t = (t1 - t0);
    //int Torneos = 1;
    //cout << Torneos << endl;


    while(delta_t < chrono::minutes(h)) {
        Jugadores = GenerarHijos(Ganadores, porcentajeVariacion, probabilidadMutar);
        Ganadores = TorneoDeGrupos(Jugadores);

        t1 = chrono::steady_clock::now();
        delta_t = t1 - t0;
        cout << std::chrono::duration_cast<std::chrono::minutes>(delta_t).count() << endl;
        //Torneos++;
        //cout << Torneos << endl;
    }

    string archivoNum = to_string(VecesRealizado);
    ofstream bestPlayers("./ResultadosTorneoInfinito/Jugadores" + archivoNum + ".output");
    for (size_t i = 0; i < Ganadores.size(); ++i) {
        bestPlayers << "Parametros Jugador" << i+1 << ": " << endl;
        for (size_t j = 0; j < Ganadores[i].parametrosVectoriales.size(); ++j) {
            for (size_t k = 0; k < Ganadores[i].parametrosVectoriales[j].size() - 1; ++k) {
                bestPlayers << Ganadores[i].parametrosVectoriales[j][k] << ", ";
            }
            bestPlayers << Ganadores[i].parametrosVectoriales[j][Ganadores[i].parametrosVectoriales[j].size() - 1] << endl;
        }
        bestPlayers << Ganadores[i].parametrosIndividuales[0] << endl << endl;
    }
    bestPlayers.close();

    ofstream QualityRandom("./ResultadosTorneoInfinito/CalidadVsRandom" + archivoNum + ".output");
    for (size_t i = 0; i < Ganadores.size(); ++i) {
        for (int j = 0; j < 1000; ++j) {
            PartidaRandomVsGoloso(Ganadores[i]);
        }
        QualityRandom << "Jugador N. " << i+1 << endl;
        QualityRandom << "Porcentaje Ganadas: " << double(Ganadores[i].partidas_ganadas_vs_random) * 100.0 / 1000.0 << endl;
        QualityRandom << "Porcentaje Perdidas: " << double(Ganadores[i].partidas_perdidas_vs_random) * 100.0 / 1000.0 << endl << endl;
    }
    QualityRandom.close();


    return Ganadores;
}

vector<jugador_goloso> torneo_h_horas_PreGrupo(int h, const vector<jugador_goloso> & Grupo_entrenado, int VecesRealizado, double porcentajeVariacion,
                                               double probabilidadMutar){

    chrono::steady_clock::time_point t0;
    chrono::steady_clock::time_point t1;

    t0 = chrono::steady_clock::now();

    vector<jugador_goloso> Jugadores;
    vector<jugador_goloso> Ganadores = Grupo_entrenado;
    //vector<jugador_goloso> Jugadores = CrearJugadoresTorneoDeGrupos(reglas_a_perfeccionar);
    //vector<jugador_goloso> Ganadores = TorneoDeGrupos(Jugadores);


    t1 = chrono::steady_clock::now();
    chrono::duration<double, std::milli> delta_t = (t1 - t0);
    //int Torneos = 1;
    //cout << Torneos << endl;


    while(delta_t < chrono::minutes(h)) {
        Jugadores = GenerarHijos(Ganadores, porcentajeVariacion, probabilidadMutar);
        Ganadores = TorneoDeGrupos(Jugadores);

        t1 = chrono::steady_clock::now();
        delta_t = t1 - t0;
        cout << std::chrono::duration_cast<std::chrono::minutes>(delta_t).count() << endl;
        //Torneos++;
        //cout << Torneos << endl;
    }

    string archivoNum = to_string(VecesRealizado);
    ofstream bestPlayers("./ResultadosTorneoInfinito/Jugadores" + archivoNum + ".output");
    for (size_t i = 0; i < Ganadores.size(); ++i) {
        bestPlayers << "Parametros Jugador" << i+1 << ": " << endl;
        for (size_t j = 0; j < Ganadores[i].parametrosVectoriales.size(); ++j) {
            for (size_t k = 0; k < Ganadores[i].parametrosVectoriales[j].size() - 1; ++k) {
                bestPlayers << Ganadores[i].parametrosVectoriales[j][k] << ", ";
            }
            bestPlayers << Ganadores[i].parametrosVectoriales[j][Ganadores[i].parametrosVectoriales[j].size() - 1] << endl;
        }
        bestPlayers << Ganadores[i].parametrosIndividuales[0] << endl << endl;
    }
    bestPlayers.close();

    ofstream QualityRandom("./ResultadosTorneoInfinito/CalidadVsRandom" + archivoNum + ".output");
    for (size_t i = 0; i < Ganadores.size(); ++i) {
        for (int j = 0; j < 1000; ++j) {
            PartidaRandomVsGoloso(Ganadores[i]);
        }
        QualityRandom << "Jugador N. " << i+1 << endl;
        QualityRandom << "Porcentaje Ganadas: " << double(Ganadores[i].partidas_ganadas_vs_random) * 100.0 / 1000.0 << endl;
        QualityRandom << "Porcentaje Perdidas: " << double(Ganadores[i].partidas_perdidas_vs_random) * 100.0 / 1000.0 << endl << endl;
    }
    QualityRandom.close();


    return Ganadores;
}

int main() {

    game_rules reglas_a_perfeccionar;
    cout << "Inserte cantidad de filas:" << endl;
    cin >> reglas_a_perfeccionar.rows;
    cout << "Inserte cantidad de columnas:" << endl;
    cin >> reglas_a_perfeccionar.columns;
    cout << "Inserte valor C:" << endl;
    cin >> reglas_a_perfeccionar.c;
    cout << "Inserte valor P:" << endl;
    cin >> reglas_a_perfeccionar.p;/*
    ofstream probando("./ResultadosTorneoInfinito/probando.output");
    probando << "xddddd" << endl;
    probando.close();*/


    vector<jugador_goloso> GrupoGanador = torneo_h_horas_Inicial(10, 1, 0.04, 0.5, reglas_a_perfeccionar); //(int h, int VecesRealizado, double porcentajeVariacion, double probabilidadMutar, const game_rules & reglas_a_perfeccionar)

    for (int i = 2; true; ++i) {
        GrupoGanador = torneo_h_horas_PreGrupo(10,GrupoGanador,i,0.04 / double(i) > 0.01 ? 0.04 / double(i) : 0.01, 0.5); //(int h, vector<jugador_goloso> & Grupo_entrenado, int VecesRealizado, double porcentajeVariacion, double probabilidadMutar)
    }
    return 0;
}

/*int main() {
    game_rules reglas_a_perfeccionar;
    cout << "Inserte cantidad de filas:" << endl;
    cin >> reglas_a_perfeccionar.rows;
    cout << "Inserte cantidad de columnas:" << endl;
    cin >> reglas_a_perfeccionar.columns;
    cout << "Inserte valor C:" << endl;
    cin >> reglas_a_perfeccionar.c;
    cout << "Inserte valor P:" << endl;
    cin >> reglas_a_perfeccionar.p;

    chrono::steady_clock::time_point t0;
    chrono::steady_clock::time_point t1;

    t0 = chrono::steady_clock::now();

    vector<jugador_goloso> Jugadores = CrearJugadoresTorneoDeGrupos(reglas_a_perfeccionar);
    vector<jugador_goloso> Ganadores = TorneoDeGrupos(Jugadores);


    t1 = chrono::steady_clock::now();
    chrono::duration<double, std::milli> delta_t = (t1 - t0);
    int Torneos = 0;


    while(delta_t < chrono::hours(1)) {
        Jugadores = GenerarHijos(Ganadores,0.05);
        Ganadores = TorneoDeGrupos(Jugadores);

        t1 = chrono::steady_clock::now();
        delta_t = t1 - t0;
        Torneos++;
        cout << Torneos << endl;
    }
    ofstream bestPlayers("bestPlayerParameters.output");
    for (size_t i = 0; i < Ganadores.size(); ++i) {
        bestPlayers << "Parametros Jugador" << i+1 << ": " << endl;
        for (size_t j = 0; j < Ganadores[i].parametrosVectoriales.size(); ++j) {
            for (size_t k = 0; k < Ganadores[i].parametrosVectoriales[j].size() - 1; ++k) {
                bestPlayers << Ganadores[i].parametrosVectoriales[j][k] << ", ";
            }
            bestPlayers << Ganadores[i].parametrosVectoriales[j][Ganadores[i].parametrosVectoriales[j].size() - 1] << endl;
        }
        bestPlayers << Ganadores[i].parametrosIndividuales[0] << endl << endl;
    }

    return 0;
}*/

/*int main() {
    std::random_device rd2;
    cout << rd2.entropy();
    cout << rd2();
}*/