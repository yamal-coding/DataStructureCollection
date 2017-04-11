#include <iostream>
#include "Ontology.h"
#include <list>
#include <unordered_set>
#include <exception>

void printEdgesList(std::list<tEdge> edges);

int main(){
	Node *arbol1 = new Node("padre1");
	Node *hijo1 = new Node("hijo1");
	Node *hijo2 = new Node("hijo2");
	arbol1->addSon(hijo1);
	arbol1->addSon(hijo2);

	Node *arbol2 = new Node("padre1");
	Node *hijo3 = new Node("hijo3");
	Node *hijo4 = new Node("hijo4");
	arbol2->addSon(hijo3);
	arbol2->addSon(hijo4);

	std::list<tEdge> edges1;
	std::unordered_set<std::string> visited1;
	arbol1->descompose(edges1, visited1);

	std::list<tEdge> edges2;
	std::unordered_set<std::string> visited2;
	arbol2->descompose(edges2, visited2);

	delete arbol1;
	delete arbol2;

	try {
		Ontology* ontology = new Ontology(edges1, edges2);
		
		std::list<tEdge> edges3;
		ontology->descompose(edges3);
		printEdgesList(edges3);
		
		std::cout << std::endl << std::endl;

		ontology->print();

		delete ontology;
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	std::cout << std::endl << std::endl;

	system("pause");

	return 0;
}

void printEdgesList(std::list<tEdge> edges) {
	if (!edges.empty()){
		for (std::list<tEdge>::iterator list_iter = edges.begin(); list_iter != edges.end(); list_iter++) {
			std::cout << "(" << tEdge(*list_iter).parent;
			if (tEdge(*list_iter).isPair)
				std::cout << ", " << tEdge(*list_iter).son;
			std::cout << ")" << std::endl;
		}
	}
}

