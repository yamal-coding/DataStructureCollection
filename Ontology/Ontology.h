#ifndef ONTOLOGY_H
#define ONTOLOGY_H

#include <list>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <exception>

typedef struct {
	bool isPair;
	std::string parent;
	std::string son;
} tEdge;

bool operator==(const tEdge &a, const tEdge &b) {
	return a.parent == b.parent && a.son == b.son && a.isPair == b.isPair;
}

class LoopInDAGException : public std::exception {
private:
	std::string msg;

public:
	LoopInDAGException(const std::string &msg) : msg(msg) {}

	virtual const char* what() const throw() {
		return msg.c_str();
	}
};

class Node {
private:
	std::string name;
	std::list<Node*> sons;
	int numParents;
	bool beingDestructed;

	int getNumParents() {
		return numParents;
	}

	void increaseNumParents() {
		numParents++;
	}

	void decreaseNumParents() {
		if (numParents > 0)
			numParents--;
	}

public:
	Node() {}

	Node(const std::string &name) : name(name), numParents(0), beingDestructed(false) {}

	Node(const std::string &name, const std::list<Node*> &sons) : name(name), sons(sons), numParents(0), beingDestructed(false) {}

	~Node() {
		beingDestructed = true;

		if (!sons.empty()){
			for (std::list<Node*>::iterator list_iter = sons.begin(); list_iter != sons.end(); list_iter++) {
				if (((Node*)(*list_iter))->getNumParents() > 1)
					((Node*)(*list_iter))->decreaseNumParents();
				else if (!((Node*)(*list_iter))->beingDestructed)
					delete ((Node*)(*list_iter));
			}
		}

		std::cout << "Node " << name << " deleted" << std::endl;
	}

	std::string getName() {
		return name;
	}

	std::list<Node*> getSons() {
		return sons;
	}

	void setName(const std::string &newName) {
		name = newName;
	}

	void addSon(Node* &newSon) {
		sons.push_back(newSon);
		newSon->increaseNumParents();
	}

	void print(const int &tabs){
		for (int i = 0; i < tabs; i++)
			std::cout << '\t';
		std::cout << name << std::endl;
		if (!sons.empty()){
			for (std::list<Node*>::iterator list_iter = sons.begin(); list_iter != sons.end(); list_iter++) {
				((Node*)(*list_iter))->print(tabs + 1);
			}
		}
	}

	void descompose(std::list<tEdge> &edges, std::unordered_set<std::string> &visited){
		visited.insert(name);

		if (sons.empty()) {
			tEdge newEdge;
			newEdge.parent = name;
			newEdge.isPair = false;
			edges.push_back(newEdge);
		}
		else {
			for (std::list<Node*>::iterator list_iter = sons.begin(); list_iter != sons.end(); list_iter++) {
				tEdge newEdge;
				newEdge.parent = name;
				newEdge.son = ((Node*)(*list_iter))->getName();
				newEdge.isPair = true;

				edges.push_back(newEdge);

				if (visited.find(((Node*)*list_iter)->getName()) == visited.end())
					if (!((Node*)(*list_iter))->getSons().empty())
						((Node*)(*list_iter))->descompose(edges, visited);
			}
		}
	}

	//recibe el string del nombre del hijo buscado y un conjunto de los nodos visitados para evitar recorridos repetidos
	bool hasSon(const std::string &sonName, std::unordered_set<std::string> &visited) {
		visited.insert(name);

		if (sons.empty())
			return false;

		std::list<Node*>::iterator list_iter = sons.begin();

		bool found = false;

		while (list_iter != sons.end() && !found) {
			if (sonName == ((Node*)*list_iter)->getName())
				found = true;
			else if (visited.find(((Node*)*list_iter)->getName()) == visited.end())
				found = ((Node*)*list_iter)->hasSon(sonName, visited);

			list_iter++;
		}
		
		return found;
	}
};


namespace std {
	template <> struct hash<tEdge> {
		std::size_t operator() (tEdge const& e) const {
			std::size_t h1 = std::hash<std::string>{}(e.parent);
			std::size_t h2 = std::hash<std::string>{}(e.son);

			return h1 ^ (h2 << 1);
		}
	};

	template <> struct hash<Node*> {
		std::size_t operator() (Node* const& n) const {
			std::size_t h = std::hash<std::string>{}(n->getName());
			return h;
		}
	};
}

class Ontology {
private:
	std::list<Node*> rootsList;

