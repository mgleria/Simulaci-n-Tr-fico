//Revisar prioridades
class Vehiculo{
    private:
            string tipo;
            int matricula;
            int prioridad;
            int origen;
            int destino;
            int posicion_actual;
            ColaVertices* recorrido;				//Recorrido parcial del vehículo.								
            ColaVertices* camino;					//Camino a seguir por el Vehiculo en la simulación. 
            
            bool set_prioridad(string tipo); 						//Retorna true si pudo setear correctamente la prioridad al vehiculo.
    public:
            Vehiculo();												//Constructor de Vehiculos aleeatorios
            Vehiculo(string, int , int , int);              		//constructor para la creacion de 'miAuto' Vehiculo
            Vehiculo(string);                             			//constructor aleatorio para 'tipo' específico
            ~Vehiculo(void){};                                      //destructor del vehiculo

            void print_DatosVehiculo(void);                         //imprime en pantalla todos los datos de un vehiculo
            void print_DatosVehiculo2(void); 
            void print_DatosVehiculo3(void);
			void print_encabezado(); 
			
            void print_recorrido(void);                             //imprime en pantalla el camino ya recorrido por el vehiculo
            void print_camino(void);                                //imprime en pantalla el camino que debe recorrer el vehiculo
            void set_camino(ColaVertices*);                         //setea la lista de vertices a seguir para completar el recorrido
            void put_recorrido(int p) {recorrido->agregar(p);};     //agrega una esquina al recorrido ya hecho por el vehiculo
            int get_siguiente(void) {return camino->cabeza();};     //devuelve la siguiente posicion a la que se debe dirigir el vehiculo
            void borrar_cabeza(void) {camino->eliminar();};      	//borra del camino el frente de la cola, dejando el 2do elemento como posicion a seguir por el vehiculo
            
            void rutaRandom();										//Setea aleatoriamente Origen y Destino (dentro del rango adecuado). Tambien Posicion_actual
            string get_tipo(void) {return tipo;};                   //
            int get_matricula(void) {return matricula;};            //
            int get_origen(void) {return origen;};                  // METODOS QUE RETORNAN EL VALOR O CONTENIDO DE UN CAMPO
            int get_posicionActual(void) {return posicion_actual;}; //
            int get_destino(void) {return destino;};
			int get_prioridad(void) {return prioridad;};
			ColaVertices* get_camino() {return camino;};
			ColaVertices* get_recorrido() {return recorrido;};                  //
            void set_posicionActual(int p) {posicion_actual = p;};
            void set_origen(int o) {origen = o;};  //
};

Vehiculo::Vehiculo(string t, int mat, int o, int d)
{
    prioridad=0;
    if(set_prioridad(t)){
    	tipo=t;
	    matricula = mat;
	    if((o>=0&&o<cantNODOS)&&(d>=0&&d<cantNODOS)&&(o!=d)){
	    	origen = o;
	        destino = d;
			posicion_actual = o;	
		}
		else{
			rutaRandom();
			cout<<"Origen y/o Destino incorrecto/s. "<<"Para el vehiculo Mat. Nro. :"<<matricula<<", se seteo el siguiente Origen-Destino:"<<endl;
			cout<<"Origen: "<<origen<<"  Destino: "<<destino<<endl;
		}
	    recorrido = new ColaVertices();
	   	camino = new ColaVertices();	
	}
	else{
		cout<<"Tipo de Vehiculo incorrecto"<<endl;
	}
}

Vehiculo::Vehiculo()
{
    int r=rand()%3+1;
	switch (r){
		case 1:
			tipo="Auto";
			break;
		case 2:
			tipo="Ambulancia";
			break;
		case 3:
			tipo="Patrullero";
			break;
		default:
			tipo="Auto";
	}

    prioridad = 0;
    if(set_prioridad(tipo)){
    matricula = rand()%1000;
	
	destino=rand()%cantNODOS;
    posicion_actual = origen = -1;
    
    recorrido = new ColaVertices();
    camino = new ColaVertices();
	}
	else{
		cout<<"Algun problema creando un vehiculo por defecto"<<endl;
	}

}

