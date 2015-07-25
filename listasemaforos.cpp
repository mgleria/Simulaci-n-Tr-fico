/*
Listasemaforos es una lista que se ordena en función de la congestión de los semáforos.
El semáforo mas congestionado se ubica primero en la lista.
*/

#include "Semaforo.cpp"		

class Nodosemaforo{
    private:
            Semaforo* dato;
            Nodosemaforo *next;
    public:
           Nodosemaforo(Semaforo* a) {dato=a; next=NULL;};  //este constructor se utiliza al añadir elementos
           void set_dato(Semaforo* a) {dato=a;};            //setea la variable dato del Nodosemaforo en cuestion
           void set_next(Nodosemaforo *n) {next=n;};        //recibe la direccion a la que apunta czo y la guarda en el next de nuevo
           Semaforo* get_dato() {return dato;};             //devuelve el campo dato del Nodosemaforo desde que se llamo a este metodo
           Nodosemaforo* get_next() {return next; };        //devuelve la direccion de memoria a la que apunta el puntero a Nodosemaforo next
};

class Listasemaforos{ 
    protected:
            Nodosemaforo *czo;
    public:
            Listasemaforos() {czo=NULL;};                          //constructor para la creacion de la Listasemaforos
            ~Listasemaforos(void);                                 //destructor de la Listasemaforos
            void insertar(Semaforo* d);                             //agrega un elemento a la Listasemaforos
            void eliminar(void);                                   //elimina la cabeza de la Listasemaforos
            bool esvacia(void);										//devuelve 1 si es vacia y cero si no lo es
            Nodosemaforo *get_czo(){return czo;};                                    
            
            Semaforo* cabeza(void);                                //devuelve la cabeza de la Listasemaforos
            Semaforo* buscar(int, int);                            //devuelve el puntero al semaforo buscado a partir de ubicacion y precedencia
            Semaforo* busca_SemaforoNumero1(int);                  //recibe un numero aleatorio de la posicion en la lista a buscar
            Semaforo* busca_SemaforoNumero2(Nodosemaforo*, int);   //metodo recursivo que llega hasta un Nodosemaforo y devuelve su semaforo
            void ordenar();                                        //se utiliza para ordenar la lista de semaforos mediante una implementacion de insert-sort...
            
            void print_TODO();
            void print();										   //Imprime todos los semaforos
            void print(string);                                    //recibe el tipo de impresion que se desea realizar y llama a su metodo
            void print_TOP10(Nodosemaforo*, int);                  //imprime en pantalla los diez primeros semaforos de la lista
            void print_ArchivoSemaforos(void);                     //imprime en un archivo la lista de semaforos completa
            
            int get_congestion(int i);    						   //Devuelve la congestion del semaforo i de la lista. Si este no existe, devuelve -1.
			void ubicarAleatorio(Vehiculo*); 
};

Listasemaforos::~Listasemaforos(void)
{
    while(czo != NULL) //elimina la cabeza de la lista mientras que czo no apunte a NULL
    this->eliminar();
}

void Listasemaforos::insertar(Semaforo* d)
{  
    Nodosemaforo* nuevo=new Nodosemaforo(d);
    nuevo->set_next(czo);
    czo = nuevo;
}

void Listasemaforos::eliminar()
{
    Nodosemaforo *aux;
    aux = czo;
    czo = czo->get_next();
    delete aux;
}

bool Listasemaforos::esvacia(void)
{   
    return czo==NULL;
    srand(time(NULL));
}

Semaforo* Listasemaforos::cabeza(void)
{
    return czo->get_dato();
}

Semaforo* Listasemaforos::buscar(int u, int a)
{
    Nodosemaforo *nodo = czo;
    Semaforo *aux = NULL;
    while(nodo != NULL)
    {
        if(((nodo->get_dato())->get_ubicacion() == u) && ((nodo->get_dato())->get_procedencia() == a))
        aux = nodo->get_dato();
        nodo = nodo->get_next();
    }
    return aux;
}

Semaforo* Listasemaforos::busca_SemaforoNumero1(int x)
{
    if(x>=0 && x<cantSemaforos)
    return busca_SemaforoNumero2(czo, x);
    else return NULL;
}

Semaforo* Listasemaforos::busca_SemaforoNumero2(Nodosemaforo* c, int x)
{
    Semaforo* aux;
    if(x > 0) aux = busca_SemaforoNumero2(c->get_next(), x-1);
    else if(x == 0) aux = c->get_dato();
    return aux;
}

