/*
 * Esta clase brinda la estructura primaria para la clase ColaVertices.
 * Cada Nodo representa un vértice del grafo que representa el plano de
 * la ciudad. Los datos que lo componen son el nro. de vértice (dato) y
 * el peso relativo del mismo (peso). El campo next se utiliza para 
 * enlazar los nodos y lograr una estructura mas compleja, una cola en  
 * este caso. 
 * Los métodos de esta clase son los básicos para manipular los miembros
 * de datos recien descriptos.
 * La clase Nodo es usada por la clase ColaVertices.
 * */
class Nodo{
    private:
            int dato;
            int peso;
            Nodo *next;
    public:
		   Nodo() {next=NULL;};   //este constructor se utiliza al añadir elementos
		   Nodo(int a) {dato=a; next=NULL;};   //este constructor se utiliza al añadir elementos
		   Nodo(int a, int b) {dato=a; peso=b; next=NULL;};   //este constructor se utiliza al añadir elementos
		   void set_dato(int a) {dato=a;};            //setea la variable dato del Nodo en cuestion
		   void set_next(Nodo *n) {next=n;};   //recibe la direccion de un Nodo y la guarda en el next de nuevo
		   int get_dato() {return dato;};             //devuelve el campo dato del Nodo desde que se llamo a este metodo
		   int get_peso() {return peso;};
		   Nodo *get_next() {return next;};    //devuelve la direccion de memoria a la que apunta el puntero a Nodo next
		   void print();
};

void Nodo::print()
{
	cout<<"ID del nodo: "<<dato<<". Peso de la arista hacia origen :"<<peso<<"."<<endl;
}


/*
* Esta clase es una cola de Nodo(s) que representan los nodos del grafo 
* de la ciudad que se modeliza. Los nodos son los vértices del grafo, 
* que a su vez son las ESQUINAS del plano de la ciudad.
* Como toda lista enlazada, como miembro de datos solo tiene un puntero
* señalando al comienzo. Los métodos sirven para el manejo básico de la
* ED.
* La clase ColaVertices es usada por la clase Grafo.
*/
class ColaVertices{ 
    protected:
            Nodo *czo;
    public:
            ColaVertices() {czo=NULL;};                  	//constructor para la creacion de la ColaVertices
            ~ColaVertices(void);                         	//destructor de la ColaVertices
            Nodo *get_comienzo() {return czo;};
            int cabeza(void);  								//Retorna el primer elemento de la cola.
            void agregar(int d);                         	//agrega un elemento a la ColaVertices
            void agregar_final(int d);                   	//agrega un dato al final de la ColaVertices
            bool eliminar(void);                         	//elimina el frente de la ColaVertices. Retorna true si tuvo exito.
            bool buscar(int);                            	//devuelve 1 si un vertice esta en la lista y 0 si no
            void print(void);                            	//impresion de la ColaVertices en pantalla
            string print_file(void);						//Impresion de la ColaVertices a un objeto stm que se retorna al final
            bool esvacia(void) {return czo==NULL;};      	//Determina si la Cola esta vacía.
            
};

ColaVertices::~ColaVertices(void)
{
    while(czo != NULL) //elimina el frente de la cola mientras que czo no apunte a NULL
    this->eliminar();
}

bool ColaVertices::eliminar()
{
    Nodo *aux;
    aux = czo;
    if(!esvacia()){ 
		czo = czo->get_next();
		delete aux;
		return true;
	}
    else{
		cout<<"ColaVertices vacia"<<endl;
		return false;
	}
    
}

int ColaVertices::cabeza()
{
	if(esvacia())return -1;
	else return czo->get_dato();

}

void ColaVertices::agregar(int d)
{  
    if(czo == NULL)
    {
        Nodo *nuevo=new Nodo(d);
        nuevo->set_next(czo);
        czo = nuevo;
    }
    else this->agregar_final(d);
}

void ColaVertices::agregar_final(int d)
{
    Nodo *nuevo=new Nodo(d);
    Nodo *aux;
    aux=czo;
    
    while((aux->get_next()) != NULL) //este while hace avanzar de a un Nodo a aux hasta que next apunte a NULL
    aux = aux->get_next();
    
    aux->set_next(nuevo);
}

bool ColaVertices::buscar(int n)
{
    Nodo *aux;
    aux = czo;
    bool bandera = false;
    while((aux != NULL) && (bandera == false))
    {
        if((aux->get_dato()) == n) bandera = true;
        aux = aux->get_next();
    }
    return bandera;
}

void ColaVertices::print(void)
{ 
    Nodo *aux;
    aux = czo;
    cout << endl;
    while(aux != NULL)
    {
        cout << " --> " << aux->get_dato();
        aux = aux->get_next();
    }
    cout << endl;
}

string ColaVertices::print_file(void)
{
	std::ostringstream stm; 
    Nodo *aux;
    aux = czo;
    stm << endl;
    while(aux != NULL)
    {
        stm << " --> " << aux->get_dato();
        aux = aux->get_next();
    }
    stm << endl;
    
    return stm.str();
}
