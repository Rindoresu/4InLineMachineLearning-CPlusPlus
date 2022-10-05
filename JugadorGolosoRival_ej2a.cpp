 
#include "JugadorGoloso.h"


std::random_device rd;
std::mt19937 generator(rd());

void send(const std::string& msg) {
    std::cout << msg << std::endl;
}

void send(int msg) {
    std::cout << msg << std::endl;
}

int read_int() {
    std::string msg;
    std::cin >> msg;
    if (msg == "salir") {
        send("listo");
        std::exit(0);
    }
    return std::stoi(msg);
}

std::string read_str() {
    std::string msg;
    std::cin >> msg;
    if (msg == "salir") {
        send("listo");
        std::exit(0);
    }
    return msg;
}

void modificarTablero(const game_rules & this_game, vector<vector<string> > & board, const int & move, bool MiJugada) {
    int fila = 0;
    while(fila < this_game.rows and board[fila][move] == "-") {
        fila++;
    } // o estoy fuera de rango porque esta vacío, o estoy en la ficha más alta.
    if(MiJugada) {
        board[fila-1][move] = this_game.color; // le asigno mi color.
    } else {
        board[fila-1][move] = this_game.opponent_color; // le asigno el color de mi oponente.
    }
}

void deshacerTablero(const game_rules & this_game, vector<vector<string> > & board, const int & move) {
    int fila = 0;
    while(fila < this_game.rows and board[fila][move] == "-") {
        fila++;
    } // o estoy fuera de rango porque esta vacío, o estoy en la ficha más alta.
    if(board[fila][move] == this_game.color) {
        board[fila][move] = "-"; //deshago mi jugada.
    } else {
        throw; //Se intento deshacer una jugada del rival.
    }
}

void chequearRectasHorizontalesPosibles(const int & f, const int & c, const game_rules & this_game,
                                const vector<vector<string> > & board, vector<int>& cantMiasPosibles,
vector<int>& cantOponentePosibles, vector<vector<bool> >& tableroExaminado) {

    if(board[f][c] != "-") { //Si hay alguna jugada en la casilla.
        int c2;
        int cantidad = 0;
        for(c2 = c; c2 < this_game.columns and (board[f][c2] == board[f][c] or board[f][c2] == "-"); c2++){
            tableroExaminado[f][c2] = true;
            if (board[f][c2] == board[f][c]) {
                cantidad++;
            }
        }
        int c3;
        for(c3 = c-1; c3 > -1 and (board[f][c3] == board[f][c] or board[f][c3] == "-"); c3--){
            tableroExaminado[f][c3] = true;
            if (board[f][c3] == board[f][c]) {
                cantidad++;
            }
        }
        if(cantidad > this_game.c) { //Si tiene una recta posible mayor a longitud C tomaremos como que es de longitud C
            cantidad = this_game.c;
        }
        if(c2-c3-2>=this_game.c) { //Si esta es una posible recta mia
            if(board[f][c] == this_game.color) {
                cantMiasPosibles[cantidad-1]++; //c3 y c2 estan en los bordes por fuera de la Recta, por tanto la recta mide c2-c3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0)
            } else {
                cantOponentePosibles[cantidad-1]++; //era del contrincante
            }
        }
    }

}

void chequearRectasVerticalesPosibles(const int & f, const int & c, const game_rules & this_game,
                              const vector<vector<string> > & board, vector<int>& cantMiasPosibles,
vector<int>& cantOponentePosibles, vector<vector<bool> >& tableroExaminado2) {

    if(board[f][c] != "-") { //Si hay alguna jugada en la casilla.
        int f2;
        int cantidad = 0;
        for(f2 = f; f2 < this_game.rows and (board[f2][c] == board[f][c] or board[f2][c] == "-"); f2++){
            tableroExaminado2[f2][c] = true;
            if (board[f2][c] == board[f][c]) {
                cantidad++;
            }
        }
        int f3;
        for(f3 = f-1; f3 > -1 and (board[f3][c] == board[f][c] or board[f3][c] == "-"); f3--){
            tableroExaminado2[f3][c] = true;
            if (board[f3][c] == board[f][c]) {
                cantidad++;
            }
        }
        if(cantidad > this_game.c) { //Si tiene una recta posible mayor a longitud C tomaremos como que es de longitud C
            cantidad = this_game.c;
        }

        if(f2-f3-2 >= this_game.c) {
            if(board[f][c] == this_game.color) { //Si esta es una recta mia
                cantMiasPosibles[cantidad-1]++; //f3 y f2 estan en los bordes por fuera de la Recta, por tanto la recta mide f2-f3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0)
            } else {
                cantOponentePosibles[cantidad-1]++;
            }
        }
    }
}

void chequearRectasDiagonalesPosibles(const int & f, const int & c, const game_rules & this_game,
                              const vector<vector<string> > & board, vector<int>& cantMiasPosibles, vector<int>& cantOponentePosibles,
vector<vector<bool> >& tableroExaminado3, vector<vector<bool> >& tableroExaminado4) {

    if(board[f][c] != "-") { //Si hay alguna jugada en la casilla.
        int f2;
        int c2;
        int cantidad = 0;
        for(f2 = f, c2 = c; f2 < this_game.rows and c2 < this_game.columns and (board[f2][c2] == board[f][c] or board[f2][c2] == "-"); f2++, c2++){
            tableroExaminado3[f2][c2] = true;
            if(board[f2][c2] == board[f][c]) {
                cantidad++;
            }
        }
        int f3;
        int c3;
        for(f3 = f-1, c3 = c-1; f3 > -1 and c3 > -1 and (board[f3][c3] == board[f][c] or board[f3][c3] == "-"); f3--, c3--){
            tableroExaminado3[f3][c3] = true;
            if(board[f3][c3] == board[f][c]) {
                cantidad++;
            }
        }
        if(cantidad > this_game.c) { //Si tiene una recta posible mayor a longitud C tomaremos como que es de longitud C
            cantidad = this_game.c;
        }
        if(f2-f3-2 >= this_game.c) {
            if(board[f][c] == this_game.color) { //Si esta es una recta mia
                cantMiasPosibles[cantidad-1]++; //f3 y f2 estan en los bordes por fuera de la Recta, por tanto la recta mide f2-f3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0). Lo mismo con c3 y c2 pero nos daba igual.
            } else {
                cantOponentePosibles[cantidad-1]++;
            }
        }

        //Lo rehacemos pero para la diagonal de la esquina superior derecha a la inferior izquierda.
        //Fila aumenta, columna disminuye.
        cantidad = 0;
        for(f2 = f, c2 = c; f2 < this_game.rows and c2 > -1 and (board[f2][c2] == board[f][c] or board[f2][c2] == "-"); f2++, c2--){
            tableroExaminado4[f2][c2] = true;
            if(board[f2][c2] == board[f][c]) {
                cantidad++;
            }
        }

        //Fila disminuye, columna aumenta.
        for(f3 = f-1, c3 = c+1; f3 > -1 and c3 < this_game.columns and (board[f3][c3] == board[f][c] or board[f3][c3] == "-"); f3--, c3++){
            tableroExaminado4[f3][c3] = true;
            if(board[f3][c3] == board[f][c]) {
                cantidad++;
            }
        }
        if(cantidad > this_game.c) { //Si tiene una recta posible mayor a longitud C tomaremos como que es de longitud C
            cantidad = this_game.c;
        }
        if(f2-f3-2 >= this_game.c) {
            if(board[f][c] == this_game.color) { //Si esta es una recta mia
                cantMiasPosibles[cantidad-1]++; //f3 y f2 estan en los bordes por fuera de la Recta, por tanto la recta mide f2-f3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0). Con c2 y c3 es al revez.
            } else {
                cantOponentePosibles[cantidad-1]++;
            }
        }
    }
}


