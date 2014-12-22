#pragma once

#include "AST.h"

// Default constructor
AST::AST(){
}

AST::AST(TNode root) {
	tree.insert(tree.begin(), root);
}

TNode AST::getRoot() {
	return tree.front();
}

INDEX AST::createTNode(NODE_TYPE NodeType, STMT stmtNumber, CONSTANT nodeValue) {
	int nodeType = convertNodeTypeStringToInteger(NodeType);
	TNode newNode (nodeType, stmtNumber);
	newNode.setIndex(tree.size());

	if (nodeType == 5 || nodeType == 7)
		newNode.setNodeValue(nodeValue);

	tree.push_back(newNode);

	return newNode.getIndex();
}

void AST::linkNode(TNode n1, TNode n2, string LinkType) {
	int n1Index = indexOf(n1);
	int n2Index = indexOf(n2);

	switch (getLinkType(LinkType)) {
	case AST::parent:
		tree.at(n1Index).addChild(n2Index);
		tree.at(n2Index).setParent(n1Index);
		break;
	case AST::children:
		tree.at(n2Index).addChild(n1Index);
		tree.at(n1Index).setParent(n2Index);
		break;
	case AST::left:
		tree.at(n1Index).setRightSibling(n2Index);
		tree.at(n2Index).setLeftSibling(n1Index);
		break;
	case AST::right:
		tree.at(n2Index).setRightSibling(n1Index);
		tree.at(n1Index).setLeftSibling(n2Index);
		break;
	}
}


/*TNode AST::getChild(TNode p, int number) {
	if (number < p.getChildren().size())
		return tree.at(p.getChildren().at(number-1));
	else {
		TNode newNode(-1 , -1);
		return newNode; 
	}
}
*/

TNode AST::getNode(INDEX index) {
	return tree.at(index);
}

/*
string AST::getNodeType(TNode n) {
	return convertNodeTypeIntegerToString(n.getNodeType());
}
*/

// new Method for CFG
string AST::getNodeType(STMT stmtNumber) {
	vector<TNode>::iterator it;
	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getStmtNumber() == stmtNumber) {
			if ( it->getNodeType() == convertNodeTypeStringToInteger("whileNode") )
				return "whileNode";
			if ( it->getNodeType() == convertNodeTypeStringToInteger("ifNode") )
				return "ifNode";
		}
	}
	
	return NULL;
}

int AST::getNoOfChild(TNode n) {
	return n.getChildren().size();
}

CONSTANT AST::getNodeValue(TNode n) {
	return n.getNodeValue();
}


vector<int> AST::getAllStmtNumByType(NODE_TYPE nodeType) {
	vector<int> stmtNumTree;
	vector<TNode>::iterator it;
	int nodeTypeInteger = convertNodeTypeStringToInteger(nodeType);
	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getNodeType() == nodeTypeInteger)
			stmtNumTree.push_back(stmtOf(*it));
	}
	return stmtNumTree;
}

vector<CONSTANT> AST::getAllConstant() {
	vector<CONSTANT> constantList;
	vector<TNode>::iterator it;

	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getNodeType() == 7)
			constantList.push_back(getNodeValue(*it));
	}

	return constantList;
}

bool AST::isMatch(TNode n, NODE_TYPE nodeType) {
	if (n.getNodeType() == convertNodeTypeStringToInteger(nodeType))
		return true;
	else
		return false;
}


/************************************
*       Methods for future use 
************************************/

vector<int> AST::getAllByType(NODE_TYPE nodeType) {
	vector<int> indexInTree;
	vector<TNode>::iterator it;

	int nodeTypeInteger = convertNodeTypeStringToInteger(nodeType);

	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getNodeType() == nodeTypeInteger)
			indexInTree.push_back(indexOf(*it));
	}

	return indexInTree;
}


TNode AST::getStmtNode(STMT stmtNumber) {
	vector<TNode>::iterator it;

	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getStmtNumber() == stmtNumber && (it->getNodeType() == 3 || it->getNodeType() == 4 ))
			return *it;
	}

	TNode blank;
	return blank;
}

TNode AST::getControlStmt(STMT stmtNumber) {
	vector<TNode>::iterator it;

	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getStmtNumber() == stmtNumber &&  it->getNodeType() == 4 )
			return *it;
	}

	TNode blank;
	return blank;
}

vector<TNode> AST::getASTVector(){
	return this->tree;
}


/**********************************************
*			Private Methods
**********************************************/

// private method to calculate the index of the TNode in the tree
INDEX AST::indexOf(TNode n) {
	return n.getIndex();
}

// private method to convert the link type from string to integer
int AST::getLinkType(string linkTypestring) {
	if (linkTypestring == "parent")
		return AST::parent;

	if (linkTypestring == "children")
		return AST::children;

	if (linkTypestring == "left")
		return AST::left;

	if (linkTypestring == "right")
		return AST::right;

	return -1;
}


//private methods to convert nodetype to integer and vice versa
int AST::convertNodeTypeStringToInteger(NODE_TYPE NodeType) {
	const char *myNodeType = NodeType.c_str();
	int nodeType = -1;

	if ( strcmp(myNodeType, "programNode") == 0)
		nodeType = 0;

	if ( strcmp(myNodeType, "procedureNode") == 0)
		nodeType = 1;

	if ( strcmp(myNodeType, "stmtLstNode") == 0)
		nodeType = 2;

	if ( strcmp(myNodeType, "assignNode") == 0)
		nodeType = 3;

	if ( strcmp(myNodeType, "whileNode") == 0)
		nodeType = 4;

	if ( strcmp(myNodeType, "varNode") == 0)
		nodeType = 5;

	if ( strcmp(myNodeType, "plusNode") == 0)
		nodeType = 6;

	if ( strcmp(myNodeType, "constantNode") == 0)
		nodeType = 7;

	if ( strcmp(myNodeType, "minusNode") == 0)
		nodeType = 8;

	if ( strcmp(myNodeType, "timesNode") == 0)
		nodeType = 9;

	if ( strcmp(myNodeType, "ifNode") == 0)
		nodeType = 10;

	if ( strcmp(myNodeType, "callNode") == 0)
		nodeType = 11;

	return nodeType;
}

string AST::convertNodeTypeIntegerToString(int nodeType) {
	switch (nodeType) {
	case 0: 
		return "programNode";
		break;
	case 1:
		return "procedureNode";
		break;
	case 2:
		return "stmtLstNode";
		break;
	case 3: 
		return "assignNode";
		break;
	case 4:
		return "whileNode";
		break;
	case 5:
		return "varNode";
		break;
	case 6:
		return "plusNode";
		break;
	case 7: 
		return "constantNode";
		break;
	case 8:
		return "minusNode";
		break;
	case 9:
		return "timesNode";
		break;
	case 10:
		return "ifNode";
		break;
	case 11:
		return "callNode";
		break;
	}
}

STMT AST::stmtOf(TNode n) {
	return n.getStmtNumber();
}


