/* La clase Grafo modeliza la ciudad sobre la que se desarrolla la
 * simulación. La implementación se realiza mediante una Matriz de 
 * Adyacencia, una cola de prioridad y una cola fifo estandar.
 * Dentro de esta clase esta implementada el algoritmo de Dijkstra, el
 * cual retorna el camino más corto desde el punto de vista de los 
 * pesos relativos, calculados en base al tráfico. 
 * 
 * */
#include <limits>
#include "HeapMinVertices.cpp"

int inf = std::numeric_limits<int>::max();

class Grafo{
	private:
		int MA[cantNODOS][cantNODOS];		//Matriz de Adyacencia del Grafo que representa la ciudad.
		HeapMinVertices *priorityQ;			//Cola de prioridad (peso) donde almaceno temporalmente nodos adyacentes para trabajarlos. 
		ColaVertices *Q;					//Cola de vertices que voy a retornar como resultado del algoritmo, 
											//con el camino óptimo de 'origen' a 'destino'

		int distancia[ cantNODOS ];      	//distancia[ u ] distancia de vértice inicial a vértice con ID = u
		bool visitados[ cantNODOS ];      	//para vértices visitados	
		int previo[ cantNODOS ];         	//para la impresion de caminos
		
		void init();						//Inicializa los valores de las estructuras de datos usadas en dijkstra()
		void relajacion( int,int,int);		//Actualiza distancias[i] haciendo el acumulado de distancias desde el origen
		void ruta( int);					//Almacena en ColaVertices* la ruta optima a seguir desde origen a destino.
	public:
		Grafo(); 
		void launch(Listasemaforos*);		//Inicializa la "ciudad" leyendo la config. desde un archivo.
		ColaVertices* dijkstra( int, int);	//Algoritmo de Dijkstra. Recibe origen y destino y retorna la ruta óptima (ColaVertices)
		void actualizarMA(Listasemaforos*);	//Recibe la lista de semaforos para tomar los datos del tráfico desde la congestión
											//de estos y actualizar los pesos de los arcos del grafo.
		void print();
	
};

Grafo::Grafo()
{				
}

/*
 * Este método lee un archivo que contiene la configuración de la ciudad y carga esos datos en memoria en la matriz de adyacencia
 * MA. Si no hay camino, se setea como "infinito" el peso de la arista correspondiente.
*/
void Grafo::launch(Listasemaforos* s)
{	
    FILE *ciudad;
    ciudad = fopen("Ciudad16.csv", "r");
    int dato = fgetc(ciudad);
    for(int i=0 ; !(feof(ciudad)) ; i++){
        for(int j=0 ; dato != 10 ; dato = fgetc(ciudad)) 				//lectura en ascii... "10 = \n" ... "44 = , "
        {	
        	switch(dato){
        		case 48:			// 48(ascii) = 0(decimal)
        			MA[i][j] = 0;
					j++;
					break;
				case 49:			//49(ascii) = 1(decimal). Construye semáforo de 1 carril.
					MA[i][j] = 1;
					s->insertar(new Semaforo(1,j,i)); //Semaforo(cantDeCarriles,IDSemaforo,procedencia)
					j++;
					break;
				case 50:			//50(ascii) = 2(decimal). Construye semáforo de 2 carriles.
					MA[i][j] = 1;
					s->insertar(new Semaforo(2,j,i));
					j++;
					break;
				case 51:			//51(ascii) = 3(decimal). Construye semáforo de 3 carriles.
					MA[i][j] = 1;
					s->insertar(new Semaforo(3,j,i));
					j++;
					break;
				case 110:
					MA[i][j] = inf; //(inf=2147483647)
					j++;
					break;
				default: //Agregar los demás caracteres que captura e indicar msj de error si hay algo mas que no debería estar.
					break;
			}
        }
        dato = fgetc(ciudad);
    }
    fclose(ciudad);
}

