#include "PQLController.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "SimpleParser.h"

void main () {
	
	/*VarTable * varTable = new VarTable();
	Modifies * modifies = new Modifies(varTable);
	Uses *uses = new Uses(varTable);
	PKB * pkb = new PKB(new AST(), varTable, new Follows(), new Parent(), modifies, uses);
	*/
	VarTable * varTable = new VarTable();
	ProcTable * procTable = new ProcTable(varTable);
	PKB * pkb = new PKB(new AST(), varTable, new Follows(), new Parent(), new Modifies(varTable), new Uses(varTable), procTable, new Calls(procTable));


	ifstream * file = new ifstream("s.txt");
	SimpleParser * parser = new SimpleParser(file, pkb);

	parser->initAST(pkb->getAST());
	parser->initVarTable(pkb->getVarTable());
	parser->initFollows(pkb->getFollows());
	parser->initParent(pkb->getParent());
	parser->initModifies(pkb->getModifies());
	parser->initUses(pkb->getUses());

	parser->run();

	PQLController * pqlc = new PQLController(pkb);
	
	std::string query = "assign a1; Select a1 such that Modifies(a1,\"x\")";

	std::list<std::string> results;
	
	pqlc->passInput(query, results);

	cout << endl;
	std::list<std::string>::const_iterator pos;
	for(pos=results.begin(); pos!=results.end(); pos++){
		cout << *pos << endl;
	}

	system("pause");




	
}