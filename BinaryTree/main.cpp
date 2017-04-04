#include <iostream>
#include "BinaryTree.h"

using namespace std;


int main(){

	Tree<int> *arbol = new Tree<int>(new Tree<int>(4), 5, new Tree<int>(7));

	arbol->print();

	delete arbol;
	return 0;
}