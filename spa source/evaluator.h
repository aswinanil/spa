#pragma once
#include "queryTree.h"
#include "PKB.h"
#include "AST.h"
#include "TNode.h"
#include <string>
#include <vector>
#include "Follows.h"
#include "Parent.h"
#include "Uses.h"
#include "Modifies.h"
#include "VarTable.h"
#include "Table.h"

using namespace std;
typedef vector<string> QUERYBRANCH;
typedef vector<string> PATTERNBRANCH;
typedef vector<string> WITHBRANCH;

class evaluator
{
public:
	evaluator(void);
	~evaluator(void);
	evaluator(queryTree _queryTree);
	AST* ast;
	Follows* follows;
	Parent* parent;
	Uses* uses;
	Modifies* modifies;
	VarTable* varTable;
	Table table;

	bool queryHasTrue;
	bool patternHasTrue;
	bool withHasTrue;

	//insertPKB
	void insertAST(AST* _ast);
	void insertFollows(Follows* _follows);
	void insertParent(Parent* _parent);
	void insertModifies(Modifies* _modifies);
	void insertUses(Uses* _uses);
	void insertVarTable(VarTable* _varTable);

	vector<string> evaluateQuery(queryTree* _queryTree);


private:
	vector<vector<string>> getAllByType(string _select, string _selectType, queryTree* _queryTree);
	
	////////cs3202////////
	void evaluateSuchThat(string _select, string _selectType, queryTree* _queryTree);
	void evaluatePattern(string _select, string _selectType, queryTree* _queryTree);
	void evaluateWith(string _select, string _selectType, queryTree* _queryTree);
	vector<string> getAllPossibleByType(string _selectType);
	void evaluateBranch(string _queryType, string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);
	void evaluateFollowsBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);
	void evaluateFollowsStarBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);
	void evaluateParentBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);
	void evaluateParentStarBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);
	void evaluateUsesBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);
	void evaluateModifiesBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);
	void evaluatePatternBranch(string _patternStmt, string _typeStmt, string _typeLeft, string _valueLeft, string _typeRight, 
		string _valueRight, vector<VALUE> _allStmtEntry, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);

	vector<vector<string>> convertSolution(vector<string> _solution);
	////////cs3202////////


};

