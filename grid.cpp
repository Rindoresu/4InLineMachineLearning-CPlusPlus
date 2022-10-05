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


void mostrarVector(vector<int> v);
void copiarVector(vector<int>& v1, vector<int> v2);
void inicializarVector(vector<int>& v1, int parametros,int valor, int k);
void cargarOutput(vector<int> pesos, vector<int>& resultado, int indice1, int indice2, int inc1, int inc2);
int cantPartidasG(int cantidadPartidas);
void cargarPesos(vector<int>&, int, int);
void vaciarVector(vector<int>&);

void gridRecorro(int cantColumnas, int cantFilas, int C, int cantFichas, int cantPartidas, string nombre);

int main(){
	gridRecorro(7, 6, 4, 21, 30, "ID");
	return 0;
}


void gridRecorro(int cantColumnas, int cantFilas, int C, int cantFichas, int cantPartidas , string nombre){
	//version goloso 4
	int parametros = (6*C)+1;

	system("g++ -g -std=c++11 -o jugGrid jugGrid_ej2a.cpp");
	system("g++ -g -std=c++11 -o random_player random_player.cpp");

	string b = "python2 c_linea.py --blue_player ./jugGrid --red_player ./random_player --columns " + to_string(cantColumnas) + " --rows " + to_string(cantFilas);
	b = b + " --p " + to_string(cantFichas) + " --c " + to_string(C) + " --iterations " + to_string(cantPartidas) + " --first rojo";
	const char* comando = b.c_str();

	//inputGrid.txt tiene parametros que jugador entrenado usara en su estrategia
	output.open("inputGrid.txt");
	 
	int aux2;
        for(int j=0; j<6 ; j++){
                if(j==0 || j==2 || j==5){
                        for(int x = 0; x < C; x++){
                            aux2 = pow(2,x);
                                output << aux2<< " ";

                        }
                } else {
                        for(int x = 0; x < C; x++){
                                aux2 = pow(2,x);
                                output << -aux2<< " ";

                        }
                }
        }       
        aux2 = pow(2,C);
    output<< -aux2;
		 	 
    output.close();

	int r = 6*C;
	
	vector<int> resultado;

	vector<int> pesos;
	int porcentaje = 0;
	int repeticiones = 0;

	 

	while(repeticiones < 10){
		cout << repeticiones << endl;
		//en la 1era iteracion pesos contiene valores random entre 0 y 99
		//en siguientes iteraciones pesos se cargan con mejores pesos de iteracion anterior
		cargarPesos(pesos, parametros,repeticiones);

		int k = (rand()%10)+1;
		inicializarVector(resultado, parametros, C, k);
		 
		for(int z = 0; z < r; z+=2){
			int peso1 = 0;
			int peso2 = 0;
			porcentaje = 0;
			//juego 900*cantPartidas cantidad de partidos para evaluar cada par de pesos 
			for(int i = -15; i < 15; i++){
				for(int j = -15; j < 15; j++){
					//comunico al jugador a entrenar los mejores z-1 pesos registrados hasta 
					//el momento, junto a una variacion del par de pesos evaluados
					//array resultado mantiene desde posicion z a los valores fijos iniciales
					//mientras que array pesos mantiene desde z a los pesos elegidos iniciales
					//entrenamos con los parametros de resultado
					cargarOutput(pesos, resultado, z, z+1, i, j);
					system(comando);
					system("rm log/player_communicator_azul.log ");
					system("rm log/player_communicator_rojo.log ");

					//outputGrid.txt contiene el resultado de las cantPartidas jugadas, 
					//con resultado: gane, perdi , empate
					//devuelvo porcentaje de ganadas con esta configuracion de pesos
					int porcentajeAux = cantPartidasG(cantPartidas);
				
					//me quedo con la configuracion que mayor porcentaje de ganadas ofrece hasta el momento
					if(porcentajeAux > porcentaje){
						peso1 = pesos[z]+i;
						peso2 = pesos[z+1]+j;
						porcentaje = porcentajeAux;
					}
				}
			}

			//guardo el mejor par de pesos obtenido
			pesos[z] = peso1;
			pesos[z+1] = peso2;
			cargarOutput(pesos, resultado, z, z+1, 0, 0);

		}
		//ultimo parametro
		int pesoz = 0;
		porcentaje = 0;
		for(int i = -15; i<15; i++){
				
			cargarOutput(pesos, resultado, r, r-1, i, 0);
				
			system(comando);
			system("rm log/player_communicator_azul.log ");
			system("rm log/player_communicator_rojo.log ");

			int porcentajeAux = cantPartidasG(cantPartidas);
				
			remove("outputGrid.txt");
			if(porcentajeAux > porcentaje){
				pesoz = pesos[r] + i;
				porcentaje = porcentajeAux;
			}
				
		}
		pesos[r] = pesoz;
		cargarOutput(pesos, resultado, r, r, 0, 0);
			
		vaciarVector(resultado);
		vaciarVector(pesos);
		repeticiones++;
	}
}


void vaciarVector(vector<int>& v1){
	while(v1.size() > 0){
		v1.pop_back();
	}
}

void mostrarVector(vector<int> v){
	for(int x = 0; x < v.size();x++){
		cout << v[x] << " ";
	}
	cout << endl;
}

void copiarVector(vector<int>& v1, vector<int> v2){
	for(int i = 0; i < v2.size(); i++){
		v1[i] = v2[i];
	}
}

void inicializarVector(vector<int>& v1, int parametros, int valor, int k){
	
	for(int j=0; j < parametros; j++){
		v1.push_back(k);
	}
	
}

void cargarOutput(vector<int> pesos, vector<int>& resultado, int indice1, int indice2, int inc1, int inc2){
	//al sumarle inc1 o inc2 se varia el valor del peso actual evaluado

	resultado[indice1] = pesos[indice1] + inc1;
	resultado[indice2] = pesos[indice2] + inc2;
	output.open("inputGrid.txt");
	//escribo en inputGrid.txt el peso evaluado actual junto a los mejores pesos hallados
	//esta informacion la usa el jugador a entrenar
	for(int x = 0; x < resultado.size(); x++){
		output << resultado[x] << " ";
	}
	output.close();
}

int cantPartidasG(int cantidadPartidas){
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
	if (count == 0)
		count = 1;
	// borro outputGrid.txt para volver a usarlo
	file.close();
	remove("outputGrid.txt");
	return (ganadas*100/count);
}

void cargarPesos(vector<int>& v1, int parametros, int repeticiones){
	input.open("inputGrid.txt");

	string str = to_string(repeticiones);

	string nombreArchivo = "pesosParciales"+ str +".txt";
	const char* archivo = nombreArchivo.c_str();
	output.open(archivo);

	for(int i = 0; i< parametros;i++){
		int aux;
		input >> aux;
		v1.push_back(aux);
		output << aux << " ";

	}	
	input.close();
	 	
	output.close();
}






