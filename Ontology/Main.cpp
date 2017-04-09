#include <iostream>
#include "Ontology.h"
#include <list>

void printEdgesList(std::list<tEdge> edges);

int main(){
	Node *arbol = new Node("padre");
	Node *hijo1 = new Node("hijo1");
	Node *hijo2 = new Node("hijo2");
	Node *hijo3 = new Node("hijo3");
	Node *hijo4 = new Node("hijo4");
	Node *hijo5 = new Node("hijo5");
	Node *hijo6 = new Node("hijo6");
	Node *hijo7 = new Node("hijo7");
	Node *hijo8 = new Node("hijo8");
	Node *hijo9 = new Node("hijo9");
	Node *hijo10 = new Node("hijo10");
	Node *hijo11 = new Node("hijo11");

	arbol->addSon(hijo1);
	arbol->addSon(hijo2);
	hijo3->addSon(hijo4);
	hijo4->addSon(hijo5);
	hijo4->addSon(hijo6);
	hijo7->addSon(hijo8);
	hijo8->addSon(hijo9);
	hijo8->addSon(hijo10);
	hijo9->addSon(hijo11);
	hijo1->addSon(hijo7);
	hijo2->addSon(hijo3);

	arbol->print(0);

	std::list<tEdge> edges;
	arbol->descompose(edges);
	printEdgesList(edges);

	std::cout << std::endl << std::endl;

	delete arbol;

	tEdge newEdge;
	newEdge.isPair = true;
	newEdge.parent = "hijo12";
	newEdge.son = "hijo6";

	edges.push_back(newEdge);

	Ontology* ontology = new Ontology(edges);

	ontology->print();

	delete ontology;

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

