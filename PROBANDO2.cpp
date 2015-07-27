#include <iostream>
#include <math.h> 
//#include <string>
#include <iomanip>
#include <time.h>     
//#include <sstream>
#include <stdio.h>
#include <stdlib.h> 
#include <fstream>

using namespace std;

#define N 5							//cantidad de trafico que atraviesa un semaforo en un turno.
#define cantNODOS 16					//Cantidad de esquinas que tiene el plano de la ciudad
#define cantSemaforos 36    			//AJUSTAR valor 

#include "Nodo.cpp"
#include "ColaVertices.cpp"
#include "HeapMinVehiculos.cpp"
#include "listasemaforos.cpp" 
#include "GrafoCiudad.cpp"

void generarTrafico(Grafo *city, int nAutos, int nPatrulleros, int nAmbulancias,Listasemaforos* S); 

int main(void){
	
	Grafo *ciudad=new Grafo();
	Listasemaforos *S=new Listasemaforos();
	ciudad->launch(S);
	
	//generarTrafico(ciudad,85,10,5,S);
	
	Vehiculo *v1=new Vehiculo();
	Vehiculo *v2=new Vehiculo();
	Vehiculo *v3=new Vehiculo();
	Vehiculo *miAuto = new Vehiculo("Auto",9999,0,15);      //Creo el vehiculo que pretendo conducir
	
	ofstream prueba;
    prueba.open("Prueba.txt");
    
    prueba<<cout<<"Hola mundo"<<endl;
    
    

	return 0;
}

void generarTrafico(Grafo *city, int nAutos, int nPatrulleros, int nAmbulancias,Listasemaforos* S)
{
	///Verificar que la suma de todos los vehiculos sea menor a la capacidad acumulada de todos los semaforos.
	
	for(int i=0;i<nAutos;i++){
		Vehiculo *a=new Vehiculo("Auto");
		S->ubicarAleatorio(a);
		a->set_camino(city->dijkstra(a->get_origen(),a->get_destino()));
	}
	
	for(int i=0;i<nPatrulleros;i++){
		Vehiculo *p=new Vehiculo("Patrullero");
		S->ubicarAleatorio(p);
		p->set_camino(city->dijkstra(p->get_origen(),p->get_destino()));
	}
	
	for(int i=0;i<nAmbulancias;i++){
		Vehiculo *am=new Vehiculo("Ambulancia");
		S->ubicarAleatorio(am);
		am->set_camino(city->dijkstra(am->get_origen(),am->get_destino()));
	}
	
	S->ordenar();
	
	///Verificar que la cantidad de vehiculos en el heap sea igual a la cantidad de vehiculos ingresados
}
