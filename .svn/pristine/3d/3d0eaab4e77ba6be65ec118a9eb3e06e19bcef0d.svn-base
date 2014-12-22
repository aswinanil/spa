#pragma once
//#include "PKB.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>


using namespace std;
typedef int STMT;
typedef short ORDER;
typedef vector<STMT> STMTLST;

class CFG
{
public:
	//! Default constuctor
	CFG();
	//! Destructor
	~CFG();
	//! Constructor
	CFG(vector<STMTLST>, vector<STMTLST>);
	//! Build CFG of a procedure
	/*!
		\param STMT start - The first statement number of the procedure
		\param STMT end - The last statement number of the procedure
	*/
	//void buildCFG(STMT start, STMT end);

   //! If order = 0, return statements that directly precedes programLine ,  If order = 1, return statements that appears directly after programLine
   /*!
	 \param order 0 or 1
	 \parm STMT program line number
	 \return otherwise empty STMTLST
   */
	STMTLST nextStatement (ORDER order, STMT programLine);

   //! If order = 0, return statements that precedes programLine ,  If order = 1, return statements that appears after programLine
   /*!
	 \param order 0 or 1
	 \parm STMT program line number
	 \return otherwise empty STMTLST
   */
	STMTLST nextStatementStar (ORDER order, STMT programLine);


   //! Check if n2 appears directly after n1
   /*!
	 \parm STMT program line number 1
	 \parm STMT program line number 2
	 \return TRUE if n2 appears directly after n1, otherwise return FALSE
   */
	bool isNext(STMT n1, STMT n2);

	//! Check if there is a control flow path from n1 to n2
   /*!
	 \parm STMT program line number 1
	 \parm STMT program line number 2
	 \return TRUE if there is a control flow path from n1 to n2, otherwise return FALSE
   */
	bool isNextStar(STMT n1, STMT n2);

	
	// For testing
	vector<STMTLST> getCFG();
	vector<STMTLST> getBlock();
private:
	vector<STMTLST> Graph;
	vector<STMTLST> Block;
	vector<bool> visited;
	bool skip;

	//AST* ast;
	//Follows* follows;
	//Parent* parent;

	vector<bool> DFS(STMT programLine); // Depth first search
	STMT getBlock(STMT statementNumber);
	STMTLST nextBlock(ORDER order, STMT blockNumber);
};

