/*
heap-MIN de Nodo (vertices) que hace de cola de prioridad para utilizar en el algoritmo de dijkstra.
El nodo adyacente con el menor peso será el que se ubica en la cima del heap.
*/

/*
DATOS RELEVANTES???
*/

class HeapMinVertices{
    protected:
            Nodo *Vertices[cantNODOS];											
            int ptr;					//puntero (índice) al último elemento insertado. (-1) cuando está vacío. 
										
			int hDer(int);				//Mediante aritmética de índices devuelven el respectivo "pariente" del indice del argumento
			int hIzq(int);
			int padre(int);  
			bool esHijoDer(int);
			void swap(int, int);		//intercambia dos elementos del arreglo. Recibe sus indices como argumentos.      

    public:
            HeapMinVertices();                                   				//constructor heap vacío
            HeapMinVertices(Nodo*);                      					//constructor heap con primer elemento
            ~HeapMinVertices(void){};                                           //destructor del heap
            bool insertar(Nodo *);              					//recibe un Vehiculo y lo inserta en el heap según su prioridad. Devuelve true si pudo hacerlo.
            Nodo *extraer(); 										//Devuelve el elemento de mayor prioridad y re-construye el heap.  
			                                        
            bool heap_lleno(void) {return ptr==(cantNODOS-1);};              //devuelve 1 si el heap está lleno. 0 si tiene al menos un lugar.
            bool empty(void) {return ptr==-1;};
            void set_cima(Nodo *newCima);
			Nodo *get_cima(void) {return Vertices[0];};  
			void print_heap();  
			            
					
};

HeapMinVertices::HeapMinVertices(){
	for(int i=0;i<cantNODOS;i++) Vertices[i]=NULL;
	ptr=-1; 
}

HeapMinVertices::HeapMinVertices(Nodo *n){

	for(int i=0;i<cantNODOS;i++) Vertices[i]=NULL;
	ptr=-1;		
	insertar(n);
	
}

int HeapMinVertices::hDer(int i){
	if(2*i+2<cantNODOS && i>=0) return 2*i+2;
	else return i;
}
int HeapMinVertices::hIzq(int i){
	if(2*i+1<cantNODOS && i>=0) return 2*i+1;
	else return i;
}
int HeapMinVertices::padre(int i){
	if(floor((i-1)/2)<cantNODOS && i>=0)return floor((i-1)/2);
	else return i;
}              

bool HeapMinVertices::esHijoDer(int i){
	float parteDec;
	
	parteDec=(i-1)/2;
	parteDec=parteDec-(int)parteDec;
	
	if(parteDec>0)return true;
	else return false;
}  

void HeapMinVertices::swap(int a,int b){
	Nodo *aux;
	
	aux=Vertices[a];
	Vertices[a]=Vertices[b];
	Vertices[b]=aux;
}

bool HeapMinVertices::insertar(Nodo *nuevo){
	int posActual;
	if(heap_lleno()){
		return false;
	}
	else{
		ptr++;		//Incremento el puntero que indica donde está el último elemento en el array
		Vertices[ptr]=nuevo; 		//Almaceno en esa posicion del array el nuevo Nodo
		posActual=ptr;			//Guardo en una variable auxiliar el puntero de la posición en el array del Nodo que acabo de guardar	//posición actual del Nodo insertado.
		while(Vertices[posActual]->get_peso()<Vertices[padre(posActual)]->get_peso()){ //Mientras la prioridad del Nodo ingresado sea mayor a la de su padre, intercambiar estos y volver a preguntar
			if(esHijoDer(posActual)){			//Si es hijo derecho, hago un doble swap triangular
				swap(posActual,padre(posActual));
				swap(posActual,posActual-1); //hermano izquierdo
				posActual=padre(posActual);		
			}
			else{									//Si es hijo izquierdo, simplemente cambio padre con hijo
				swap(posActual,padre(posActual));  									//VER QUE PASA SI padre() DEVUELVE -2
				posActual=padre(posActual);	
			}			
		}
		return true;
	}	
}

Nodo *HeapMinVertices::extraer(){
	if(empty()){
		cout<<"No hay Vertices por extraer"<<endl;
		return NULL;
	}
	else{
		Nodo *cima=Vertices[0];			//almaceno el elemento a devolver, para hacerlo al finalizar el metodo
		for(int i=0;i<=ptr;i++){
			Vertices[i]=Vertices[i+1];
		}
		ptr--;
		return cima;
	}
}

void HeapMinVertices::print_heap(){
	if(empty()) cout<<"El Heap esta vacio"<<endl;
	else{
		cout<<"VERTICES ALMACENADOS EN EL HEAP :"<<endl;
		for(int i=0;i<=ptr;i++)	Vertices[i]->print();
	}
}
