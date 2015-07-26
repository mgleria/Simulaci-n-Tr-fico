#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <sstream>
#include <math.h> 
#include <time.h>
#include <stdio.h>
#include <fstream>

using namespace std;

#define N 10							//cantidad de trafico que atraviesa un semaforo en un turno.
#define cantNODOS 16					//Cantidad de esquinas que tiene el plano de la ciudad
#define cantSemaforos 36    			//AJUSTAR valor al numero de semaforos del plano mas grande 

#include "Nodo.cpp"
#include "ColaVertices.cpp"
#include "HeapMinVehiculos.cpp"  		//Tiene dentro #include "Vehiculo.cpp"

#include "listasemaforos.cpp"  			//Tiene dentro #include "Semaforo.cpp"
#include "GrafoCiudad.cpp"

void ordenarSemaforos(Listasemaforos*);
void recalculando(Grafo*, Vehiculo*); 				//Calcula
void generarTrafico(Grafo *,int,int,int,Listasemaforos*);
void simular(Grafo*,Listasemaforos*,Vehiculo*);				//Este método corre la simulación hasta que miAuto llega a destino
void verde(Semaforo *,Listasemaforos *);						//Metodo que permite el paso de vehiculos en la cantidad n por carril y envía, de ser posible,
														//los autos al siguiente semaforo destino. 

int main(void){
	Grafo *ciudad=new Grafo();
	Listasemaforos *Semaforos=new Listasemaforos();
	
	Vehiculo *miAuto = new Vehiculo("Auto",1234,0,15);      //Creo el vehiculo que pretendo conducir

	ciudad->launch(Semaforos);		//Leo desde archivo. Armo el escenario. Genero MA y construyo y cargo en la lista los semaforos.
	
	generarTrafico(ciudad,10,0,0,Semaforos);			//Función que genera tráfico aleatorio
	
	//Semaforos->print_TODO();

	simular(ciudad,Semaforos,miAuto);
	    
	return 0;
}

