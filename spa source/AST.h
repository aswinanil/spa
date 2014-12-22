#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "TNode.h"

using namespace std;

/*!  \class AST.
	 \brief A tree representation of the abstract syntatic structure of source code
*/

typedef string NODE_TYPE;
typedef string LINK_TYPE;
typedef int INDEX;
typedef int STMT;

class AST {

public:
	enum Link_Type{parent, children, left, right};
	//! Default constuctor
	AST();
	
	//! Constructor
	AST(TNode root);

	//! get root node of AST
	TNode getRoot();

	//! Create AST node with specified note type, statement number and node value
	/*!
		\param NODE_TYPE AST node type
		\param STMT statement number
		\param CONSTANT value of AST node
		\return a reference to the AST node
	*/
	INDEX createTNode(NODE_TYPE nodeType, STMT stmtNumber, CONSTANT nodeValue);

	//! Form a link with specified node type between nodes N1 and N2
	/*!
		n1 is <LinkType> of n2
		\param Tnode AST node
		\param LINK_TYPE left, right, parent, child
	*/
	void linkNode(TNode n1, TNode n2, LINK_TYPE LinkType);



	//TNode getChild(TNode p, int number);

	//! Get the AST node with given index
	/*!
		\parm INDEX index of AST node
		\return AST node
	*/
	TNode getNode(INDEX index);

	//! Get the AST node type 
	/*!
		\parm Tnode AST node
		\return node type 
	*/
	
//	string getNodeType(TNode n);

	// New method for CFG
	string getNodeType(STMT stmtNumber);

	//! Get the number of child of an AST node 
	/*!
		\parm Tnode AST node
		\return number of children AST nodes
	*/
	int getNoOfChild(TNode n);

	//! Get the node value of an AST node 
	/*!
		\parm Tnode AST node
		\return node value
	*/	
	CONSTANT getNodeValue(TNode n);

	//! Get all stmt number for a given node type
	/*!
		\parm NODE_TYPE AST node type
		\return a list of statement number for a AST node type
	*/
	vector<int> getAllStmtNumByType(NODE_TYPE nodeType);

	//! Get all constants for answering constant queries
	/*!
		\return a list of constants in the AST
	*/
	vector<CONSTANT> getAllConstant();

	//! Check if an AST node matches a node type
	/*!
		\parm Tnode AST node
		\parm NODE_TYPE AST node type
		\return TRUE if AST node n matches nodeType
	*/
	bool isMatch(TNode n, NODE_TYPE nodeType);


	/************************************
	*       Methods for future use 
	************************************/

	// Method to get an array of index for the given node type in the whole AST
	vector<int> getAllByType(NODE_TYPE nodeType);

	TNode getStmtNode(STMT stmtNumber);
	TNode getControlStmt(STMT stmtNumber);

	// Method to get the whole AST data structure
	vector<TNode> getASTVector();
	
private:
	// data structure of AST
	vector<TNode> tree;

	// private method to calculate the index of the TNode in the AST
	INDEX indexOf(TNode n);

	// private method to convert the link type
	int getLinkType(string linkType);

	// private methods to convert nodetype to integer and vice versa
	int convertNodeTypeStringToInteger(NODE_TYPE NodeType);
	string convertNodeTypeIntegerToString(int nodeType);

	// private method to get the stmt number of a node
	STMT stmtOf(TNode n);
};