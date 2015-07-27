#include <iostream>
#include <math.h> 
#include <iomanip>
#include <time.h>     
#include <stdio.h>
#include <stdlib.h> 
#include <fstream>
#include <sstream>
#include <string> 

using namespace std;

#define N 5							//Cantidad de trafico que atraviesa un semaforo en un turno.
#define cantNODOS 16				//Cantidad de esquinas que tiene el plano de la ciudad
#define cantSemaforos 36    		//Cantidad de semáforos que tiene en total la ciudad 

#include "Nodo.cpp"
#include "ColaVertices.cpp"
#include "HeapMinVehiculos.cpp"
#include "listasemaforos.cpp" 
#include "GrafoCiudad.cpp"

void generarTrafico(Grafo*,int,int,int,Listasemaforos*);
void simular(Grafo*,Listasemaforos*,Vehiculo*);				//Este método corre la simulación hasta que miAuto llega a destino
void verde(Semaforo *,Listasemaforos *);	//Metodo que permite el paso de vehiculos en la cantidad n por carril y envía, de ser posible,
											//los autos al siguiente semaforo destino. 
void recalculando(Grafo *, Vehiculo *);

int main(void){
	Grafo *ciudad=new Grafo();
	Listasemaforos *Semaforos=new Listasemaforos();
	Nodosemaforo *nodo;
	Semaforo *semAux;
	Vehiculo *miAuto = new Vehiculo("Auto",9999,15,0);      //Creo el vehiculo que pretendo conducir. Ruta por defecto: []
	int iteracion=0;
	string p;
	
	ciudad->launch(Semaforos);
	
	generarTrafico(ciudad,70,20,10,Semaforos);
	
	Semaforos->ubicarMiAuto(miAuto);

	miAuto->set_camino(ciudad->dijkstra(miAuto->get_origen(),miAuto->get_destino()));
	
	Semaforos->ordenar();
	
	ostringstream os;
	os << "##################################  COMIENZO DE LA SIMULACION DE TRÁFICO   ##################################"<<endl;
    os << "-----------------------ITERACION: " << iteracion << "-------------------------------------------" << endl;
    p = os.str();
	
	Semaforos->print_TODO_file(p,iteracion);		

	while(miAuto->get_posicionActual()!=miAuto->get_destino())
	{

		nodo = Semaforos->get_czo();
		iteracion++;
		while(nodo!=NULL){
			semAux = nodo->get_dato();
			verde(semAux,Semaforos);
			nodo = nodo->get_next();
		}
		Semaforos->ordenar();
		
		os.str(""); os.clear();
		os << endl<<endl<<"-----------------------ITERACION: " << iteracion << "-------------------------------------------" << endl;
		p = os.str();
		Semaforos->print_TODO_file(p,iteracion);
		
		if((iteracion%2)==0&&(miAuto->get_posicionActual()!= miAuto->get_destino())){	//Recalculo el recorrido de mi auto cada 2 iteraciones
			cout<<"Recalculando..."<<endl;
			ciudad->actualizarMA(Semaforos);
			cout<<"Pos_actual: "<<miAuto->get_posicionActual();
			recalculando(ciudad,miAuto);
		}
		
		cout<<endl<<"-----------------------ITERACION: "<<iteracion<<"-------------------------------------------"<<endl;
		miAuto->print_DatosVehiculo3();
	}
	
	Semaforos->print_TODO_file("\n\nEstado FINAL de los semaforos \n",iteracion);

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
	city->actualizarMA(S);
	
	///Verificar que la cantidad de vehiculos en el heap sea igual a la cantidad de vehiculos ingresados
}