void chequearRectasHorizontales(const int & f, const int & c, const game_rules & this_game,
                                const vector<vector<string> > & board, vector<int>& cant_rectas_longitud,
                                vector<int>& cant_rectas_no_tapadas_oponente_longitud,
vector<int>& cant_rectas_no_tapadas_dosExtremos_oponente_longitud, vector<vector<bool> >& tableroExaminado) {

    if(board[f][c] != "-") { //Si hay alguna jugada en la casilla.
        int c2;
        for(c2 = c; c2 < this_game.columns and board[f][c2] == board[f][c]; c2++){
            tableroExaminado[f][c2] = true;
        }
        int c3;
        for(c3 = c; c3 > -1 and board[f][c3] == board[f][c]; c3--){
            tableroExaminado[f][c3] = true;
        }
        if(board[f][c] == this_game.color) { //Si esta es una recta mia
            cant_rectas_longitud[c2-c3-2]++; //c3 y c2 estan en los bordes por fuera de la Recta, por tanto la recta mide c2-c3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0)
        } else if((c2 < this_game.columns and board[f][c2] == "-") or (c3 > -1 and board[f][c3] == "-")) { //Es una recta del oponente no tapada
            cant_rectas_no_tapadas_oponente_longitud[c2-c3-2]++;
            if((c2 < this_game.columns and board[f][c2] == "-") and (c3 > -1 and board[f][c3] == "-")) {
                cant_rectas_no_tapadas_dosExtremos_oponente_longitud[c2-c3-2]++; //Cantidad de rectas no tapadas en ninguno de los dos extremos aumenta.
            }
        }
    }

}

void chequearRectasVerticales(const int & f, const int & c, const game_rules & this_game,
                              const vector<vector<string> > & board, vector<int>& cant_rectas_longitud,
                              vector<int>& cant_rectas_no_tapadas_oponente_longitud,
vector<int>& cant_rectas_no_tapadas_dosExtremos_oponente_longitud, vector<vector<bool> >& tableroExaminado2) {

    if(board[f][c] != "-") { //Si hay alguna jugada en la casilla.
        int f2;
        for(f2 = f; f2 < this_game.rows and board[f2][c] == board[f][c]; f2++){
            tableroExaminado2[f2][c] = true;
        }
        int f3;
        for(f3 = f; f3 > -1 and board[f3][c] == board[f][c]; f3--){
            tableroExaminado2[f3][c] = true;
        }
        if(board[f][c] == this_game.color) { //Si esta es una recta mia
            cant_rectas_longitud[f2-f3-2]++; //f3 y f2 estan en los bordes por fuera de la Recta, por tanto la recta mide f2-f3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0)
        } else if((f2 < this_game.rows and board[f2][c] == "-") or (f3 > -1 and board[f3][c] == "-")) { //Es una recta del oponente no tapada
            cant_rectas_no_tapadas_oponente_longitud[f2-f3-2]++;
            if((f2 < this_game.rows and board[f2][c] == "-") and (f3 > -1 and board[f3][c] == "-")) {
                cant_rectas_no_tapadas_dosExtremos_oponente_longitud[f2-f3-2]++; //Cantidad de rectas no tapadas en ninguno de los dos extremos aumenta.
            }
        }

    }
}

void chequearRectasDiagonales(const int & f, const int & c, const game_rules & this_game,
                              const vector<vector<string> > & board, vector<int>& cant_rectas_longitud,
                              vector<int>& cant_rectas_no_tapadas_oponente_longitud,
                              vector<int>& cant_rectas_no_tapadas_dosExtremos_oponente_longitud, vector<vector<bool> >& tableroExaminado3,
vector<vector<bool> >& tableroExaminado4) {

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
        if(board[f][c] == this_game.color) { //Si esta es una recta mia
            cant_rectas_longitud[f2-f3-2]++; //f3 y f2 estan en los bordes por fuera de la Recta, por tanto la recta mide f2-f3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0). Lo mismo con c3 y c2 pero nos daba igual.
        } else if((f2 < this_game.rows and c2 < this_game.columns and board[f2][c2] == "-") or (f3 > -1 and c3 > -1 and board[f3][c3] == "-")) { //Es una recta Diagonal del oponente no tapada
            cant_rectas_no_tapadas_oponente_longitud[f2-f3-2]++;
            if((f2 < this_game.rows and c2 < this_game.columns and board[f2][c2] == "-") and (f3 > -1 and c3 > -1 and board[f3][c3] == "-")) {
                cant_rectas_no_tapadas_dosExtremos_oponente_longitud[f2-f3-2]++; //Cantidad de rectas no tapadas en ninguno de los dos extremos aumenta.
            }
        }

        //Lo rehacemos pero para la diagonal de la esquina superior derecha a la inferior izquierda.
        //Fila aumenta, columna disminuye.
        for(f2 = f, c2 = c; f2 < this_game.rows and c2 > -1 and board[f2][c2] == board[f][c]; f2++, c2--){
            tableroExaminado4[f2][c2] = true;
        }

        //Fila disminuye, columna aumenta.
        for(f3 = f, c3 = c; f3 > -1 and c3 < this_game.columns and board[f3][c3] == board[f][c]; f3--, c3++){
            tableroExaminado4[f3][c3] = true;
        }
        if(board[f][c] == this_game.color) { //Si esta es una recta mia
            cant_rectas_longitud[f2-f3-2]++; //f3 y f2 estan en los bordes por fuera de la Recta, por tanto la recta mide f2-f3 menos una casilla, y por tanto lo guardamos con -2 (porque empezamos en 0). Con c2 y c3 es al revez.
        } else if((f2 < this_game.rows and c2 > -1 and board[f2][c2] == "-") or (f3 > -1 and c3 < this_game.columns and board[f3][c3] == "-")) { //Es una recta Diagonal del oponente no tapada
            cant_rectas_no_tapadas_oponente_longitud[f2-f3-2]++;
            if((f2 < this_game.rows and c2 > -1 and board[f2][c2] == "-") and (f3 > -1 and c3 < this_game.columns and board[f3][c3] == "-")) {
                cant_rectas_no_tapadas_dosExtremos_oponente_longitud[f2-f3-2]++; //Cantidad de rectas no tapadas en ninguno de los dos extremos aumenta.
            }
        }
    }
}

