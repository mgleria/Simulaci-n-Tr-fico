#include <iostream>
#include <math.h> 
//#include <string>
#include <iomanip>
//#include <time.h>     VERIFICAR SI LOS UTILIZO
//#include <sstream>
#include <stdio.h>
#include <stdlib.h> 
#include <fstream>

using namespace std;

#define N 10							//cantidad de trafico que atraviesa un semaforo en un turno.
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
	Listasemaforos *Semaforos=new Listasemaforos();
	
	ciudad->launch(Semaforos);
	
	ciudad->print();
	
	generarTrafico(ciudad,50,10,5,Semaforos);

	Semaforos->print();
	
	Vehiculo *v1=new Vehiculo();
	Vehiculo *v2=new Vehiculo();
	Vehiculo *v3=new Vehiculo();
	Vehiculo *miAuto = new Vehiculo("Auto",9999,0,15);      //Creo el vehiculo que pretendo conducir
	
	v1->print_DatosVehiculo();
	v2->print_DatosVehiculo();
	v3->print_DatosVehiculo();
	miAuto->print_DatosVehiculo();
	
	Semaforos->ubicarAleatorio(v1);
	Semaforos->ubicarAleatorio(v2);
	Semaforos->ubicarAleatorio(v3); 
	cout<<endl<<"Semaforos->buscar(1,0)->rojo(miAuto); :"<<Semaforos->buscar(1,0)->rojo(miAuto)<<endl;
	
	//Calculo el camino a recorrer de cada uno
	v1->set_camino(ciudad->dijkstra(v1->get_origen(),v1->get_destino()));
	v2->set_camino(ciudad->dijkstra(v2->get_origen(),v2->get_destino()));
	v3->set_camino(ciudad->dijkstra(v3->get_origen(),v3->get_destino()));
	miAuto->set_camino(ciudad->dijkstra(miAuto->get_origen(),miAuto->get_destino()));
	
	cout<<endl<<"-------------------DSP de UBICARALEATORIO------------------------------------------"<<endl;
	v1->print_DatosVehiculo();
	cout<<"Camino:   ";
	v1->get_camino()->print();
	cout<<endl;
	cout<<"Recorrido:   ";
	v1->get_recorrido()->print();
	cout<<endl;
	v2->print_DatosVehiculo();
	cout<<"Camino:   ";
	v2->get_camino()->print();
	cout<<endl;
	cout<<"Recorrido:   ";
	v2->get_recorrido()->print();
	cout<<endl;
	v3->print_DatosVehiculo();
	cout<<"Camino:   ";
	v3->get_camino()->print();
	cout<<endl;
	cout<<"Recorrido:   ";
	v3->get_recorrido()->print();
	cout<<endl;	
	miAuto->print_DatosVehiculo();
	cout<<"Camino:   ";
	miAuto->get_camino()->print();
	cout<<endl;
	cout<<"Recorrido:   ";
	miAuto->get_recorrido()->print();
	cout<<endl;	

	
	Semaforos->ordenar();
	ciudad->actualizarMA(Semaforos);
	
	cout<<endl<<"-----------------------MA ACTUALIZADA---------------------------------------------"<<endl;
	
	ciudad->print();
	
	Semaforos->print_TODO();
	 
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
