/*
heap-MIN implementado en base a la prioridad de los vehiculos
*/

/*
PRIORIDAD VEHICULOS (de mayor a menor)
AMBULANCIAS	-> 1
PATRULLEROS -> 2
AUTOS 		-> 3
*/
#include "Vehiculo.cpp"
#include <vector> 

class HeapMin{
    protected:
            vector<Vehiculo*> Vehiculos;	
								
			int ptrPat;					//puntero (índice) al último elemento insertado tipo Patrullero. Iniciado en (-1) 
			int ptrAmb;					//puntero (índice) al último elemento insertado tipo Ambulancia. Iniciado en (-1) 
			int capacidad; 				//capacidad predeterminada para el heap en función del número de carriles
											
    public:
            HeapMin(int);               //constructor heap vacío
            HeapMin(int,Vehiculo*);     //constructor heap con primer elemento
            ~HeapMin(void){};           //destructor del heap
            bool insertar(Vehiculo *);  //recibe un Vehiculo y lo inserta en el heap según su prioridad. Retorna true si pudo hacerlo.
            Vehiculo *extraer(); 		//Devuelve el elemento de mayor prioridad y re-construye el heap.
			Vehiculo *first(); 			//Devuelve el primer elemento del heap sin modificarlo.
			
			void print(); 				//Imprime el heap.
			void print_FULL();
			string print_FULL_file();
						                                        
            bool heap_lleno(void) 
            {return Vehiculos.size()==capacidad;};      //devuelve true si el heap está lleno. false si tiene al menos un lugar.
            
            bool heap_vacio(void) 
            {return Vehiculos.empty();};				//devuelve true si el heap está vacío.

			int get_prioridad(int i);					//Retorna la prioridad del elemento i del heap.
			
			int get_cantVehiculos() 
			{return Vehiculos.size();};   				//Retorna la cantidad de elementos que tiene el heap.
			
			int get_capacidad() 
			{return capacidad;};   						//Devuelve la capacidad seteada del heap.
};

HeapMin::HeapMin(int capacidad){
	this->capacidad=capacidad;
	ptrAmb=ptrPat=-1;
}

HeapMin::HeapMin(int capacidad, Vehiculo *v){
	this->capacidad=capacidad;
	ptrAmb=ptrPat=-1;
	insertar(v);	
}

Vehiculo *HeapMin::first(){
	if(!heap_vacio()) return Vehiculos[0]; 
	else return NULL;
}

bool HeapMin::insertar(Vehiculo *nuevo){

	if(heap_lleno()){
		cout<<"No se pudo insertar el Vehiculo matricula nro. "<<nuevo->get_matricula()<<". El semaforo estaba colmado. "<<endl;
		return false;
	}
	else{
		switch(nuevo->get_prioridad()){
			case 1:
				if(ptrAmb==-1){//Primer Ambulancia
					Vehiculos.insert(Vehiculos.begin(),nuevo);  //Inserta nuevo al principio y desplazo todo lo que el vector tenga un lugar hacia la derecha
					ptrAmb++;
				}
				else{//Ya había al menos una Ambulancia
				ptrAmb++;
				Vehiculos.insert(Vehiculos.begin()+ptrAmb,nuevo);	//Inserta nuevo después de la ultima ambulancia y desplaza todo lo que el vector tenga
				}
				if(ptrPat!=-1)ptrPat++; 	//Si hay algún Patrullero, incremento en uno su puntero
				break;
			case 2:
//				cout<<"Vehiculos.size() :"<<Vehiculos.size()<<endl;
//				cout<<"nuevo->get_prioridad()"<<nuevo->get_prioridad()<<endl;
				if(ptrPat==-1){//Primer Patrullero
					ptrPat=ptrAmb+1;
					Vehiculos.insert(Vehiculos.begin()+ptrPat,nuevo);
						
				}
				else{//Ya había al menos un Patrullero
					ptrPat++;											//Pre-incremento el puntero al ultimo patrullero
					Vehiculos.insert(Vehiculos.begin()+ptrPat,nuevo);   //Inserto el patrullero en esa posición.					
				}				
				break;
			case 3:
				Vehiculos.push_back(nuevo);								//Agregó el auto al final del vector. No necesito llevar el puntero.
				break;								
			default:
				cout<<endl<<"ATENCION: Numero de Prioridad incorrecta"<<endl;	
		}
		return true;	
	}	
}

Vehiculo *HeapMin::extraer(){
	if(heap_vacio()){
		cout<<"No se puede extraer Vehiculos de un heap vacio"<<endl;
		return NULL;
	}
	else{
		Vehiculo *cima=Vehiculos.front();			//almaceno el elemento a devolver, para hacerlo al finalizar el metodo
		vector<Vehiculo*> aux=Vehiculos;			//NO se puede hacer la operacion assign sobre el mismo vector
		Vehiculos.assign(aux.begin()+1,aux.end()); 	//Re-asigno los elementos al vector, excepto el primero
		if(ptrAmb!=-1)ptrAmb--; 					//Si hay alguna Ambulancia, decremento en uno su puntero
		if(ptrPat!=-1)ptrPat--; 					//Si hay algún Patrullero, decremento en uno su puntero
		return cima;								//Devuelvo el vehículo extraído
	}
}

int HeapMin::get_prioridad(int i)
{	
		if(i>=0 && i<capacidad && i<get_cantVehiculos())
		return Vehiculos.at(i)->get_prioridad();
		else return -1;
}	


void HeapMin::print(){
	int pos=0;
	
	if(heap_vacio()) cout<<"El Heap esta vacio"<<endl;
	else{
		cout << endl
		<< setw(15) << setiosflags(ios::left)<< "Tipo " 
		<<" || "
		<< setw(11) << setiosflags(ios::left)<< "Matricula " 
		<<" || "
		<< setw(11) << setiosflags(ios::left)<< "Prioridad ";		
		for(int i=0;i<Vehiculos.size();i++)	Vehiculos.at(i)->print_DatosVehiculo2();
		} 
}

void HeapMin::print_FULL(){
	int pos=0;
	
	if(heap_vacio()) cout<<"El Heap esta vacio"<<endl;
	else{	
		for(int i=0;i<Vehiculos.size();i++)	Vehiculos.at(i)->print_DatosVehiculo3();
		} 
}

string HeapMin::print_FULL_file(){
	std::ostringstream stm;
	
	if(heap_vacio()) stm<<"El Heap esta vacio"<<endl;
	else{	
		for(int i=0;i<Vehiculos.size();i++)	stm<<Vehiculos.at(i)->print_DatosVehiculo_file();
		} 
	return stm.str();
}
