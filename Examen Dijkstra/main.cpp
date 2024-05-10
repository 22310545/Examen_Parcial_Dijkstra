#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <set>

using namespace std;

// Definiciones adelantadas de las clases Nodo y Arco
class Nodo;
class Arco;

class ListaAdyacencia {
public:
    Arco* primero;
    Arco* ultimo;

    ListaAdyacencia() : primero(nullptr), ultimo(nullptr) {}

    bool esVacio();
    bool esAdyacente(Nodo* nodo);
    void insertar(Arco* nodo, Nodo* destino);
    string mostrar(Nodo* first);
    vector<Arco*> getArcos();
    void nuevaAdyacencia(Nodo* destino, int costo);
};

class Nodo {
public:
    string dato;
    ListaAdyacencia lista;
    Nodo* siguiente;
    int costo;
    list<Nodo*> caminoCorto;

    Nodo(string dato) : dato(dato), costo(numeric_limits<int>::max()), siguiente(nullptr) {}
};

class Arco {
public:
    Nodo* destino;
    int costo;
    Arco* siguiente;

    Arco(Nodo* destino, int costo) : destino(destino), costo(costo), siguiente(nullptr) {}
};

class Grafo {
public:
    Nodo* primero;
    Nodo* ultimo;

    Grafo() : primero(nullptr), ultimo(nullptr) {}

    bool esVacio();
    bool existeVertice(Nodo* nodo);
    void nuevoArco(Nodo* origen, Nodo* destino, int costo);
    void nuevoNodo(Nodo* nodo);
    string mostrar();
    Grafo* dijkstra(Grafo* grafo, Nodo* source);
    Nodo* getLowestDistanceNode(set<Nodo*>& noVisitados);
    void calculateMinimumDistance(Nodo* destino, int costoLink, Nodo* source);
};

bool ListaAdyacencia::esVacio() {
    return primero == nullptr;
}

bool ListaAdyacencia::esAdyacente(Nodo* nodo) {
    Arco* actual = primero;
    bool encontrado = false;
    while (actual != nullptr && !encontrado) {
        if (nodo->dato == actual->destino->dato) {
            encontrado = true;
        }
        actual = actual->siguiente;
    }
    return encontrado;
}

void ListaAdyacencia::insertar(Arco* nodo, Nodo* destino) {
    if (esVacio()) {
        primero = nodo;
        ultimo = nodo;
    } else {
        if (destino->dato <= primero->destino->dato) {
            nodo->siguiente = primero;
            primero = nodo;
        } else {
            if (destino->dato >= ultimo->destino->dato) {
                ultimo->siguiente = nodo;
                ultimo = nodo;
            } else {
                Arco* posicion = primero;
                while (destino->dato != posicion->destino->dato) {
                    posicion = posicion->siguiente;
                }
                nodo->siguiente = posicion->siguiente;
                posicion->siguiente = nodo;
            }
        }
    }
}

string ListaAdyacencia::mostrar(Nodo* first) {
    string valores = "";
    Arco* temporal = primero;
    Nodo* nodo = first;
    while (temporal != nullptr) {
        while (nodo != nullptr) {
            if (esAdyacente(nodo)) {
                valores += "1 ";
            } else {
                valores += "0 ";
            }
            nodo = nodo->siguiente;
        }
        temporal = temporal->siguiente;
    }
    return valores;
}

vector<Arco*> ListaAdyacencia::getArcos() {
    vector<Arco*> arcos;
    Arco* temporal = primero;
    while (temporal != nullptr) {
        arcos.push_back(temporal);
        temporal = temporal->siguiente;
    }
    return arcos;
}

void ListaAdyacencia::nuevaAdyacencia(Nodo* destino, int costo) {
    if (!esAdyacente(destino)) {
        Arco* nodo = new Arco(destino, costo);
        insertar(nodo, destino);
    }
}

bool Grafo::esVacio() {
    return primero == nullptr;
}

bool Grafo::existeVertice(Nodo* nodo) {
    bool existe = false;
    if (!esVacio()) {
        Nodo* temporal = primero;
        while (temporal != nullptr && !existe) {
            if (temporal->dato == nodo->dato) {
                existe = true;
            }
            temporal = temporal->siguiente;
        }
    }
    return existe;
}