//Despacha N vehículos hacia sus respectivos destinos si las colas en destino no están llenas. 
//Si así lo fuere los autos permanecen en su actual ubicación. 
void verde(Semaforo *semActual,Listasemaforos *S)
{	
	Vehiculo *vehiculo;				//Puntero al con el que se manejan los diferentes vehiculos 
	
	Vehiculo *aux = new Vehiculo("Auto",8888,0,15);	
	
	Semaforo *nextSemaforo;			//Puntero al semaforo al que intenta dirigirse "vehiculo"
	HeapMin *colaSem;				//Variable para almacenar un puntereo a la cola del Semaforo actual.
	
	int nextNodo =-1;					//Entero de referencia al próximo nodo del grafo al que quiero moverme
	int contVehiculos=0;			//contador para controlar la cantidad de vehículos que 'cruzan' un semaforo
	semActual->desbloquear();		//Si el semaforo quedó bloaqueado, se desbloquea aca.
	
	colaSem = semActual->get_ColaDelSemaforo();
	
	if(colaSem->heap_vacio()){ 	//Si la cola del semaforo no existe o está vacia:
	semActual->bloquear();							//el semaforo se bloquea para que la simulación continue.
	//cout<<"Se bloqueo el semaforo por cola_vacia"<<endl; 
	}
	if(colaSem == NULL){ 	//Si la cola del semaforo no existe o está vacia:
	semActual->bloquear();							//el semaforo se bloquea para que la simulación continue.
	//cout<<"Se bloqueo el semaforo por cola_NULL"<<endl; 
	}    						

	while(contVehiculos<N*(semActual->get_carriles())&&(!semActual->get_estado())) //Deja pasar n*carriles vehiculos
	{
		vehiculo=colaSem->first(); //Tomo el primer vehiculo de la cola del semaforo
		
		if(vehiculo!=NULL){

			if(vehiculo->get_posicionActual()==vehiculo->get_destino()){ //Si ya llegó al destino
				colaSem->extraer(); //Extraigo el vehiculo de la cola del semaforo, y no lo ubico en ningún otro lugar. 
				semActual->bloquear(); //Bloqueo el semaforo.
			}
			if(vehiculo->get_posicionActual()==vehiculo->get_origen()){ //Si todavía no se movió del origen: Primer movimiento
				vehiculo->put_recorrido(vehiculo->get_siguiente()); 	//Cargo el primer elemento (cabeza) en la cola de vertices recorrido
				vehiculo->borrar_cabeza(); 								//Si no borro el primer elemento, pierdo una iteracion moviendome a donde ya estoy
			}
			
			nextNodo=vehiculo->get_siguiente();
			if(nextNodo>=0) {
				nextSemaforo=S->buscar(nextNodo,semActual->get_ubicacion()); //S->buscar(NodoUbicacion, nodoProcedencia)

				if((!nextSemaforo->is_full())&&(nextSemaforo!=NULL)){ //Si el proximo semaforo NO está lleno y si existe:
					vehiculo = colaSem->extraer(); //Extraigo el vehiculo del semaforo actual
					nextSemaforo->rojo(vehiculo);  //Pongo en cola de 'nextSemaforo' a 'vehiculo'
					vehiculo->set_posicionActual(nextSemaforo->get_ubicacion());
					vehiculo->put_recorrido(vehiculo->get_siguiente()); //Cargo el elemento en la cola de vertices recorridos
					vehiculo->borrar_cabeza(); //Borro el salto que ya hizo el vehiculo en su camino.
					contVehiculos++;
					
					//if(vehiculo->get_matricula() == 9999) vehiculo->print_DatosVehiculo3();
				}
				else semActual->bloquear(); //El semaforo al que se dirigía estaba lleno o no existía
			}
			else semActual->bloquear(); //El semaforo al que se dirigía estaba lleno o no existía
		}
		else semActual->bloquear(); //El vehiculo no existe, seguramente la cola del semaforo esta vacía. 
	}
}

void recalculando(Grafo *city, Vehiculo *myCar)
{
	city->dijkstra(myCar->get_posicionActual(),myCar->get_destino())->print();
	//~ myCar->set_camino(city->dijkstra(myCar->get_posicionActual(),myCar->get_destino()));
}