void Listasemaforos::ordenar()
{
    Semaforo* aux[cantSemaforos];
    Semaforo* temp;
    int i, j;
    for(i=0 ; i<cantSemaforos ; i++)
    {
        aux[i] = czo->get_dato();
        this->eliminar();
    }
    for(i=1 ; i<cantSemaforos ; i++) //IMPLEMENTACION DE UN INSERT-SORT
    {
        j=i-1;
        while(j>=0)
        {
            if(((aux[j+1])->congestion()) < ((aux[j])->congestion()))
            {
                temp = aux[j+1];
                aux[j+1]=aux[j];
                aux[j]=temp;
            }
            j--;
        }
    }
    for(i=0 ; i<cantSemaforos ; i++)
    this->insertar(aux[i]);
}

int Listasemaforos::get_congestion(int i)
{
	Semaforo *aux;
	
	aux=busca_SemaforoNumero1(i);
	
	if(aux!=NULL) return aux->congestion();
	else return -1;
	
}

void Listasemaforos::ubicarAleatorio(Vehiculo *v)
{ 
	
	int rnd = rand()%cantSemaforos;
	Semaforo *aux;
	aux=busca_SemaforoNumero1(rnd);
	
	while(aux->get_ubicacion() == v->get_origen()){
		rnd= rand()%cantSemaforos;
		aux=busca_SemaforoNumero1(rnd);
	}
	
	if(aux != NULL) {
		if(aux->rojo(v)) { //Si 'v' se pudo encolar en la cola de 'aux':
			v->set_origen(aux->get_ubicacion()); //Seteo origen y pos_actual de 'v' relativos al semaforo 'aux'
			v->set_posicionActual(aux->get_ubicacion());
		}
		else cout<<"El semaforo estaba colmado. No se pudo insertar el vehiculo mat.: "<<v->get_matricula()<<endl;			
	}
	else cout<<"Error en la busqueda del semaforo (UbicarAleatorio) "<<endl;
}

void Listasemaforos::print()
{ 
    Nodosemaforo *nodo = czo;
    int cont=0;
    while(nodo != NULL)
    {	
		cont++;
		cout<<cont<<"  ";
        nodo->get_dato()->print();
        nodo = nodo->get_next();
    }
}

void Listasemaforos::print_TODO()
{ 
    Nodosemaforo *nodo = czo;
    int cont=0;
    while(nodo != NULL)
    {	
		cont++;
		cout<<setw(100) << setiosflags(ios::right) <<"Semaforo numero: "<<cont<<".   ";
		nodo->get_dato()->print();
        nodo->get_dato()->print_AUTOS();
        nodo = nodo->get_next();
    }
}

void Listasemaforos::print(string t)
{ 
    if(t == "TOP10") print_TOP10(czo, 10);
    else if(t == "archivo") this->print_ArchivoSemaforos();
    else if(t=="TODO") print_TOP10(czo, cantSemaforos);
}

void Listasemaforos::print_TOP10(Nodosemaforo* c, int x)
{
    (c->get_dato())->print();
    if(x > 1) print_TOP10(c->get_next(), x-1);
		
}

void Listasemaforos::print_ArchivoSemaforos()
{ 
    ofstream salidaSemaforos;
    salidaSemaforos.open("RankingSemaforos.txt");
    Nodosemaforo *aux;
    aux = czo;
    salidaSemaforos << setw(11) << setiosflags(ios::left) << "|| Esquina " 
					<< setw(11) << setiosflags(ios::left) << "|| Presedencia " 
					<< setw(11) << setiosflags(ios::left) << "|| Carriles " 
					<< setw(11) << setiosflags(ios::left) << "|| Saturacion ";
    salidaSemaforos << endl << "-----------------------------------------------------------";
    while(aux != NULL)
    {
        salidaSemaforos << " || " << setw(7) << setiosflags(ios::left) << (aux->get_dato())->get_ubicacion();
        salidaSemaforos << " || " << setw(11) << setiosflags(ios::left) << (aux->get_dato())->get_procedencia();
        salidaSemaforos << " || " << setw(8) << setiosflags(ios::left) << (aux->get_dato())->get_carriles();
        salidaSemaforos << " || " << setw(3) << setiosflags(ios::left) << (aux->get_dato())->congestion();
        
        aux = aux->get_next();
    }
    salidaSemaforos.close();
}