void Grafo::nuevoArco(Nodo* origen, Nodo* destino, int costo) {
    if (existeVertice(origen) && existeVertice(destino)) {
        Arco* nodo = new Arco(destino, costo);
        origen->lista.nuevaAdyacencia(destino, costo);
        destino->lista.nuevaAdyacencia(origen, costo);

        cout << "Origen: " << origen->dato << " Costo: " << costo << " Destino: " << destino->dato << endl;
    }
}

void Grafo::nuevoNodo(Nodo* nodo) {
    if (!existeVertice(nodo)) {
        if (esVacio()) {
            primero = nodo;
            ultimo = nodo;
        } else {
            if (nodo->dato.compare(primero->dato) <= 0) {
                nodo->siguiente = primero;
                primero = nodo;
            } else {
                if (nodo->dato.compare(ultimo->dato) >= 0) {
                    ultimo->siguiente = nodo;
                    ultimo = nodo;
                } else {
                    Nodo* temp = primero;
                    while (nodo->dato.compare(temp->dato) != 0) {
                        temp = temp->siguiente;
                    }
                    nodo->siguiente = temp->siguiente;
                    temp->siguiente = nodo;
                }
            }
        }
    }
}

string Grafo::mostrar() {
    string valores = "   ";
    Nodo* temporal = primero;
    while (temporal != nullptr) {
        valores += temporal->dato + " ";
        temporal = temporal->siguiente;
    }
    valores += "\n";
    temporal = primero;
    while (temporal != nullptr) {
        valores += temporal->dato + "->" + temporal->lista.mostrar(primero) + "\n";
        temporal = temporal->siguiente;
    }
    return valores;
}

Grafo* Grafo::dijkstra(Grafo* grafo, Nodo* source) {
    source->costo = 0;
    set<Nodo*> visitados;
    set<Nodo*> noVisitados;

    noVisitados.insert(source);
    while (!noVisitados.empty()) {
        Nodo* actual = getLowestDistanceNode(noVisitados);
        noVisitados.erase(actual);
        vector<Arco*> links = actual->lista.getArcos();
        for (Arco* link : links) {
            Nodo* adjacentNode = link->destino;
            int costoLink = link->costo;
            if (!visitados.count(adjacentNode)) {
                calculateMinimumDistance(adjacentNode, costoLink, actual);
                noVisitados.insert(adjacentNode);
            }
        }
        visitados.insert(actual);
    }
    Nodo* temp = primero;
    while (temp != nullptr) {
        string route = "";
        cout << "Nodo: " << temp->dato << " costo: " << temp->costo << endl;
        for (Nodo* node : temp->caminoCorto) {
            route += node->dato + " -> ";
        }
        cout << "Ruta mas corta: " << route << endl;
        temp = temp->siguiente;
    }
    return grafo;
}

Nodo* Grafo::getLowestDistanceNode(set<Nodo*>& noVisitados) {
    Nodo* lowestDistanceNode = nullptr;
    int lowestDistance = numeric_limits<int>::max();

    for (Nodo* node : noVisitados) {
        int distancia = node->costo;
        if (distancia < lowestDistance) {
            lowestDistance = distancia;
            lowestDistanceNode = node;
        }
    }
    return lowestDistanceNode;
}

void Grafo::calculateMinimumDistance(Nodo* destino, int costoLink, Nodo* source) {
    int costoSource = source->costo;
    if (costoSource + costoLink < destino->costo) {
        destino->costo = costoSource + costoLink;
        destino->caminoCorto = source->caminoCorto;
        destino->caminoCorto.push_back(source);
    }
}

int main() {
    Grafo grafo;

    Nodo* nodo1 = new Nodo("A");
    Nodo* nodo2 = new Nodo("B");
    Nodo* nodo3 = new Nodo("C");
    Nodo* nodo4 = new Nodo("D");
    Nodo* nodo5 = new Nodo("E");

    grafo.nuevoNodo(nodo1);
    grafo.nuevoNodo(nodo2);
    grafo.nuevoNodo(nodo3);
    grafo.nuevoNodo(nodo4);
    grafo.nuevoNodo(nodo5);

    grafo.nuevoArco(nodo5, nodo3, 10);
    grafo.nuevoArco(nodo4, nodo2, 20);
    grafo.nuevoArco(nodo1, nodo4, 1);
    grafo.nuevoArco(nodo3, nodo1, 0);
    grafo.nuevoArco(nodo2, nodo1, 12);
    grafo.nuevoArco(nodo2, nodo5, 5);

    cout << grafo.mostrar() << endl;

    grafo.dijkstra(&grafo, nodo1);

    return 0;
}