double valuarJugada(const game_rules & this_game, const vector<vector<string> > & board, const int & move,
        const vector<double> & C_parametros, const vector<double> & C_parametros_oponente,
        const vector<double> & C_parametros_oponente_doble,
const vector<double> & C_parametros_peso_particionar) {

    vector<int> cant_rectas_longitud(C_parametros.size(), 0); //para cada longitud posible empiezo sin haber encontrado jugadas.
    vector<int> cant_rectas_no_tapadas_oponente_longitud(C_parametros_oponente.size(), 0); //para cada longitud posible empiezo sin haber encontrado jugadas del oponente.
    vector<int> cant_rectas_no_tapadas_dosExtremos_oponente_longitud(C_parametros_oponente_doble.size(), 0); //para cada longitud posible empiezo sin haber encontrado jugadas del oponente.
    vector<vector<bool> > tableroExaminado(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero horizontalmente.
    vector<vector<bool> > tableroExaminado2(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero verticalmente.
    vector<vector<bool> > tableroExaminado3(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero diagonalmente superior izquierda inferior derecha.
    vector<vector<bool> > tableroExaminado4(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero diagonalmente superior derecha inferior izquierda.


    for(int f = 0; f < this_game.rows; f++) {
        for(int c = 0; c < this_game.columns; c++) {
            if(!tableroExaminado[f][c] or !tableroExaminado2[f][c] or !tableroExaminado3[f][c] or !tableroExaminado4[f][c]) { //Si una recta no esta examinada entonces AL MENOS una celda de la misma no fue examinada (no cuenta rectas que esten completamente tapadas por otras rectas).
                chequearRectasHorizontales(f, c, this_game, board, cant_rectas_longitud, cant_rectas_no_tapadas_oponente_longitud, cant_rectas_no_tapadas_dosExtremos_oponente_longitud, tableroExaminado);
                chequearRectasVerticales(f, c, this_game, board, cant_rectas_longitud, cant_rectas_no_tapadas_oponente_longitud, cant_rectas_no_tapadas_dosExtremos_oponente_longitud, tableroExaminado2);
                chequearRectasDiagonales(f, c, this_game, board, cant_rectas_longitud, cant_rectas_no_tapadas_oponente_longitud, cant_rectas_no_tapadas_dosExtremos_oponente_longitud, tableroExaminado3, tableroExaminado4);
                tableroExaminado[f][c] = true;
                tableroExaminado2[f][c] = true;
                tableroExaminado3[f][c] = true;
                tableroExaminado4[f][c] = true;
            }
        }
    }

    double valorJugada = 0;
    for(int k = 1; k < this_game.c + 1; k++) {
        valorJugada += cant_rectas_longitud[k-1] * C_parametros[k-1]; //Calculo el peso de todas las rectas de cada longitud mias.
        valorJugada += cant_rectas_no_tapadas_oponente_longitud[k-1] * C_parametros_oponente[k-1]; //Calculo el peso de todas las rectas del oponente no tapadas.
        valorJugada += cant_rectas_no_tapadas_dosExtremos_oponente_longitud[k-1] * C_parametros_oponente_doble[k-1]; //Calculo el peso de todas las rectas del oponente no tapadas en ninguno de los dos extremos.
    }

    vector<vector<int> > cant_particiones_menores_c_fila(this_game.rows, vector<int>(this_game.c, 0)); //Quiero disminuir la cantidad de particiones libres en cada fila con mas de C columnas libres.
    for(int f = 0; f < this_game.rows; f++) {
        int celdas_vacias_en_fila = 0;
        for(int c = 0; c < this_game.columns; c++) {
            if(board[f][c] == "-") {
                celdas_vacias_en_fila++;
            } else {
                if (celdas_vacias_en_fila < this_game.c) { // Encontre una recta en la fila con menos de C celdas vacias, eso significa que alguien lo tapo (en particular podria haber sido mi jugada, si es asi eso es bueno, si no es asi, todas las jugadas van a contar ese peso).
                    cant_particiones_menores_c_fila[f][celdas_vacias_en_fila]++; // Encontre una particion de longitud celdas_vacias_en_fila
                }
                celdas_vacias_en_fila = 0;
            }
        }
        if (celdas_vacias_en_fila < this_game.c) {
            cant_particiones_menores_c_fila[f][celdas_vacias_en_fila]++;
        }
    }
    for(int k = 0; k < this_game.rows; k++) {
        for(int particion = 0; particion < this_game.c; particion++) {
            valorJugada += cant_particiones_menores_c_fila[k][particion] * C_parametros_peso_particionar[particion]; // Esto cuenta cuanto peso aporta haber particionado la fila para no poder hacer jugadas horizontales mas grandes que C.
        }
    }

    return valorJugada;
}



double valuarJugada2(const game_rules & this_game, const vector<vector<string> > & board, const int & move,
                    const vector<double> & peso_mis_jugadas, const vector<double> & peso_jugadas_contrincante,
const vector<double> & C_parametros_peso_particionar) {

    vector<int> cantMiasPosibles(this_game.c, 0); //para cada longitud posible empiezo sin haber encontrado jugadas.
    vector<int> cantOponentePosibles(this_game.c, 0); //para cada longitud posible empiezo sin haber encontrado jugadas del oponente.
    vector<vector<bool> > tableroExaminado(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero horizontalmente.
    vector<vector<bool> > tableroExaminado2(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero verticalmente.
    vector<vector<bool> > tableroExaminado3(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero diagonalmente superior izquierda inferior derecha.
    vector<vector<bool> > tableroExaminado4(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero diagonalmente superior derecha inferior izquierda.


    for(int f = 0; f < this_game.rows; f++) {
        for(int c = 0; c < this_game.columns; c++) {
            if(!tableroExaminado[f][c] or !tableroExaminado2[f][c] or !tableroExaminado3[f][c] or !tableroExaminado4[f][c]) { //Si una recta no esta examinada entonces AL MENOS una celda de la misma no fue examinada (no cuenta rectas que esten completamente tapadas por otras rectas).
                chequearRectasHorizontalesPosibles(f, c, this_game, board, cantMiasPosibles, cantOponentePosibles, tableroExaminado);
                chequearRectasVerticalesPosibles(f, c, this_game, board, cantMiasPosibles, cantOponentePosibles, tableroExaminado2);
                chequearRectasDiagonalesPosibles(f, c, this_game, board, cantMiasPosibles, cantOponentePosibles, tableroExaminado3, tableroExaminado4);
                tableroExaminado[f][c] = true;
                tableroExaminado2[f][c] = true;
                tableroExaminado3[f][c] = true;
                tableroExaminado4[f][c] = true;
            }
        }
    }

    double valorJugada = 0;
    for(int k = 1; k < this_game.c + 1; k++) {
        valorJugada += cantMiasPosibles[k-1] * peso_mis_jugadas[k-1]; //Calculo el peso de todas las rectas de cada longitud mias.
        valorJugada += cantOponentePosibles[k-1] * peso_jugadas_contrincante[k-1]; //Calculo el peso de todas las rectas del oponente no tapadas.
    }



    vector<vector<int> > cant_particiones_menores_c_fila(this_game.rows, vector<int>(this_game.c, 0)); //Quiero disminuir la cantidad de particiones libres en cada fila con mas de C columnas libres.
    for(int f = 0; f < this_game.rows; f++) {
        int celdas_vacias_en_fila = 0;
        for(int c = 0; c < this_game.columns; c++) {
            if(board[f][c] == "-") {
                celdas_vacias_en_fila++;
            } else {
                if (celdas_vacias_en_fila < this_game.c) { // Encontre una recta en la fila con menos de C celdas vacias, eso significa que alguien lo tapo (en particular podria haber sido mi jugada, si es asi eso es bueno, si no es asi, todas las jugadas van a contar ese peso).
                    cant_particiones_menores_c_fila[f][celdas_vacias_en_fila]++; // Encontre una particion de longitud celdas_vacias_en_fila
                }
                celdas_vacias_en_fila = 0;
            }
        }
        if (celdas_vacias_en_fila < this_game.c) {
            cant_particiones_menores_c_fila[f][celdas_vacias_en_fila]++;
        }
    }
    for(int k = 0; k < this_game.rows; k++) {
        for(int particion = 0; particion < this_game.c; particion++) {
            valorJugada += cant_particiones_menores_c_fila[k][particion] * C_parametros_peso_particionar[particion]; // Esto cuenta cuanto peso aporta haber particionado la fila para no poder hacer jugadas horizontales mas grandes que C.
        }
    }

    return valorJugada;
}

double valuarJugada3(const game_rules & this_game, const vector<vector<string> > & board, const int & move,
                     const vector<double> & peso_mis_jugadas, const vector<double> & peso_jugadas_contrincante,
                     const vector<double> & C_parametros, const vector<double> & C_parametros_oponente,
                     const vector<double> & C_parametros_oponente_doble,
const vector<double> & C_parametros_peso_particionar) {

    vector<int> cant_rectas_longitud(C_parametros.size(), 0); //para cada longitud posible empiezo sin haber encontrado jugadas.
    vector<int> cant_rectas_no_tapadas_oponente_longitud(C_parametros_oponente.size(), 0); //para cada longitud posible empiezo sin haber encontrado jugadas del oponente.
    vector<int> cant_rectas_no_tapadas_dosExtremos_oponente_longitud(C_parametros_oponente_doble.size(), 0); //para cada longitud posible empiezo sin haber encontrado jugadas del oponente.
    vector<vector<bool> > tableroExaminado(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero horizontalmente.
    vector<vector<bool> > tableroExaminado2(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero verticalmente.
    vector<vector<bool> > tableroExaminado3(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero diagonalmente superior izquierda inferior derecha.
    vector<vector<bool> > tableroExaminado4(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero diagonalmente superior derecha inferior izquierda.


    for(int f = 0; f < this_game.rows; f++) {
        for(int c = 0; c < this_game.columns; c++) {
            if(!tableroExaminado[f][c] or !tableroExaminado2[f][c] or !tableroExaminado3[f][c] or !tableroExaminado4[f][c]) { //Si una recta no esta examinada entonces AL MENOS una celda de la misma no fue examinada (no cuenta rectas que esten completamente tapadas por otras rectas).
                chequearRectasHorizontales(f, c, this_game, board, cant_rectas_longitud, cant_rectas_no_tapadas_oponente_longitud, cant_rectas_no_tapadas_dosExtremos_oponente_longitud, tableroExaminado);
                chequearRectasVerticales(f, c, this_game, board, cant_rectas_longitud, cant_rectas_no_tapadas_oponente_longitud, cant_rectas_no_tapadas_dosExtremos_oponente_longitud, tableroExaminado2);
                chequearRectasDiagonales(f, c, this_game, board, cant_rectas_longitud, cant_rectas_no_tapadas_oponente_longitud, cant_rectas_no_tapadas_dosExtremos_oponente_longitud, tableroExaminado3, tableroExaminado4);
                tableroExaminado[f][c] = true;
                tableroExaminado2[f][c] = true;
                tableroExaminado3[f][c] = true;
                tableroExaminado4[f][c] = true;
            }
        }
    }

    double valorJugada = 0;
    for(int k = 1; k < this_game.c + 1; k++) {
        valorJugada += cant_rectas_longitud[k-1] * C_parametros[k-1]; //Calculo el peso de todas las rectas de cada longitud mias.
        valorJugada += cant_rectas_no_tapadas_oponente_longitud[k-1] * C_parametros_oponente[k-1]; //Calculo el peso de todas las rectas del oponente no tapadas.
        valorJugada += cant_rectas_no_tapadas_dosExtremos_oponente_longitud[k-1] * C_parametros_oponente_doble[k-1]; //Calculo el peso de todas las rectas del oponente no tapadas en ninguno de los dos extremos.
    }

    vector<int> cantMiasPosibles(this_game.c, 0); //para cada longitud posible empiezo sin haber encontrado jugadas.
    vector<int> cantOponentePosibles(this_game.c, 0); //para cada longitud posible empiezo sin haber encontrado jugadas del oponente.
    tableroExaminado = vector<vector<bool> >(this_game.rows, vector<bool>(this_game.columns, false)); //Vuelvo a chequear de otra forma
    tableroExaminado2 = vector<vector<bool> >(this_game.rows, vector<bool>(this_game.columns, false));
    tableroExaminado3 = vector<vector<bool> >(this_game.rows, vector<bool>(this_game.columns, false));
    tableroExaminado4 = vector<vector<bool> >(this_game.rows, vector<bool>(this_game.columns, false));


    for(int f = 0; f < this_game.rows; f++) {
        for(int c = 0; c < this_game.columns; c++) {
            if(!tableroExaminado[f][c] or !tableroExaminado2[f][c] or !tableroExaminado3[f][c] or !tableroExaminado4[f][c]) { //Si una recta no esta examinada entonces AL MENOS una celda de la misma no fue examinada (no cuenta rectas que esten completamente tapadas por otras rectas).
                chequearRectasHorizontalesPosibles(f, c, this_game, board, cantMiasPosibles, cantOponentePosibles, tableroExaminado);
                chequearRectasVerticalesPosibles(f, c, this_game, board, cantMiasPosibles, cantOponentePosibles, tableroExaminado2);
                chequearRectasDiagonalesPosibles(f, c, this_game, board, cantMiasPosibles, cantOponentePosibles, tableroExaminado3, tableroExaminado4);
                tableroExaminado[f][c] = true;
                tableroExaminado2[f][c] = true;
                tableroExaminado3[f][c] = true;
                tableroExaminado4[f][c] = true;
            }
        }
    }

    for(int k = 1; k < this_game.c + 1; k++) {
        valorJugada += cantMiasPosibles[k-1] * peso_mis_jugadas[k-1]; //Calculo el peso de todas las rectas de cada longitud mias.
        valorJugada += cantOponentePosibles[k-1] * peso_jugadas_contrincante[k-1]; //Calculo el peso de todas las rectas del oponente no tapadas.
    }

    vector<vector<int> > cant_particiones_menores_c_fila(this_game.rows, vector<int>(this_game.c, 0)); //Quiero disminuir la cantidad de particiones libres en cada fila con mas de C columnas libres.
    for(int f = 0; f < this_game.rows; f++) {
        int celdas_vacias_en_fila = 0;
        for(int c = 0; c < this_game.columns; c++) {
            if(board[f][c] == "-") {
                celdas_vacias_en_fila++;
            } else {
                if (celdas_vacias_en_fila < this_game.c) { // Encontre una recta en la fila con menos de C celdas vacias, eso significa que alguien lo tapo (en particular podria haber sido mi jugada, si es asi eso es bueno, si no es asi, todas las jugadas van a contar ese peso).
                    cant_particiones_menores_c_fila[f][celdas_vacias_en_fila]++; // Encontre una particion de longitud celdas_vacias_en_fila
                }
                celdas_vacias_en_fila = 0;
            }
        }
        if (celdas_vacias_en_fila < this_game.c) {
            cant_particiones_menores_c_fila[f][celdas_vacias_en_fila]++;
        }
    }
    for(int k = 0; k < this_game.rows; k++) {
        for(int particion = 0; particion < this_game.c; particion++) {
            valorJugada += cant_particiones_menores_c_fila[k][particion] * C_parametros_peso_particionar[particion]; // Esto cuenta cuanto peso aporta haber particionado la fila para no poder hacer jugadas horizontales mas grandes que C.
        }
    }

    return valorJugada;
}

void chequearRectasHorizontalesGanadoras(const int & f, const int & c, const game_rules & this_game,
                                         const vector<vector<string> > & board, int& cantRectasGanadoras,
vector<vector<bool> >& tableroExaminado) {

    if(board[f][c] == this_game.opponent_color) { //Si hay alguna jugada en la casilla.
        int c2;
        int c3;
        int cantidad_vacias = 0;
        int cantidad = 0;

        //Chequeo primero lo ya chequeado
        for(c3 = c-1; c3 > -1 and (cantidad_vacias < 1 or board[f][c3] != "-") and (board[f][c3] == board[f][c] or board[f][c3] == "-"); c3--){
            tableroExaminado[f][c3] = true;
            if (board[f][c3] == board[f][c]) {
                cantidad++;
            } else { //Como no es una casilla con ficha del oponente, entonces es una casilla vacia
                cantidad_vacias++;
            }
        }

        for(c2 = c; c2 < this_game.columns and (cantidad_vacias < 1 or board[f][c2] != "-") and (board[f][c2] == board[f][c] or board[f][c2] == "-"); c2++){
            tableroExaminado[f][c2] = true;
            if (board[f][c2] == board[f][c]) {
                cantidad++;
            } else { //Como no es una casilla con ficha del oponente, entonces es una casilla vacia
                cantidad_vacias++;
            }
        }


        if(cantidad_vacias<=1) { //Como no es una casilla con ficha del oponente, entonces es una casilla vacia
            if(cantidad >= this_game.c -1) {
                cantRectasGanadoras++;
            }
        }
    }

}

void chequearRectasDiagonalesGanadoras(const int & f, const int & c, const game_rules & this_game,
                                      const vector<vector<string> > & board, int& cantRectasGanadoras,
vector<vector<bool> >& tableroExaminado2, vector<vector<bool> >& tableroExaminado3) {

    if(board[f][c] == this_game.opponent_color) { //Si hay alguna jugada en la casilla.
        int f2;
        int c2;
        int f3;
        int c3;
        int cantidad = 0;
        int cantidad_vacias = 0;

        //chequeo primero lo ya chequeado
        for(f3 = f-1, c3 = c-1; f3 > -1 and c3 > -1 and (cantidad_vacias < 1 or board[f3][c3] != "-") and (board[f3][c3] == board[f][c] or board[f3][c3] == "-"); f3--, c3--){
            tableroExaminado2[f3][c3] = true;
            if(board[f3][c3] == board[f][c]) {
                cantidad++;
            } else { //Como no es una casilla con ficha del oponente, entonces es una casilla vacia
                cantidad_vacias++;
            }
        }

        for(f2 = f, c2 = c; f2 < this_game.rows and c2 < this_game.columns and (cantidad_vacias < 1 or board[f2][c2] != "-") and (board[f2][c2] == board[f][c] or board[f2][c2] == "-"); f2++, c2++){
            tableroExaminado2[f2][c2] = true;
            if(board[f2][c2] == board[f][c]) {
                cantidad++;
            } else { //Como no es una casilla con ficha del oponente, entonces es una casilla vacia
                cantidad_vacias++;
            }
        }

        if(cantidad_vacias<=1) { //Como no es una casilla con ficha del oponente, entonces es una casilla vacia
            if(cantidad >= this_game.c -1) {
                cantRectasGanadoras++;
            }
        }

        //Lo rehacemos pero para la diagonal de la esquina superior derecha a la inferior izquierda.
        //Fila aumenta, columna disminuye.
        cantidad = 0;
        cantidad_vacias = 0;

        //chequeo primero lo ya chequeado
        for(f3 = f-1, c3 = c+1; f3 > -1 and c3 < this_game.columns and (cantidad_vacias < 1 or board[f3][c3] != "-") and (board[f3][c3] == board[f][c] or board[f3][c3] == "-"); f3--, c3++){
            tableroExaminado3[f3][c3] = true;
            if(board[f3][c3] == board[f][c]) {
                cantidad++;
            } else { //Como no es una casilla con ficha del oponente, entonces es una casilla vacia
                cantidad_vacias++;
            }
        }

        for(f2 = f, c2 = c; f2 < this_game.rows and c2 > -1  and (cantidad_vacias < 1 or board[f2][c2] != "-") and (board[f2][c2] == board[f][c] or board[f2][c2] == "-"); f2++, c2--){
            tableroExaminado3[f2][c2] = true;
            if(board[f2][c2] == board[f][c]) {
                cantidad++;
            } else { //Como no es una casilla con ficha del oponente, entonces es una casilla vacia
                cantidad_vacias++;
            }
        }

        if(cantidad_vacias<=1) { //Como no es una casilla con ficha del oponente, entonces es una casilla vacia
            if(cantidad >= this_game.c -1) {
                cantRectasGanadoras++;
            }
        }
    }
}

bool oponenteVaAGanar(const game_rules & this_game, const vector<vector<string> > & board) {
    int cant_rectas_ganadoras = 0;
    vector<vector<bool> > tableroExaminado(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero.
    vector<vector<bool> > tableroExaminado2(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero.
    vector<vector<bool> > tableroExaminado3(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero.
    for(int f = 0; f < this_game.rows; f++) {
        for(int c = 0; c < this_game.columns; c++) {
            if(!tableroExaminado[f][c] or !tableroExaminado2[f][c] or !tableroExaminado3[f][c]) { //Si una recta no esta examinada entonces AL MENOS una celda de la misma no fue examinada (no cuenta rectas que esten completamente tapadas por otras rectas).
                chequearRectasHorizontalesGanadoras(f, c, this_game, board, cant_rectas_ganadoras, tableroExaminado);
                chequearRectasDiagonalesGanadoras(f, c, this_game, board, cant_rectas_ganadoras, tableroExaminado2, tableroExaminado3);
                tableroExaminado[f][c] = true;
                tableroExaminado2[f][c] = true;
                tableroExaminado3[f][c] = true;
            }
        }
    }
    return cant_rectas_ganadoras > 0;
}

double valuarJugada4(const game_rules & this_game, const vector<vector<string> > & board, const int & move,
                     const vector<double> & peso_mis_jugadas, const vector<double> & peso_jugadas_contrincante,
                     const vector<double> & C_parametros, const vector<double> & C_parametros_oponente,
const vector<double> & C_parametros_oponente_doble,
const vector<double> & C_parametros_peso_particionar, const double & parametro_oponente_gana) {

    vector<int> cant_rectas_longitud(C_parametros.size(), 0); //para cada longitud posible empiezo sin haber encontrado jugadas.
    vector<int> cant_rectas_no_tapadas_oponente_longitud(C_parametros_oponente.size(), 0); //para cada longitud posible empiezo sin haber encontrado jugadas del oponente.
    vector<int> cant_rectas_no_tapadas_dosExtremos_oponente_longitud(C_parametros_oponente_doble.size(), 0); //para cada longitud posible empiezo sin haber encontrado jugadas del oponente.
    vector<vector<bool> > tableroExaminado(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero horizontalmente.
    vector<vector<bool> > tableroExaminado2(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero verticalmente.
    vector<vector<bool> > tableroExaminado3(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero diagonalmente superior izquierda inferior derecha.
    vector<vector<bool> > tableroExaminado4(this_game.rows, vector<bool>(this_game.columns, false)); //Todavia no examine el tablero diagonalmente superior derecha inferior izquierda.


    for(int f = 0; f < this_game.rows; f++) {
        for(int c = 0; c < this_game.columns; c++) {
            if(!tableroExaminado[f][c] or !tableroExaminado2[f][c] or !tableroExaminado3[f][c] or !tableroExaminado4[f][c]) { //Si una recta no esta examinada entonces AL MENOS una celda de la misma no fue examinada (no cuenta rectas que esten completamente tapadas por otras rectas).
                chequearRectasHorizontales(f, c, this_game, board, cant_rectas_longitud, cant_rectas_no_tapadas_oponente_longitud, cant_rectas_no_tapadas_dosExtremos_oponente_longitud, tableroExaminado);
                chequearRectasVerticales(f, c, this_game, board, cant_rectas_longitud, cant_rectas_no_tapadas_oponente_longitud, cant_rectas_no_tapadas_dosExtremos_oponente_longitud, tableroExaminado2);
                chequearRectasDiagonales(f, c, this_game, board, cant_rectas_longitud, cant_rectas_no_tapadas_oponente_longitud, cant_rectas_no_tapadas_dosExtremos_oponente_longitud, tableroExaminado3, tableroExaminado4);
                tableroExaminado[f][c] = true;
                tableroExaminado2[f][c] = true;
                tableroExaminado3[f][c] = true;
                tableroExaminado4[f][c] = true;
            }
        }
    }

    double valorJugada = 0;
    for(int k = 1; k < this_game.c + 1; k++) {
        valorJugada += cant_rectas_longitud[k-1] * C_parametros[k-1]; //Calculo el peso de todas las rectas de cada longitud mias.
        valorJugada += cant_rectas_no_tapadas_oponente_longitud[k-1] * C_parametros_oponente[k-1]; //Calculo el peso de todas las rectas del oponente no tapadas.
        valorJugada += cant_rectas_no_tapadas_dosExtremos_oponente_longitud[k-1] * C_parametros_oponente_doble[k-1]; //Calculo el peso de todas las rectas del oponente no tapadas en ninguno de los dos extremos.
    }

    vector<int> cantMiasPosibles(this_game.c, 0); //para cada longitud posible empiezo sin haber encontrado jugadas.
    vector<int> cantOponentePosibles(this_game.c, 0); //para cada longitud posible empiezo sin haber encontrado jugadas del oponente.
    tableroExaminado = vector<vector<bool> >(this_game.rows, vector<bool>(this_game.columns, false)); //Vuelvo a chequear de otra forma
    tableroExaminado2 = vector<vector<bool> >(this_game.rows, vector<bool>(this_game.columns, false));
    tableroExaminado3 = vector<vector<bool> >(this_game.rows, vector<bool>(this_game.columns, false));
    tableroExaminado4 = vector<vector<bool> >(this_game.rows, vector<bool>(this_game.columns, false));


    for(int f = 0; f < this_game.rows; f++) {
        for(int c = 0; c < this_game.columns; c++) {
            if(!tableroExaminado[f][c] or !tableroExaminado2[f][c] or !tableroExaminado3[f][c] or !tableroExaminado4[f][c]) { //Si una recta no esta examinada entonces AL MENOS una celda de la misma no fue examinada (no cuenta rectas que esten completamente tapadas por otras rectas).
                chequearRectasHorizontalesPosibles(f, c, this_game, board, cantMiasPosibles, cantOponentePosibles, tableroExaminado);
                chequearRectasVerticalesPosibles(f, c, this_game, board, cantMiasPosibles, cantOponentePosibles, tableroExaminado2);
                chequearRectasDiagonalesPosibles(f, c, this_game, board, cantMiasPosibles, cantOponentePosibles, tableroExaminado3, tableroExaminado4);
                tableroExaminado[f][c] = true;
                tableroExaminado2[f][c] = true;
                tableroExaminado3[f][c] = true;
                tableroExaminado4[f][c] = true;
            }
        }
    }

    for(int k = 1; k < this_game.c + 1; k++) {
        valorJugada += cantMiasPosibles[k-1] * peso_mis_jugadas[k-1]; //Calculo el peso de todas las rectas de cada longitud mias.
        valorJugada += cantOponentePosibles[k-1] * peso_jugadas_contrincante[k-1]; //Calculo el peso de todas las rectas del oponente no tapadas.
    }

    vector<vector<int> > cant_particiones_menores_c_fila(this_game.rows, vector<int>(this_game.c, 0)); //Quiero disminuir la cantidad de particiones libres en cada fila con mas de C columnas libres.
    for(int f = 0; f < this_game.rows; f++) {
        int celdas_vacias_en_fila = 0;
        for(int c = 0; c < this_game.columns; c++) {
            if(board[f][c] == "-") {
                celdas_vacias_en_fila++;
            } else {
                if (celdas_vacias_en_fila < this_game.c) { // Encontre una recta en la fila con menos de C celdas vacias, eso significa que alguien lo tapo (en particular podria haber sido mi jugada, si es asi eso es bueno, si no es asi, todas las jugadas van a contar ese peso).
                    cant_particiones_menores_c_fila[f][celdas_vacias_en_fila]++; // Encontre una particion de longitud celdas_vacias_en_fila
                }
                celdas_vacias_en_fila = 0;
            }
        }
        if (celdas_vacias_en_fila < this_game.c) {
            cant_particiones_menores_c_fila[f][celdas_vacias_en_fila]++;
        }
    }
    for(int k = 0; k < this_game.rows; k++) {
        for(int particion = 0; particion < this_game.c; particion++) {
            valorJugada += cant_particiones_menores_c_fila[k][particion] * C_parametros_peso_particionar[particion]; // Esto cuenta cuanto peso aporta haber particionado la fila para no poder hacer jugadas horizontales mas grandes que C.
        }
    }

    if(oponenteVaAGanar(this_game, board)){
        valorJugada += parametro_oponente_gana;
    }

    return valorJugada;
}

int jugadaGolosa(const game_rules & this_game, vector<vector<string> > & board, const vector<double> & C_parametros,
                 const vector<double> & C_parametros_oponente, const vector<double> & C_parametros_oponente_doble,
const vector<double> & C_parametros_peso_particionar) {
    int move;
    vector<int> best_moves;
    double valorJugadaMaxima = -INFINITY;
    double valorJugadaActual;

    for(move = 0; move < this_game.columns; move++) {
        if(board[0][move] == "-") { // "-" determina la casilla vacía, por lo que la jugada es válida
            modificarTablero(this_game, board, move, true); //Es mi jugada por lo que va un true al final.
            valorJugadaActual = valuarJugada(this_game, board, move, C_parametros, C_parametros_oponente,
                    C_parametros_oponente_doble, C_parametros_peso_particionar);

            if(valorJugadaActual > valorJugadaMaxima) {
                best_moves.erase(best_moves.begin(), best_moves.end());
                best_moves.push_back(move);
                valorJugadaMaxima = valorJugadaActual;
            } else if (valorJugadaActual == valorJugadaMaxima) {
                best_moves.push_back(move);
            }

            deshacerTablero(this_game, board, move);
        }
    }
    uniform_int_distribution<int> select_move(0, best_moves.size() - 1);
    int selected_move = select_move(generator); //Selecciono random entre cualquiera de las jugadas mejores de igual valor.

    return best_moves[selected_move];
}

int jugadaGolosa2(const game_rules & this_game, vector<vector<string> > & board,
                  const vector<double> & peso_mis_jugadas, const vector<double> & peso_jugadas_contrincante,
const vector<double> & C_parametros_peso_particionar) {
    int move;
    vector<int> best_moves;
    double valorJugadaMaxima = -INFINITY;
    double valorJugadaActual;

    for(move = 0; move < this_game.columns; move++) {
        if(board[0][move] == "-") { // "-" determina la casilla vacía, por lo que la jugada es válida
            modificarTablero(this_game, board, move, true); //Es mi jugada por lo que va un true al final.
            valorJugadaActual = valuarJugada2(this_game, board, move, peso_mis_jugadas, peso_jugadas_contrincante,
                    C_parametros_peso_particionar);

            if(valorJugadaActual > valorJugadaMaxima) {
                best_moves.erase(best_moves.begin(), best_moves.end());
                best_moves.push_back(move);
                valorJugadaMaxima = valorJugadaActual;
            } else if (valorJugadaActual == valorJugadaMaxima) {
                best_moves.push_back(move);
            }

            deshacerTablero(this_game, board, move);
        }
    }
    uniform_int_distribution<int> select_move(0, best_moves.size() - 1);
    int selected_move = select_move(generator); //Selecciono random entre cualquiera de las jugadas mejores de igual valor.

    return best_moves[selected_move];
}

int jugadaGolosa3(const game_rules & this_game, vector<vector<string> > & board,
                  const vector<double> & peso_mis_jugadas, const vector<double> & peso_jugadas_contrincante, const vector<double> & C_parametros,
                 const vector<double> & C_parametros_oponente, const vector<double> & C_parametros_oponente_doble,
const vector<double> & C_parametros_peso_particionar) {
    int move;
    vector<int> best_moves;
    double valorJugadaMaxima = -INFINITY;
    double valorJugadaActual;

    for(move = 0; move < this_game.columns; move++) {
        if(board[0][move] == "-") { // "-" determina la casilla vacía, por lo que la jugada es válida
            modificarTablero(this_game, board, move, true); //Es mi jugada por lo que va un true al final.
            valorJugadaActual = valuarJugada3(this_game, board, move, peso_mis_jugadas, peso_jugadas_contrincante, C_parametros, C_parametros_oponente,
                                             C_parametros_oponente_doble, C_parametros_peso_particionar);

            if(valorJugadaActual > valorJugadaMaxima) {
                best_moves.erase(best_moves.begin(), best_moves.end());
                best_moves.push_back(move);
                valorJugadaMaxima = valorJugadaActual;
            } else if (valorJugadaActual == valorJugadaMaxima) {
                best_moves.push_back(move);
            }

            deshacerTablero(this_game, board, move);
        }
    }
    uniform_int_distribution<int> select_move(0, best_moves.size() - 1);
    int selected_move = select_move(generator); //Selecciono random entre cualquiera de las jugadas mejores de igual valor.

    return best_moves[selected_move];
}

int jugadaGolosa4(const game_rules & this_game, vector<vector<string> > & board,
                  const vector<double> & peso_mis_jugadas, const vector<double> & peso_jugadas_contrincante, const vector<double> & C_parametros,
                  const vector<double> & C_parametros_oponente, const vector<double> & C_parametros_oponente_doble,
                  const vector<double> & C_parametros_peso_particionar,
const double & parametro_oponente_gana) {
    int move;
    vector<int> best_moves;
    double valorJugadaMaxima = -INFINITY;
    double valorJugadaActual;

    for(move = 0; move < this_game.columns; move++) {
        if(board[0][move] == "-") { // "-" determina la casilla vacía, por lo que la jugada es válida
            modificarTablero(this_game, board, move, true); //Es mi jugada por lo que va un true al final.
            valorJugadaActual = valuarJugada4(this_game, board, move, peso_mis_jugadas, peso_jugadas_contrincante, C_parametros, C_parametros_oponente,
                                              C_parametros_oponente_doble, C_parametros_peso_particionar, parametro_oponente_gana);

            if(valorJugadaActual > valorJugadaMaxima) {
                best_moves.erase(best_moves.begin(), best_moves.end());
                best_moves.push_back(move);
                valorJugadaMaxima = valorJugadaActual;
            } else if (valorJugadaActual == valorJugadaMaxima) {
                best_moves.push_back(move);
            }

            deshacerTablero(this_game, board, move);
        }
    }
    uniform_int_distribution<int> select_move(0, best_moves.size() - 1);
    int selected_move = select_move(generator); //Selecciono random entre cualquiera de las jugadas mejores de igual valor.

    return best_moves[selected_move];
}

void jugadorGoloso1() {
    game_rules this_game;
    string msg, go_first;
    int move;

    msg = read_str();
    vector<double> C_parametros(this_game.c);
    vector<double> C_parametros_oponente(this_game.c);
    vector<double> C_parametros_oponente_doble(this_game.c);
    vector<double> C_parametros_peso_particionar(this_game.c);

    for(int i = 0; i < this_game.c; i++) {
        C_parametros[i] = i;
        C_parametros_oponente[i] = -i;
        C_parametros_oponente_doble[i] = -i;
        C_parametros_peso_particionar[i] = i;
    }

    while (msg != "salir") {

        this_game.color = msg;
        this_game.opponent_color = read_str();

        this_game.columns = read_int();
        this_game.rows = read_int();
        this_game.c = read_int();
        this_game.p = read_int();

        vector<vector<string> > board(this_game.rows, vector<string>(this_game.columns, "-"));

        go_first = read_str();
        if (go_first == "vos") {
            move = jugadaGolosa(this_game,board, C_parametros, C_parametros_oponente, C_parametros_oponente_doble, C_parametros_peso_particionar);
            //move = do_move(generator); // Deberiamos cambiar por jugadaGolosa
            modificarTablero(this_game, board, move, true); //Mi jugada, va un true al final.
            send(move);
        }

        msg = read_str();
        while (msg != "ganaste" and msg != "perdiste" and msg != "empataron"){
            modificarTablero(this_game, board,stoi(msg), false); // Construyo la jugada del oponente.

            move = jugadaGolosa(this_game,board, C_parametros, C_parametros_oponente, C_parametros_oponente_doble, C_parametros_peso_particionar);
            //move = do_move(generator); // Deberiamos cambiar por jugadaGolosa
            modificarTablero(this_game, board, move, true); //Mi jugada, va un true al final.
            send(move);
            msg = read_str();
        }

        msg = read_str(); // Lo hago para el próximo ciclo.
    }

    send("listo");
}

void jugadorGoloso2() {
    game_rules this_game;
    string msg, go_first;
    int move;

    msg = read_str();
    vector<double> mi_peso(this_game.c);
    vector<double> peso_contrincante(this_game.c);
    vector<double> C_parametros_peso_particionar(this_game.c);

    for(int i = 0; i < this_game.c; i++) {
        mi_peso[i] = pow(2,i);
        peso_contrincante[i] = -pow(2,i+1);
        C_parametros_peso_particionar[i] = i;
    }

    while (msg != "salir") {

        this_game.color = msg;
        this_game.opponent_color = read_str();

        this_game.columns = read_int();
        this_game.rows = read_int();
        this_game.c = read_int();
        this_game.p = read_int();

        vector<vector<string> > board(this_game.rows, vector<string>(this_game.columns, "-"));

        go_first = read_str();
        if (go_first == "vos") {
            move = jugadaGolosa2(this_game,board, mi_peso, peso_contrincante, C_parametros_peso_particionar);
            //move = do_move(generator); // Deberiamos cambiar por jugadaGolosa
            modificarTablero(this_game, board, move, true); //Mi jugada, va un true al final.
            send(move);
        }

        msg = read_str();
        while (msg != "ganaste" and msg != "perdiste" and msg != "empataron"){
            modificarTablero(this_game, board,stoi(msg), false); // Construyo la jugada del oponente.

            move = jugadaGolosa2(this_game,board, mi_peso, peso_contrincante, C_parametros_peso_particionar);
            //move = do_move(generator); // Deberiamos cambiar por jugadaGolosa
            modificarTablero(this_game, board, move, true); //Mi jugada, va un true al final.
            send(move);
            msg = read_str();
        }

        msg = read_str(); // Lo hago para el próximo ciclo.
    }

    send("listo");
}

void jugadorGoloso3() {
    game_rules this_game;
    string msg, go_first;
    int move;

    msg = read_str();
    vector<double> mi_peso(this_game.c);
    vector<double> peso_contrincante(this_game.c);
    vector<double> C_parametros(this_game.c);
    vector<double> C_parametros_oponente(this_game.c);
    vector<double> C_parametros_oponente_doble(this_game.c);
    vector<double> C_parametros_peso_particionar(this_game.c);

    for(int i = 0; i < this_game.c; i++) {
        mi_peso[i] = pow(10,i);
        peso_contrincante[i] = -pow(10,i+1);
        C_parametros[i] = pow(10,i);
        C_parametros_oponente[i] = -pow(10,i+1);
        C_parametros_oponente_doble[i] = -pow(10,i+1);
        C_parametros_peso_particionar[i] = pow(4,i);
    }
    C_parametros[this_game.c-1] = this_game.c*10000;
    C_parametros_oponente[this_game.c-1] = -this_game.c*10000;
    C_parametros_oponente[this_game.c-2] = -this_game.c*100;
    C_parametros_oponente_doble[this_game.c-3] = -this_game.c*100;
    peso_contrincante[this_game.c-2] = -this_game.c*100;
    peso_contrincante[this_game.c-1] = -this_game.c*100;

    while (msg != "salir") {

        this_game.color = msg;
        this_game.opponent_color = read_str();

        this_game.columns = read_int();
        this_game.rows = read_int();
        this_game.c = read_int();
        this_game.p = read_int();

        vector<vector<string> > board(this_game.rows, vector<string>(this_game.columns, "-"));

        go_first = read_str();
        if (go_first == "vos") {
            move = jugadaGolosa3(this_game,board, mi_peso, peso_contrincante, C_parametros, C_parametros_oponente, C_parametros_oponente_doble, C_parametros_peso_particionar);
            //move = do_move(generator); // Deberiamos cambiar por jugadaGolosa
            modificarTablero(this_game, board, move, true); //Mi jugada, va un true al final.
            send(move);
        }

        msg = read_str();
        while (msg != "ganaste" and msg != "perdiste" and msg != "empataron"){
            modificarTablero(this_game, board,stoi(msg), false); // Construyo la jugada del oponente.

            move = jugadaGolosa3(this_game,board, mi_peso, peso_contrincante, C_parametros, C_parametros_oponente, C_parametros_oponente_doble, C_parametros_peso_particionar);
            //move = do_move(generator); // Deberiamos cambiar por jugadaGolosa
            modificarTablero(this_game, board, move, true); //Mi jugada, va un true al final.
            send(move);
            msg = read_str();
        }

        msg = read_str(); // Lo hago para el próximo ciclo.
    }

    send("listo");
}
 
  
void jugadorGoloso4() {
    game_rules this_game;
    string msg, go_first;
    int move;

    msg = read_str();
    vector<double> mi_peso(this_game.c);
    vector<double> peso_contrincante(this_game.c);
    vector<double> C_parametros(this_game.c);
    vector<double> C_parametros_oponente(this_game.c);
    vector<double> C_parametros_oponente_doble(this_game.c);
    vector<double> C_parametros_peso_particionar(this_game.c);
    double parametro_oponente_gana = -100;


    vector<int> input_rival = {83 ,-77, -93 ,-86 ,-49, 62, 90, 63, 40, 72, 11, 67, -82 ,-62, -67, -29, 22, 69, 93, -11, -29 ,21 ,84 ,-98  };
    //cargo parametros de inputGrid (son los mejores obtenidos por metodo grid search )
    for(int i = 0; i < this_game.c; i++) {
        mi_peso[i] = input_rival[i];
    }
    for(int i = 0; i < this_game.c; i++) {
        peso_contrincante[i] = input_rival[i+this_game.c];
    }
    for(int i = 0; i < this_game.c; i++) {
        C_parametros[i] = input_rival[i+2*this_game.c];
    }
    for(int i = 0; i < this_game.c; i++) {
        C_parametros_oponente[i] = input_rival[i+3*this_game.c];
    }
     for(int i = 0; i < this_game.c; i++) {
        C_parametros_oponente_doble[i] = input_rival[i+4*this_game.c];
    }
    for(int i = 0; i < this_game.c; i++) {
        C_parametros_peso_particionar[i] = input_rival[i+5*this_game.c];
    }
   
    /*
    for(int i = 0; i < this_game.c; i++) {
        mi_peso[i] = pow(2,i);//Calculo el peso de todas las rectas de cada longitud mias.
        peso_contrincante[i] = -pow(2,i+1);//Calculo el peso de todas las rectas del oponente no tapadas.
        C_parametros[i] = pow(2,i);//Calculo el peso de todas las rectas de cada longitud mias.
        C_parametros_oponente[i] = -pow(2,i+1);//Calculo el peso de todas las rectas del oponente no tapadas.
        C_parametros_oponente_doble[i] = -pow(2,i+1);//Calculo el peso de todas las rectas del oponente no tapadas en ninguno de los dos extremos.
        C_parametros_peso_particionar[i] = pow(1.5,i);// Esto cuenta cuanto peso aporta haber particionado la fila para no poder hacer jugadas horizontales mas grandes que C.
    }
    */
    while (msg != "salir") {

        this_game.color = msg;
        this_game.opponent_color = read_str();

        this_game.columns = read_int();
        this_game.rows = read_int();
        this_game.c = read_int();
        this_game.p = read_int();

        vector<vector<string> > board(this_game.rows, vector<string>(this_game.columns, "-"));

        go_first = read_str();
        if (go_first == "vos") {
            move = jugadaGolosa4(this_game,board, mi_peso, peso_contrincante, C_parametros, C_parametros_oponente, C_parametros_oponente_doble, C_parametros_peso_particionar, parametro_oponente_gana);
            //move = do_move(generator); // Deberiamos cambiar por jugadaGolosa
            modificarTablero(this_game, board, move, true); //Mi jugada, va un true al final.
            send(move);
        }

        msg = read_str();
        while (msg != "ganaste" and msg != "perdiste" and msg != "empataron"){
            modificarTablero(this_game, board,stoi(msg), false); // Construyo la jugada del oponente.

            move = jugadaGolosa4(this_game,board, mi_peso, peso_contrincante, C_parametros, C_parametros_oponente, C_parametros_oponente_doble, C_parametros_peso_particionar, parametro_oponente_gana);
            //move = do_move(generator); // Deberiamos cambiar por jugadaGolosa
            modificarTablero(this_game, board, move, true); //Mi jugada, va un true al final.
            send(move);
            msg = read_str();
        }

        msg = read_str(); // Lo hago para el próximo ciclo.
    }

    send("listo");
}


int main(){
    //jugadorGoloso1();
    //jugadorGoloso2();
    //jugadorGoloso3();
    jugadorGoloso4();
    return 0;
}

 