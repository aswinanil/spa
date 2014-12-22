//Design Extractor traverses AST and compute all the remaining design abstractions: Uses, Modifies for procedure and call statement; Next; Affects
#pragma once

#include "DesignExtractor.h"


	//Default Constructor
	DesignExtractor::DesignExtractor(){}

	//Constructor
	DesignExtractor::DesignExtractor(PKB* pkb):temp(){
		
		ast = pkb->getAST();
		follows = pkb->getFollows();
		parent = pkb->getParent();
		varTable = pkb->getVarTable();
		procTable = pkb->getProcTable();
		modifies = pkb->getModifies();
		uses = pkb->getUses();
		calls = pkb->getCalls();

		flag = false;
	}

	void DesignExtractor::buildCFG(STMT start, STMT end) {
	Graph.erase(Graph.begin(),Graph.end());
	Block.erase(Block.begin(),Block.end());
	bool then = false;
	bool once = false;
	bool WHILE = false;
	int blockNumber = 0;
	STMT lastThenStatement = -1;
	STMT lastElseStatement = -1;
	STMTLST ifStatements;
	STMTLST whileStatements;
	
	string ifNode = "ifNode";
	string whileNode = "whileNode";
	
	for(int i=start; i<=end; i++ ) {
		STMTLST child = parent->getChild(i);	
		int prevFollows = follows->getFollows(0,i);
		int parentStmt = parent->getParent(i);

		if (child.size() == 0) {// no children, not a container statement
			
			int after = follows->getFollows(1,i);
			if ( after != -1 ) { // has a statement that follows it
				if (Block.size() <= blockNumber)
					Block.push_back(STMTLST());
				Block.at(blockNumber).push_back(i);
			}
			
			else if ( after == -1  ) { // does not have a statement that follows it

						
				
				if (Graph.size() <= blockNumber) {
					if (prevFollows != -1 && (find(ifStatements.begin(),ifStatements.end(), prevFollows)!=ifStatements.end())) { // follows after an "If" statement
							
							cout<<"Block Number= "<<blockNumber<<endl;
							if (lastThenStatement != -1) {
								if(Graph.size() <= blockNumber-1)
									Graph.push_back(STMTLST());
								Graph.at(getBlock(lastThenStatement)).push_back(blockNumber);
							}
						
							if (lastElseStatement != -1) {
								if(Graph.size() <= blockNumber-1)
									Graph.push_back(STMTLST());
								Graph.at(getBlock(lastElseStatement)).push_back(blockNumber);
							}

							lastThenStatement = lastElseStatement = -1;
					}

					if (prevFollows != -1 && (find(whileStatements.begin(),whileStatements.end(), prevFollows)!=whileStatements.end())) {  // follows after "while" statement
							
						Graph.at(getBlock(prevFollows)).push_back(getBlock(i)); 				
					}
				
				}
				if ( parentStmt != -1 && (find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) ) { // check if the parent of current stmt is "ifStmt"
					if (then) { // the "then" of "ifNode"

						if(Block.size() <= blockNumber)
							Block.push_back(STMTLST());
						Block.at(blockNumber).push_back(i);						
						
						blockNumber++;

						lastThenStatement = i;
						then = false;
					}
					else {
						if(Block.size() <= blockNumber)
							Block.push_back(STMTLST());
						Block.at(blockNumber).push_back(i);							

						Graph.at(getBlock(parentStmt)).push_back(getBlock(i));	// link the "if" Block to the "else" block					
						
						blockNumber++;
						
						lastElseStatement = i;
					}									   
					
				}
				
				if (prevFollows == -1 && parentStmt != -1) { // if the parent stmt does not follows any stmt, then get the parent of the parent stmt.
						// or the case when follows of i is (-1,-1) but is a child of a container statement
					if (find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) {// parent is "if" statement
						
						int parentSTMT = parentStmt;
						
						do {
							int parentStmt2 = parent->getParent(parentSTMT);
							if ( parentStmt2 != -1 && (find(whileStatements.begin(),whileStatements.end(), parentStmt2)!=whileStatements.end()) ) {
								if(Graph.size() <= blockNumber)
									Graph.push_back(STMTLST());
								Graph.at(getBlock(i)).push_back(getBlock(parentStmt2));		
								WHILE = true;
							}

							parentSTMT = parentStmt2;
						} while (parentSTMT != -1);		

						parentSTMT = parentStmt;
						if (!WHILE) {
							do {
								int parentStmt2 = parent->getParent(parentSTMT);
								if ( parentStmt2 != -1 && (find(ifStatements.begin(),ifStatements.end(), parentStmt2)!=ifStatements.end()) ){
									if (Graph.size() <= blockNumber)
										Graph.push_back(STMTLST());
									Graph.at(getBlock(i)).push_back(-1);
								}

								parentSTMT = parentStmt2;
							} while (parentSTMT != -1);		
						}			
					}
				}

				if (parentStmt != -1 && (find(whileStatements.begin(),whileStatements.end(), parentStmt)!=whileStatements.end()) ) { // for last stmt in the while loop			

						if(Block.size() <= blockNumber)
								Block.push_back(STMTLST());
						if (getBlock(i) == -1)
							Block.at(blockNumber).push_back(i);
						
						blockNumber++;
			
						if(Graph.size() <= blockNumber-1)
								Graph.push_back(STMTLST());
						Graph.at(getBlock(i)).push_back(getBlock(parentStmt)); // link back to parent while stmt
				}
			
			}
		}
		
		
		
		else if ( child.size()>0 && (ast->getNodeType(i))==whileNode ) { // it is a while statement
			whileStatements.push_back(i);
			
			// while node is a block of itself
			if (Block.size()==0) {
				
				Block.push_back(STMTLST());
				Block.at(blockNumber).push_back(i);

				Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1);
				blockNumber++;
				once = true;
			}
			else if (Block.size() > 0) {				
				if (once) {
					blockNumber--;
					once = false;
				}
				
				Block.push_back(STMTLST());
				Block.at(blockNumber+1).push_back(i); 
				
				if(Graph.size() <= blockNumber)
					Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1);

			
				blockNumber++;

				Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1);

				blockNumber++;
				// check if this while statement has a parent that is also a while statement
				if ( parentStmt != -1  && (find(whileStatements.begin(),whileStatements.end(), parentStmt)!=whileStatements.end()) ) {
					Graph.at(getBlock(i)).push_back(getBlock(parentStmt));
				}

			}
			
		}

		
		
		else if ( child.size()>0 && (ast->getNodeType(i))==ifNode ) { // it is an if statement
			ifStatements.push_back(i);

			if (Block.size() == 0) {
				Block.push_back(STMTLST());
			}
			
			if (Block.size() > 0) {		
				if(Block.size() <= blockNumber)
					Block.push_back(STMTLST());				
			}	
				
				Block.at(blockNumber).push_back(i); // if statements will end the current block

				if (Graph.size() <= blockNumber)
					Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1); 
				
				
				if ( parentStmt != -1 && then == false && (find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) ) {
					Graph.at(getBlock(parentStmt)).push_back(getBlock(i));
				}
				
				blockNumber++;
				then = true;
				
		}	

		if (prevFollows != -1 && (find(ifStatements.begin(),ifStatements.end(), prevFollows)!=ifStatements.end())) { // follows after an "If" statement
					
						if (lastThenStatement != -1) {
							if(Graph.size() <= blockNumber-1)
								Graph.push_back(STMTLST());
							Graph.at(getBlock(lastThenStatement)).push_back(getBlock(i));
						}
						
						if (lastElseStatement != -1) {
							if(Graph.size() <= blockNumber-1)
								Graph.push_back(STMTLST());
							Graph.at(getBlock(lastElseStatement)).push_back(getBlock(i));
						}

						lastThenStatement = lastElseStatement = -1;				
		}

		if (prevFollows != -1 && (find(whileStatements.begin(),whileStatements.end(), prevFollows)!=whileStatements.end())) {  // follows after "while" statement
					
					if(Block.size() <= blockNumber)
						Block.push_back(STMTLST());
					if (getBlock(i) == -1)
						Block.at(blockNumber).push_back(i);	
					
					
						Graph.at(getBlock(prevFollows)).push_back(getBlock(i)); 				
		}


		STMTLST nextBlocks = nextBlock(1,getBlock(i));

		
		if ( i ==  end && getBlock(i) == -1) {
			if(Block.size() <= blockNumber)
				Block.push_back(STMTLST());
			Block.at(blockNumber).push_back(i);	
		}

		if ( i == end && (nextBlocks.size() == 0)) { // ending node						
			if (find(ifStatements.begin(),ifStatements.end(), prevFollows)!=ifStatements.end()) {
				for (int i=0; i<Graph.size();i++) {
					for (int j=0;j<Graph.at(i).size(); j++) {
						if (Graph[i][j] == -1) {
							Graph.at(i).erase(Graph.at(i).begin(), Graph.at(i).end());
							Graph.at(i).push_back(blockNumber);
						}
					}
				}
			}
					Graph.push_back(STMTLST());
					Graph.at(getBlock(i)).push_back(-1);
					break;
			
		}
		else if ( i == end && (nextBlocks.size() > 0)) { // ending node is last statement in a while loop
				if (find(whileStatements.begin(),whileStatements.end(), Block.at(nextBlocks.at(0)).at(0) )!=whileStatements.end()) {
					
					if ( parentStmt != -1) { // check if it is the last while loop to exit or there is parent while loop
						int parentSTMT = parentStmt;
						
						do {
							int parentStmt2 = parent->getParent(parentSTMT);
							if ( parentStmt2 == -1 && (find(whileStatements.begin(),whileStatements.end(), parentSTMT)!=whileStatements.end()) ) {
								Graph.at(getBlock(parentSTMT)).push_back(-1);		
							}

							parentSTMT = parentStmt2;
						} while (parentSTMT != -1);
						break;
					}
					else {
						Graph.at(nextBlocks.at(0)).push_back(-1);
						break;
					}
				}
		}
		
	}
		

}


	//Build CFG for each procedure
	void DesignExtractor::computeCFG(){
	
		int numOfProc = procTable->getNoOfProc();
		//CFG cfg = CFG(ast, follows, parent);

		for(int i=0; i<numOfProc; i++){
			buildCFG(procTable->getFirstStmt(i), procTable->getLastStmt(i));
			CFG cfg = CFG(Graph,Block);
			procTable->insertCFG(i, &cfg);
		}
	}

	string DesignExtractor::computeCalls(){

		string error = "";
		if(flag == false){

			temp = calls->getTemp();		

			for(vector<pair<pair<PROCNAME, PROCNAME>, STMT>>::size_type i = 0; i<temp.size(); i++)
			{
				PROCINDEX caller = procTable->getProcIndex(temp[i].first.first);
				PROCINDEX callee = procTable->getProcIndex(temp[i].first.second);

				if(caller == -1)
				{
					error += "\rProcedure "+ temp[i].first.first+" does not exist. Setting \""+temp[i].first.first+" calls "+temp[i].first.second+"\" fails.\n";
				}
				else if(callee == -1)
				{

					error += "\rProcedure "+ temp[i].first.second+" does not exist. Setting \""+temp[i].first.first+" calls "+temp[i].first.second+"\" fails.\n";
				}
				else
				{
					calls->setCalls(caller, callee);
				
				}

			}

			flag = true;
		}

		return error;
	}

	//compute Modifies and Uses for procedures and call statements
	void DesignExtractor::computeModifiesAndUses(){

		if(flag == false){
			computeCalls();
		}

		//Compute Modifies and Uses for procedures
		int numOfProc = procTable->getNoOfProc();
		PROCLIST procNames = procTable->getAllProcNames();

		for(int i = 0; i<numOfProc; i++){

			PROCINDXLIST myCalleesStar = calls->getCalleesStar(procNames[i]);
			INDEXLIST modifiedByI = procTable->getModifies(procNames[i]);
			INDEXLIST usedByI = procTable->getUses(procNames[i]);

			for(int j = 0; j<myCalleesStar.size(); j++){
				
				INDEXLIST modifiedByJ = procTable->getModifies(procNames[myCalleesStar[j]]);

				for(int k = 0; k<modifiedByJ.size(); k++)
				{
					INDEXLIST::iterator it = find(modifiedByI.begin(), modifiedByI.end(), modifiedByJ[k]);
					if(it == modifiedByI.end()){
						procTable->insertModifies(procNames[i], varTable->getVarName(modifiedByJ[k]));
					}
				}

				INDEXLIST usedByJ = procTable->getUses(procNames[myCalleesStar[j]]);

				for(int k = 0; k<usedByJ.size(); k++)
				{
					INDEXLIST::iterator it = find(usedByI.begin(), usedByI.end(), usedByJ[k]);
					if(it == usedByI.end()){
						procTable->insertUses(procNames[i], varTable->getVarName(usedByJ[k]));
					}
				}
			}
		}

		//Compute Modifies and Uses for call statements
		for(vector<pair<pair<PROCNAME, PROCNAME>, STMT>>::size_type i = 0; i<temp.size(); i++){
		
			INDEXLIST varModified = procTable->getModifies(temp[i].first.second);
			INDEXLIST varUsed = procTable->getUses(temp[i].first.second);
			PROCINDEX callee = procTable->getProcIndex(temp[i].first.second);

			for(int i=0; i<varModified.size(); i++){
				modifies->setModifies(temp[i].second, varTable->getVarName(varModified[i]));
			}

			for(int i=0; i<varUsed.size(); i++){
				uses->setUses(temp[i].second, varTable->getVarName(varUsed[i]));
			}

		}

	}

	
	// private method
STMT DesignExtractor::getBlock(STMT statementNumber) {
	for (size_t i=0; i<Block.size();i++) {
		for (size_t j=0;j<Block.at(i).size(); j++) {
			if (Block[i][j] == statementNumber)
				return i;
		}
	}
	
	return -1;
}

// private method
STMTLST DesignExtractor::nextBlock(ORDER order, STMT blockNumber) {
	STMTLST nextBlock;
	
	if (blockNumber >= Graph.size()) {
		return nextBlock;
	}

	
	if((blockNumber < Graph.size()) && (order==1)) {
		nextBlock = Graph.at(blockNumber);
		return nextBlock;
	}

	if((blockNumber < Graph.size()) && (order==10)) {
		for (size_t i=0; i<Graph.size();i++) {
			for (size_t j=0;j<Graph.at(i).size(); j++) {
				if (Graph[i][j] == blockNumber)
					nextBlock.push_back(i);
			}
		}
		return nextBlock;
	}

	return nextBlock;
}