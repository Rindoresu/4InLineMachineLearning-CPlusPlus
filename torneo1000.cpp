#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>


using namespace std;

ofstream output;
ifstream input;
string msg;
int cantPartidasG();
 
 
int main(){
//int parametros = 4*cantColumnas;

int cantColumnas = 7;
int cantFilas = 6;
int C = 4;
int cantFichas = 21;
int cantPartidas = 2000;
	
	system("g++ -g -std=c++11 -o jugGrid jugGrid_ej2a.cpp");

	system("g++ -g -std=c++11 -o random_player random_player.cpp");
	//system("g++ -g -std=c++11 -o JugadorGoloso_ej2a JugadorGoloso_ej2a.cpp");

	string b = "python2 c_linea.py --blue_player ./jugGrid --red_player ./random_player --columns " + to_string(cantColumnas) + " --rows " + to_string(cantFilas);
	b = b + " --p " + to_string(cantFichas) + " --c " + to_string(C) + " --iterations " + to_string(cantPartidas) + " --first rojo";
	const char* comando = b.c_str();

	/*
	cantPartidas = 1000;
	for(int i=0; i<cantPartidas; i++){
		system(comando);
	}
	*/
	system(comando);
	//outputGrid.txt contiene el resultado de las cantPartidas jugadas, 
	//con resultado: gane, perdi , empate
	//devuelvo porcentaje de ganadas con esta configuracion de pesos
	int porcentajeAux = cantPartidasG();
	cout<<"porcentaje de partidas ganadas por jugador entrenado por grid es: "<<porcentajeAux<<endl;
				 
}		

int cantPartidasG(){
	 
	int ganadas = 0;
	int empatadas = 0;
	int perdidas = 0;
	int count = 0;
	ifstream file("outputGrid.txt");

	while (getline(file, msg)){
        count++;
        if (msg == "ganaste") ganadas++;
		if (msg == "perdiste") perdidas++;
		if (msg == "empataron") empatadas++;
	}
	if(count == 0)
		count = 1;
	remove("outputGrid.txt");

	return (ganadas*100/count);
}
 