//función de inicialización de variables para usar en dijkstra()
void Grafo::init()
{
	if(priorityQ != NULL) delete priorityQ;
	if(Q != NULL) delete Q;
	
	priorityQ=new HeapMinVertices();	
	Q=new ColaVertices();	
	
    for( int i = 0 ; i <cantNODOS ; ++i ){
        distancia[ i ] = inf;  //inicializamos todas las distancias con valor infinito
        visitados[ i ] = false; //inicializamos todos los vértices como no visitados
        previo[ i ] = -1;      //inicializamos el previo del vertice i con -1
    }
}

//Paso de relajacion
void Grafo::relajacion( int actual , int adyacente , int peso ){
    //Si la distancia del origen al vertice actual + el peso de su arista es menor a la distancia del origen al vertice adyacente
    if( distancia[ actual ] + peso < distancia[ adyacente ] ){
        distancia[ adyacente ] = distancia[ actual ] + peso;  //relajamos el vertice actualizando la distancia
        previo[ adyacente ] = actual;                         //a su vez actualizamos el vertice previo
        priorityQ->insertar( new Nodo(adyacente,distancia[adyacente])); //agregamos adyacente a la cola de prioridad
    }
}

void Grafo::ruta( int dest){
    if( previo[ dest ] != -1 )    		//si aun poseo un vertice previo
        ruta( previo[ dest ]);  		//recursivamente sigo explorando
    Q->agregar(dest);      				//terminada la recursion, agrego los elementos a la cola de vertices
}


ColaVertices* Grafo::dijkstra( int origen,  int destino)
{											
    init(); 											//inicializamos los arreglos
    
    priorityQ->insertar( new Nodo(origen,0)); 			//Insertamos el vértice inicial en la Cola de Prioridad
    distancia[ origen ] = 0;      						//Este paso es importante, inicializamos la distancia del inicial como 0
    int actual , adyacente , peso;
    while( !priorityQ->empty() ){                   	//Mientras cola no este vacia
        actual = priorityQ->extraer()->get_dato();      //Obtengo de la cola el nodo con menor peso, en un comienzo será el inicial. Elimino el elemento de la cola.
        if( visitados[ actual ] ) continue; 			//Si el vértice actual ya fue visitados entonces sigo sacando elementos de la cola
        visitados[ actual ] = true;         			//Marco como visitados el vértice actual

        for( int j = 0 ; j < cantNODOS ; j++ ){ 			//reviso sus adyacentes del vertice actual; fila[actual] de MA
            if(MA[actual][j]>0 && MA[actual][j]<inf){ 		//Si es 0 es el elemento actual, si es inf no hay arco.
            	adyacente = j;   							//id del vertice adyacente (j+1 porque el array arranca en 0)	
            	peso = MA[actual][j];        				//peso de la arista que une actual con adyacente ( actual , adyacente )
            	if( !visitados[ adyacente ] ){        		//si el vertice adyacente no fue visitados
					relajacion( actual , adyacente , peso ); 	//realizamos el paso de relajacion
            	}
			}   
        }
    }
	ruta(destino);
	
	return Q;
    
}

void Grafo::print()
{
	cout<<"La matriz de adyacencia MA del grafo 'ciudad' es:"<<endl<<endl;
	int inf = std::numeric_limits<int>::max();
	for(int i=0;i<cantNODOS;i++){
		for(int j=0;j<cantNODOS;j++) {
			if(MA[i][j]==inf) cout<<setw(5)<<setiosflags(ios::internal)<<"inf";
			else {
				
				cout<<setw(5)<<setiosflags(ios::internal)<<MA[i][j];
			}
		}
		cout<<endl;
	}	
}

void Grafo::actualizarMA(Listasemaforos *S)
{	
	Nodosemaforo *nodo = S->get_czo();
	Semaforo *aux=NULL;
	
	while(nodo!=NULL)
	{
		aux = nodo->get_dato();
		if(aux->congestion()!= 0) MA[aux->get_procedencia()][aux->get_ubicacion()]=aux->congestion(); //Si no me rompe MA
		nodo = nodo->get_next();
	}
		
}