void generarTrafico(Grafo *city, int nAutos, int nPatrulleros, int nAmbulancias,Listasemaforos* S)
{
	///Verificar que la suma de todos los vehiculos sea menor a la capacidad acumulada de todos los semaforos.
	
	for(int i=0;i<nAutos;i++){
		Vehiculo *a=new Vehiculo("Auto");
//		a->print_DatosVehiculo();
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
	
	///Verificar que la cantidad de vehiculos en el heap sea igual a la cantidad de vehiculos ingresada
	
}


void simular(Grafo *city,Listasemaforos *S,Vehiculo *myCar)
{	
	Nodosemaforo *nodo;		//nodo con el que recorro la lista de semaforos
	Semaforo *miSem; 		//Semaforo donde se va a ubicar 'myCar'
	Semaforo *SemAux; 		//Semaforo auxiliar de la simulacion
	int iteracion=0; 		//Cantidad de Iteraciones
	
	miSem = S->buscar(1,0);  ///VER COMO RESOLVER ESTO. NO ESTOY BUSCANDO EL SEMAFORO INICIAL
	
	if(miSem->rojo(myCar))	//Al ubicar 'myCar' en el semaforo de Origen, pasa a manejarse como un Vehiculo mas de la simulación.
	{
								
		while(myCar->get_destino() != myCar->get_posicionActual()){ //Mi auto llegó a destino?? (Condicion de parada de la simuación)
			
			iteracion++;
			ordenarSemaforos(S);			//Ordeno los semáforos en función del tráfico			
			nodo = S->get_czo();			//seteo en 'nodo' el nuevo czo de la lista de semaforos 
			city->actualizarMA(S);			//actualizo MA para que dijkstra() tenga los pesos adecuados en los arcos del grafo

			while(nodo!=NULL)				//Este ciclo es el que recorre la lista de semaforos y le da VERDE una vez a cada 
			{								//semaforo de la ciudad.
				SemAux = nodo->get_dato();
				verde(SemAux,S);  			
				nodo = nodo->get_next();     
			}		
			
			if((iteracion%2)==0){			//Recalculo el recorrido de mi auto cada 2 iteraciones
				recalculando(city,myCar);
			}
			
			ordenarSemaforos(S);			//Vuelvo a ordenar los semaforos en función de su congestion.
			
			cout<<"Camino recorrido en iteracion Nro. "<<iteracion<<": "<<endl;
			myCar->print_camino();
		}
	}
	else{
		cout<<"Semaforo Inicial lleno. Simulacion no iniciada"<<endl;
	}
}

//Despacha n vehículos hacia sus respectivos destinos si las colas en destino no están llenas. 
//Si así lo fuere los autos permanecen en su actual ubicación. 
void verde(Semaforo *semActual,Listasemaforos *S)
{	
	Vehiculo *vehiculo;
	Semaforo *nextSemaforo;
	HeapMin *colaSem;				//Variable para almacenar un puntereo a la cola del Semaforo actual.
	int nextNodo;					//Entero de referencia al próximo nodo del grafo al que quiero moverme
	int contVehiculos=0;			//contador para controlar la cantidad de vehículos que 'cruzan' un semaforo
	bool semBloqueado=false;		//bandera para bloquear un semaforo
	
	colaSem = semActual->get_ColaDelSemaforo();
	if(colaSem == NULL || colaSem->heap_vacio()) 	//Si la cola del semaforo no existe o está vacia, el semaforo se bloquea para que
	semBloqueado = true;   							//la simulación continue.

	while(contVehiculos<N*semActual->get_carriles()&&(!semBloqueado)){ //Deja pasar n*carriles vehiculos
		
		vehiculo=colaSem->first(); //Tomo el primer vehiculo de la cola del semaforo
		if(vehiculo!=NULL){
			if(vehiculo->get_posicionActual()==vehiculo->get_origen()){ //Si todavía no se movió del origen: Primer movimiento
				vehiculo->put_recorrido(vehiculo->get_siguiente()); //Cargo el primer elemento en la cola de vertices recorridos
				
				//cout<<"primer get_siguiente() :"<<vehiculo->get_siguiente()<<endl;
				
				vehiculo->borrar_siguiente(); //Si no borro el primer elemento, pierdo una iteracion moviendome a donde ya estoy
			}
			nextNodo=vehiculo->get_siguiente();
//			cout<<"segundo get_siguiente() :"<<nextNodo<<endl;
			
			nextSemaforo=S->buscar(semActual->get_ubicacion(),nextNodo);
			
//			cout<<"semActual->get_ubicacion() :"<<semActual->get_ubicacion()<<endl;
//			cout<<"nextNodo "<<nextNodo<<endl;
//			cout<<endl<<"nextSemaforo :"<<endl;
//			nextSemaforo->print();

			if((!nextSemaforo->is_full())&&(nextSemaforo!=NULL)){ //Si el proximo semaforo NO está lleno y si existe:
				nextSemaforo->rojo(colaSem->extraer()); //Extraigo el vehiculo del semaforo actual y lo paso al proximo
				vehiculo->set_posicionActual(nextSemaforo->get_ubicacion());
				
				vehiculo->put_recorrido(semActual->get_ubicacion()); //Cargo el elemento en la cola de vertices recorridos
				vehiculo->borrar_siguiente(); //Borro el salto que ya hizo el vehiculo en su camino.
				
				contVehiculos++;
			}
			else semBloqueado=true; //El semaforo al que se dirigía estaba lleno o no existía
		}
		else semBloqueado=true; //El vehiculo no existe, seguramente la cola del semaforo esta vacía. 
	}
}

void recalculando(Grafo *city, Vehiculo *myCar)
{
	city->actualizarMA(S);
	myCar->set_camino(city->dijkstra(myCar->get_posicionActual(),myCar->get_destino()));
}

void ordenarSemaforos(Listasemaforos *S)
{
	S->ordenar();
}