	void edgesListToOntology(std::list<tEdge> edges) {
		if (!edges.empty()){
			std::unordered_set<tEdge> checkedPairs; //unoreder_set (HashSet) with the edges that have been already inserted in the new Ontology.
			std::unordered_map<std::string, Node*> nodePointersHashSet; //unordered_map (HashMap) with pointers to every Node of the new Ontology.
			std::unordered_set<std::string> possibleRootNodes; //unordered_set (HashSet) with the possible root nodes while processing the edges


			for (std::list<tEdge>::iterator list_iter = edges.begin(); list_iter != edges.end(); list_iter++) {
				if (checkedPairs.find(*list_iter) == checkedPairs.end()){
					if (list_iter->isPair) { //if it is a complete edge (two nodes)

						std::unordered_map<std::string, Node*>::iterator parent_iter = nodePointersHashSet.find(list_iter->parent);
						std::unordered_map<std::string, Node*>::iterator son_iter = nodePointersHashSet.find(list_iter->son);
						std::unordered_map<std::string, Node*>::iterator end_iter = nodePointersHashSet.end();

						if (parent_iter != end_iter && son_iter != end_iter) { //Los nodos estan en la ontologia pero no estan conectados
							//comprobar si son_iter tiene como hijo en el arbol a parent_iter. si es asi, es un ciclo
							std::unordered_set<std::string> visited;
							if (((Node*)(son_iter->second))->hasSon(((Node*)(parent_iter->second))->getName(), visited)) {
								//diferentes respuestas ante la deteccion del ciclo
								//se puede devolver una ontologia a medias indicando que arista provoca el conflicto
								//o se puede liberar la memoria de los nodos actuales y lanzar una excepcion
								//La mas acertada es abortar la ejecucion pues si se deja seguir habria que decidir que hacer cuando:
								//el nodo que hace el ciclo se convierte en padre de un nodo raiz -> cual es la nueva raiz?

								//liberar el conjunto de nodos temporal
								for (std::unordered_set<std::string>::iterator rootsIter = possibleRootNodes.begin(); rootsIter != possibleRootNodes.end(); rootsIter++) {
									delete nodePointersHashSet.find(*rootsIter)->second;
								}

								throw LoopInDAGException("There is a loop caused by (" + list_iter->parent + ", " + list_iter->son + ") edge");
							}

							//se le aniade como hijo el Node apuntado por son_iter al Node apuntado por parent_iter
							((Node*)(*parent_iter).second)->addSon(((Node*)(*son_iter).second));


							if (possibleRootNodes.find(list_iter->son) != possibleRootNodes.end())
								possibleRootNodes.erase(list_iter->son);
						}
						else if (parent_iter == end_iter && son_iter != end_iter) {

							//se crea un Node padre y se le aniade como hijo el Node al que apunta son_iter
							Node* newParent = new Node(list_iter->parent);
							newParent->addSon(((Node*)(*son_iter).second));

							nodePointersHashSet.insert({ list_iter->parent, newParent });

							//comprobar si el hijo estaba en la estructura de posibles nodos raices y quitarlo de ella y meter al padre si o si
							possibleRootNodes.insert(list_iter->parent);

							if (possibleRootNodes.find(list_iter->son) != possibleRootNodes.end())
								possibleRootNodes.erase(list_iter->son);


						}
						else if (parent_iter != end_iter && son_iter == end_iter) {
							//se crea un Node hijo y se le hace hijo del Node apuntado por parent_iter
							Node* newSon = new Node(list_iter->son);
							((Node*)(*parent_iter).second)->addSon(newSon);

							nodePointersHashSet.insert({ list_iter->son, newSon });
						}
						else { //Los nodos no estan en la ontologia
							//se crean dos nuevos objetos de tipo Node y se aniade uno de ellos como hijo al otro
							Node* newParent = new Node(list_iter->parent);
							Node* newSon = new Node(list_iter->son);
							newParent->addSon(newSon);

							nodePointersHashSet.insert({ list_iter->parent, newParent });
							nodePointersHashSet.insert({ list_iter->son, newSon });

							//poner al padre en una estructura temporal de posibles nodos raiz
							possibleRootNodes.insert(list_iter->parent);
						}
					}
					else { //if it is a single node
						if (nodePointersHashSet.find(list_iter->parent) == nodePointersHashSet.end()) {
							Node* newRoot = new Node(list_iter->parent);
							nodePointersHashSet.insert({ list_iter->parent, newRoot });

							//poner al nuevo nodo en una estructura temporal de posibles nodos raiz
							possibleRootNodes.insert(list_iter->parent);
						}
					}

					checkedPairs.insert(*list_iter);
				}
			}

			//aniadir a la lista de nodos raiz de la ontologia los nodos de la estructura temporal de posibles nodos raiz.
			//Los que quedan en ella si son verdaderas raices. Cojo los strings del set y busco los nodos del hashset y los aniado a la lista de raices de la ontologia
			for (std::string root : possibleRootNodes)
				rootsList.push_back((Node*)(*nodePointersHashSet.find(root)).second);

		}
	}

public:
	Ontology(std::list<Node*> rootsList) : rootsList(rootsList) {}

	//This constructor builds an Ontology using a list of edges (vertex pairs of a graph)
	Ontology(const std::list<tEdge> &edges) {
		edgesListToOntology(edges);
	}

	//This constructor builds an Ontology using two list of edges (vertex pairs of a graph)
	Ontology(const std::list<tEdge> &edges1, const std::list<tEdge> &edges2) {
		std::list<tEdge> edgesAux = edges1;

		for (tEdge e : edges2)
			edgesAux.push_back(e);

		edgesListToOntology(edgesAux);
	}

	~Ontology() {
		if (!rootsList.empty()) {
			for (std::list<Node*>::iterator list_iter = rootsList.begin(); list_iter != rootsList.end(); list_iter++) {
				delete ((Node*)(*list_iter));
			}
		}

		std::cout << "Ontology deleted" << std::endl;
	}

	void print() {
		if (!rootsList.empty()){
			for (std::list<Node*>::iterator list_iter = rootsList.begin(); list_iter != rootsList.end(); list_iter++) {
				((Node*)(*list_iter))->print(0);
			}
		}
	}

	void descompose(std::list<tEdge> &edges){
		if (!rootsList.empty()){
			std::unordered_set<std::string> visited;
			for (std::list<Node*>::iterator list_iter = rootsList.begin(); list_iter != rootsList.end(); list_iter++) {
				((Node*)(*list_iter))->descompose(edges, visited);
			}
		}
	}

	//This function must return a JSON or XML object serialized from an Ontology
	void serialize() {
		//1. Descompose
		//2. Write down the edges list
	}
};

#endif