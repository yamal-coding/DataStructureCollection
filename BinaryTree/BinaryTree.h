#ifndef TREE_H
#define TREE_H

#include <iostream>

template <class T> class Tree {
private:
	Tree<T> *left;
	T raiz;
	Tree<T> *right;


public:
	Tree (T raiz) : left(NULL), raiz(raiz), right(NULL) {}

	Tree(Tree<T> *left, T raiz, Tree<T> *right) : left(left), raiz(raiz), right(right) {}

	Tree(Tree<T> *left, T raiz) : left(left), raiz(raiz) {}

	Tree(T raiz, Tree<T> *right) : raiz(raiz), right(right) {}

	~Tree(){
		if (left != NULL) {
			delete left;
			std::cout << "Hijo izquierdo liberado" << std::endl;
		}
		if (right != NULL) {
			delete right;
			std::cout << "Hijo derecho liberado" << std::endl;
		}

		std::cout << "Nodo liberado" << std::endl;
	}

	void print(){
		if (left != NULL)
			left->print();
		std::cout << raiz << std::endl;
		if (right != NULL)
			right->print();
	}

};


#endif