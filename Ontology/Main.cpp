#include <iostream>
#include "Ontology.h"
#include <list>

void printEdgesList(std::list<tEdge> edges);

int main(){

	/*Tree<int> *arbol = new Tree<int>(new Tree<int>(4), 5, new Tree<int>(7));

	arbol->print();

	delete arbol;*/

	Node *arbol = new Node("padre");
	Node *hijo1 = new Node("hijo1");
	Node *hijo2 = new Node("hijo2");
	arbol->addSon(hijo1);
	arbol->addSon(hijo2);
	hijo1->addSon(hijo2);

	arbol->print(0);

	std::list<tEdge> edges;
	arbol->descompose(edges);

	printEdgesList(edges);

	delete arbol;

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