Vehiculo::Vehiculo(string t)
{
	 
    if(!set_prioridad(t)){
    	cout<<endl<<"TIPO DE VEHÍCULO INCORRECTO"<<endl;
    	cout<<"Vehículo NO iniciado"<<endl;
	}
	else{
		tipo = t;
    	matricula = (rand()%1000 + 1000); 	//Matricula aleatoria entre 1000 y 1999.
		//rutaRandom(); //Setea Origen y Destino aleatoriamente
    	destino=rand()%cantNODOS;
    	posicion_actual = origen = -1; //Cuando ubico el vehiculo seteo estos campos
		//prioridad ya iniciada.
		recorrido = new ColaVertices();
   		camino = new ColaVertices();
	}

}

bool Vehiculo::set_prioridad(string t)
{
    //REVISAR ->tipo ENUM?? Codigo desprolijo
	if(t=="Auto"){ prioridad=3; return true;}
    if(t=="Patrullero") {prioridad=2; return true;}
    if(t=="Ambulancia") {prioridad=1; return true;}
    if(prioridad==0) return false;
}

void Vehiculo::set_camino(ColaVertices* aux)
{
	//~ camino->~ColaVertices();
	//~ camino = aux;
	
	Nodo *nodo = aux->get_comienzo();	
    while(nodo != NULL){
		camino->agregar(nodo->get_dato());
		nodo = nodo->get_next();
	}
}

void Vehiculo::rutaRandom()
{	
//	srand(time(NULL));
	origen = rand()%cantNODOS;
	posicion_actual=origen;			
	destino = rand()%cantNODOS;
	if(origen==destino){
//	 	srand(time(NULL));
	 	destino = rand()%cantNODOS;
	 }
}

void Vehiculo::print_DatosVehiculo()
{
	if(this!=NULL){
		cout << endl << setw(11) << setiosflags(ios::left)<< "Tipo " <<
						setw(16) << setiosflags(ios::left)<< "|| Matricula" << 
						setw(12) << setiosflags(ios::left)<< "|| Origen" << 
						setw(12) << setiosflags(ios::left)<< "|| PosActual" <<
						setw(12) << setiosflags(ios::left)<< "|| Destino" << 
						setw(12) << setiosflags(ios::left)<< "|| Estado";
						
		cout << endl << setw(11) << setiosflags(ios::left) << tipo;
		cout << "||" << setw(14) << setiosflags(ios::left) << matricula;
		cout << "||" << setw(10) << setiosflags(ios::left) << origen;
		cout << "||" << setw(10) << setiosflags(ios::left) << posicion_actual;
		cout << "||" << setw(10) << setiosflags(ios::left) << destino;
		
		if(posicion_actual == destino)
		cout << "|| " << setw(10) << setiosflags(ios::left) << "En destino..!";
		else
		cout << "|| " << setw(10) << setiosflags(ios::left) << "Circulando...";
		cout << endl;
	}
	else cout<<endl<<"El vehiculo no es valido"<<endl;	
}

void Vehiculo::print_DatosVehiculo2()
{
	if(this!=NULL){
    cout << endl 
    << setw(15) << setiosflags(ios::left) << tipo
    <<"    "
    << setw(11) << setiosflags(ios::left) << matricula
    <<"    "
    << setw(11) << prioridad;
	}
	else cout<<endl<<"El vehiculo no es valido"<<endl;
}

void Vehiculo::print_DatosVehiculo3()
{
	if(this!=NULL){
    cout << endl 
    << setw(10) << tipo
    << setw(10) << matricula
    << setw(7) << origen
    << setw(7) << destino
    << setw(7) << posicion_actual<<endl;
    cout<<"El camino por recorrer es: ";
    this->print_camino();
    cout<<"El camino ya realizado es: ";
	this->print_recorrido();
	}
	else cout<<endl<<"El vehiculo no es valido"<<endl;
}

void Vehiculo::print_encabezado()
{
    cout << endl << setw(10) << setiosflags(ios::left) 
	<< "Tipo " << setw(11) << setiosflags(ios::left) 
	<< " || Matricula " << setw(11) << setiosflags(ios::left) 
	<< " || Origen " << setw(11) << setiosflags(ios::left)
	<< " || Posicion Actual " << setw(11) << setiosflags(ios::left) 
	<< " || Destino " << setw(11) << setiosflags(ios::left) 
	<< " || Estado ";

}

void Vehiculo::print_recorrido()
{
    if(this->recorrido->esvacia()) cout << endl << " --> En ORIGEN" << endl;
    else recorrido->print();
}

void Vehiculo::print_camino()
{
    if(this->camino->esvacia()) cout << endl << " --> DESTINO alcanzado"<<endl;
    else camino->print();
}
