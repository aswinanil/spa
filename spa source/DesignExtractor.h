//Design Extractor traverses AST and compute all the remaining design abstractions: Uses, Modifies for procedure and call statement; Next; Affects
#pragma once

#include "PKB.h"
#include "CFG.h"

class DesignExtractor{
public:
	//! Default constructor
	DesignExtractor();

	//! Constructor
	DesignExtractor(PKB* pkb);

	//! Methods to compute design abstractions
	//! Note that computeCalls() must be called before computeModifiesAndUses()
	void buildCFG(STMT, STMT);
	void computeCFG();
	string computeCalls(void);
	//! Compute Modifies and Uses for procedures and call statements
	void computeModifiesAndUses(void);

private:
	AST * ast;
	VarTable * varTable;
	Follows * follows;
	Parent * parent;
	Modifies * modifies;
	Uses * uses;
	Calls * calls;
	ProcTable * procTable;

	vector<STMTLST> Graph;
	vector<STMTLST> Block;

	bool flag; //flag = true if computeCalls() has been executed already
	vector<pair<pair<PROCNAME, PROCNAME>, STMT>> temp; //temp storage for call stmts

	STMT getBlock(STMT statementNumber);
	STMTLST nextBlock(ORDER order, STMT blockNumber);
};
