/*
 * COMENTAR ESTA CLASE
 * 
 * */

class Semaforo{
	private:
			int ubicacion;
			int procedencia;
			int carriles;
			bool bloqueado;
			HeapMin *colaVehiculos;

	public:
			Semaforo(int,int,int);
			
			int get_ubicacion(){return ubicacion;};
			int get_procedencia(){return procedencia;};
			int get_carriles(){return carriles;};
			bool get_estado() {return bloqueado;};
			void bloquear() {bloqueado = true;};
			void desbloquear() {bloqueado = false;};
			bool is_full(){ return colaVehiculos->heap_lleno(); };
			
			HeapMin *get_ColaDelSemaforo(){ return colaVehiculos;};
			
			int congestion();
			int get_cantidadDeVehiculos();
			void print();
			void print_AUTOS();
			
			bool rojo(Vehiculo*);		//Este m�todo es el que recibe los autos para encolarlos, si es que la 
										//capacidad no est� colmada.
					
};

//Constructor del semaforo. Verifica que el numero de carriles se encuentre entre 
//1 y 3. Setea la capacidad de la cola en funci�n de los carrilles y la variable global n. 
Semaforo::Semaforo(int c, int IDnodo, int proc){
			
		ubicacion=IDnodo;
		procedencia=proc;
		carriles=c;
		bloqueado = false;
		
		int capacidad_cola = 3*N*c;
		colaVehiculos=new HeapMin(capacidad_cola);
}

//Si la capacidad lo permite, encola el veh�culo procedente de alg�n sem�foro previo y devuelve true. 
//En caso contrario devuelve false.
bool Semaforo::rojo(Vehiculo *v)
{	if(this != NULL) return colaVehiculos->insertar(v);  
	else return false;
}

//Devuelve un entero que representa el porcentaje sobre 100, de ocupaci�n que presenta la cola del sem�foro. 
int Semaforo::congestion(){
	return (int)(colaVehiculos->get_cantVehiculos()*100/colaVehiculos->get_capacidad());
}
int Semaforo::get_cantidadDeVehiculos(){
	if(!colaVehiculos->heap_vacio()) return colaVehiculos->get_cantVehiculos();
	else return 0;
}

//Imprime la informaci�n concerniente al sem�foro.
void Semaforo::print(){
	
	if(this != NULL){
		cout<<endl;
		cout<<"Ubicado en Nodo Nro. :"<<setw(3)<<ubicacion; 
		cout<<" Procedencia :"<<setw(3)<<procedencia;
		cout<<" Nro Carriles: "<<carriles;
		cout<<" Congestion del Semaforo: "<<setw(3)<<congestion()<<"% ";
		cout<<"Cant. Vehiculos: "<<setw(3)<<get_cantidadDeVehiculos()
		<<endl;
	}
	else cout<<endl<<"El semaforo NO es valido"<<endl;
}

//Imprime la informaci�n de todos los autos del semaforo
void Semaforo::print_AUTOS(){
	
	if(!colaVehiculos->heap_vacio()){
		colaVehiculos->print_FULL();
	}
	else cout<<endl<<"El semaforo NO tiene Vehiculos"<<endl;
}

