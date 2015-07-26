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
            ColaVertices() {czo=NULL;};                  //constructor para la creacion de la ColaVertices
            ~ColaVertices(void);                         //destructor de la ColaVertices
            Nodo *get_comienzo() {return czo;};
            int cabeza(void);  						//Retorna el primer elemento de la cola.
            void agregar(int d);                         //agrega un elemento a la ColaVertices
            void agregar_final(int d);                   //agrega un dato al final de la ColaVertices
            void eliminar(void);                         //elimina el frente de la ColaVertices
            bool buscar(int);                            //devuelve 1 si un vertice esta en la lista y 0 si no
            void print(void);                            //impresion de la ColaVertices en pantalla
            bool esvacia(void) {return czo==NULL;};      //Determina si la Cola esta vacía.
            
};

ColaVertices::~ColaVertices(void)
{
    while(czo != NULL) //elimina el frente de la cola mientras que czo no apunte a NULL
    this->eliminar();
}

void ColaVertices::eliminar()
{
    Nodo *aux;
    aux = czo;
    czo = czo->get_next();
    delete aux;
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
