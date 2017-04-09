#ifndef ONTOLOGY_H
#define ONTOLOGY_H

#include <list>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

typedef struct {
	bool isPair;
	std::string parent;
	std::string son;
} tEdge;

bool operator==(const tEdge &a, const tEdge &b) {
	return a.parent == b.parent && a.son == b.son && a.isPair == b.isPair;
}

class Node {
private:
	std::string name;
	std::list<Node*> sons;
	int numParents;

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

	Node(std::string name) : name(name), numParents(0) {}

	Node(std::string name, std::list<Node*> sons) : name(name), sons(sons), numParents(0) {}

	~Node() {
		if (!sons.empty()){
			for (std::list<Node*>::iterator list_iter = sons.begin(); list_iter != sons.end(); list_iter++) {
				if (((Node*)(*list_iter))->getNumParents() > 1)
					((Node*)(*list_iter))->decreaseNumParents();
				else
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

	void setName(std::string newName) {
		name = newName;
	}

	void addSon(Node* newSon) {
		sons.push_back(newSon);
		newSon->increaseNumParents();
	}

	void print(int tabs){
		for (int i = 0; i < tabs; i++)
			std::cout << '\t';
		std::cout << name << std::endl;
		if (!sons.empty()){
			for (std::list<Node*>::iterator list_iter = sons.begin(); list_iter != sons.end(); list_iter++) {
				((Node*)(*list_iter))->print(tabs + 1);
			}
		}
	}

	void descompose(std::list<tEdge> &edges){
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

				if (!((Node*)(*list_iter))->getSons().empty()){
					((Node*)(*list_iter))->descompose(edges);
				}
			}
		}
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
	std::list<Node*> categoryList;

public:
	Ontology(std::list<Node*> categoryList) : categoryList(categoryList) {}

	Ontology(std::list<tEdge> edges) {
		if (!categoryList.empty()){
			std::unordered_set<tEdge> checkedPairs; //unoreder_set (HashSet) with the edges that have been already inserted in the new Ontology.
			std::unordered_map<std::string, Node*> nodePointersHashSet; //unordered_map (HashMap) with pointers to every Node of the new Ontology.
			std::unordered_map<std::string, bool> singleNodesList; //unordered_map (HashMap) with pointers to every Node of the new Ontology.


			for (std::list<tEdge>::iterator list_iter = edges.begin(); list_iter != edges.end(); list_iter++) {
				if (checkedPairs.find(*list_iter) != checkedPairs.end()){
					//Node* parent = new Node(list_iter->parent);

					if (list_iter->isPair) { //if it is a complete edge (two nodes)
						//Node* son = new Node(list_iter->son);

						std::unordered_map<std::string, Node*>::iterator parent_iter = nodePointersHashSet.find(list_iter->parent);
						std::unordered_map<std::string, Node*>::iterator son_iter = nodePointersHashSet.find(list_iter->son);
						std::unordered_map<std::string, Node*>::iterator end_iter = nodePointersHashSet.end();

						if (parent_iter != end_iter && son_iter != end_iter) {
							//se crean dos nuevos objetos de tipo Node y se aniade uno de ellos como hijo al otro
						}
						else if (parent_iter == end_iter && son_iter != end_iter) {
							//se crea un Node padre y se le aniade como hijo el Node al que apunta son_iter
						}
						else if (parent_iter != end_iter && son_iter == end_iter) {
							//se crea un Node hijo y se le hace hijo del Node apuntado por parent_iter
						}
						else { //Los nodos estan en la ontologia pero no estan conectados
							//se le aniade como hijo el Node apuntado por son_iter al Node apuntado por parent_iter

						}
					}
					else { //if it is a single node
						if (nodePointersHashSet.find(list_iter->parent) == nodePointersHashSet.end()) {
							//PONERLO EN LA LISTA DE HIJOS PRINCIPAL PERO A LA ESPERA POR SI APARECE ALGUN PADRE SUYO
						}
							
					}
					checkedPairs.insert(*list_iter);
				}
			}

		}
	}

	~Ontology() {
		if (!categoryList.empty()) {
			for (std::list<Node*>::iterator list_iter = categoryList.begin(); list_iter != categoryList.end(); list_iter++) {
				delete ((Node*)(*list_iter));
			}
		}

		std::cout << "Ontology deleted" << std::endl;
	}

	void print() {
		if (!categoryList.empty()){
			for (std::list<Node*>::iterator list_iter = categoryList.begin(); list_iter != categoryList.end(); list_iter++) {
				((Node*)(*list_iter))->print(0);
			}
		}
	}

	void descompose(std::list<tEdge> &edges){
		if (!categoryList.empty()){
			for (std::list<Node*>::iterator list_iter = categoryList.begin(); list_iter != categoryList.end(); list_iter++) {
				((Node*)(*list_iter))->descompose(edges);
			}
		}
	}
};

#endif