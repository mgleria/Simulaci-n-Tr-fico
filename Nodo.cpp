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
