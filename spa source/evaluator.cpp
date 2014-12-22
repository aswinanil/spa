#include "evaluator.h"
#include <iostream>
#include "AbstractWrapper.h"
using namespace std;



evaluator::evaluator(void)
{
	

}
evaluator::~evaluator(void){

}
void evaluator::insertAST(AST* _ast){
	ast = _ast;
}
void evaluator::insertFollows(Follows* _follows){
	follows = _follows;
}
void evaluator::insertParent(Parent* _parent){
	parent = _parent;
}
void evaluator::insertUses(Uses* _uses){
	uses = _uses;
}
void evaluator::insertModifies(Modifies* _modifies){
	modifies = _modifies;
}
void evaluator::insertVarTable(VarTable* _varTable){
	varTable = _varTable;
}

vector<string> evaluator::evaluateQuery(queryTree* _queryTree){
	vector<string> select1=_queryTree->getSelect(0);
	string select = select1.at(0);
	string selectType =  select1.at(1);
	//string select2 = _queryTree->getSelect(1).at(0);
	//string selectType2 =  _queryTree->getSelect(1).at(1);
	vector<string> results;
	queryHasTrue=false;
	patternHasTrue=false;
	withHasTrue=true;
	if(select!=""){
		if(_queryTree->getSuchThatQuerySize()>0){
			 evaluateSuchThat(select ,selectType,_queryTree);
		}else{	queryHasTrue=true;
		}
		if(_queryTree->getPatternQuerySize()>0){
			evaluatePattern(select, selectType, _queryTree);
		}else{	patternHasTrue=true;
		}
		if(_queryTree->getWithQuerySize()>0){
			evaluateWith(select, selectType, _queryTree);
		}
		if(queryHasTrue && patternHasTrue && withHasTrue){
			if(table.hasColumns(select)){
				results = table.getColumn(select);
			}else{
				results = getAllPossibleByType(selectType);
			}
		}
	}
	return results;
}

void evaluator::evaluateWith(string _select, string _selectType, queryTree* _queryTree){
	for(int i=0; i<_queryTree->getWithQuerySize(); i++){
		WITHBRANCH withTree = _queryTree->getWithQuery(i);
		string value0 = withTree.at(0);
		string value1 = withTree.at(1);
		string value2 = withTree.at(2);
		string value3 = withTree.at(3);
		string value4 = withTree.at(4);
		string value5 = withTree.at(5);
		string value6 = withTree.at(6);
		string value7 = withTree.at(7);
		vector<string> allPossibleStmt;
		
		if(value0=="attRef"){
			if(value3=="stmt#"){
				if(table.hasColumns(value1)){
					allPossibleStmt = table.getColumn(value1);
				}else{
					allPossibleStmt = getAllPossibleByType(value2);
				}	
				if(value4=="Ident"){
					withHasTrue = false;
				}else if(value4=="Integer"){
					vector<string> correctEntry;
					for(int i=0;i<allPossibleStmt.size();i++){
						if(allPossibleStmt.at(i)==value5){
							correctEntry.push_back(value5);
							table.insert(value1, correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}else if(value4=="prog_line"){
					withHasTrue = false;
				}else if(value4=="attrRef"){
					//to implement if value3==value7
					if(value3==value7){
						vector<string> allPossibleStmt2; 
						if(table.hasColumns(value5)){
							allPossibleStmt2 = table.getColumn(value5);
						}else{
							allPossibleStmt2 = getAllPossibleByType(value6);
						}						
						for(int i=0;i<allPossibleStmt.size();i++){
							vector<string> correctEntry;
							string entryLeft = allPossibleStmt.at(i);
							for(int j=0;j<allPossibleStmt2.size(); j++){
								string entryRight = allPossibleStmt2.at(j);
								if(entryLeft==entryRight){
									correctEntry.push_back(entryRight);
									table.insert(value1, entryLeft, value5, correctEntry);
									withHasTrue=true;
									break;
								}
							}
						}
					}
				}
			}else if(value3=="varName"){
				if(table.hasColumns(value1)){
					allPossibleStmt = table.getColumn(value1);
				}else{
					allPossibleStmt = getAllPossibleByType(value2);
				}
				if(value4=="Ident"){
					vector<string> correctEntry;
					for(int i=0;i<allPossibleStmt.size();i++){
						if(allPossibleStmt.at(i)==value5){
							correctEntry.push_back(value5);
						}
					}
					if(correctEntry.size()>0){
						table.insert(value1, correctEntry);
						withHasTrue=true;
					}
				}else if(value4=="Integer"){
					withHasTrue = false;
				}else if(value4=="prog_line"){
					withHasTrue = false;
				}else if(value4=="attrRef"){
					if(value3==value7){
						vector<string> allPossibleStmt2;
						if(table.hasColumns(value5)){
							allPossibleStmt2 = table.getColumn(value5);
						}else{
							allPossibleStmt2 = getAllPossibleByType(value6);
						}
						for(int i=0;i<allPossibleStmt.size();i++){
							vector<string> correctEntry;
							string entryLeft = allPossibleStmt.at(i);
							for(int j=0;j<allPossibleStmt2.size(); j++){
								string entryRight = allPossibleStmt2.at(j);
								if(entryLeft==entryRight){
									correctEntry.push_back(entryRight);
									table.insert(value1, entryLeft, value5, correctEntry);
									withHasTrue=true;
									break;
								}
							}
						}
					}
				}
			}else if(value3=="procName"){
				if(value4=="Ident"){
					vector<string> correctEntry;
					if(table.hasColumns(value1)){
						allPossibleStmt = table.getColumn(value1);
					}else{
						allPossibleStmt = getAllPossibleByType(value2);
					}
					for(int i=0;i<allPossibleStmt.size();i++){
						if(allPossibleStmt.at(i)==value5){
							correctEntry.push_back(value5);
							table.insert(value1, correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}else if(value4=="Integer"){
					//withHasTrue==false;
				}else if(value4=="prog_line"){
					//withHasTrue==false;
				}else if(value4=="attrRef"){
					if(value7=="stmt#"){
						//withHasTrue==false;
					}else if(value7=="varName" || value7=="procName"){
						vector<string> correctEntry;
						vector<string> allPossibleStmt2;
						if(table.hasColumns(value5)){
							allPossibleStmt2 = table.getColumn(value5);
						}else{
							allPossibleStmt2 = getAllPossibleByType(value6);
						}					
						for(int i=0;i<allPossibleStmt2.size(); i++){
							if(value1==allPossibleStmt2.at(i)){
								correctEntry.push_back(value1);
								table.insert(value5, correctEntry);
								withHasTrue=true;
								break;
							}
						}
					}else if(value7=="value"){
						//withHasTrue==false;
					}
				}
			}else if(value3=="value"){
				//to be confirmed
				if(value4=="Ident"){
				}else if(value4=="Integer"){
				}else if(value4=="prog_line"){
				}else if(value4=="attrRef"){
				}
			}
		}
		else if(value0=="Integer"){
			if(value4=="Ident"){
			}else if(value4=="Integer"){
				if(value1==value5){
					withHasTrue==true;
				}
			}else if(value4=="prog_line"){
				vector<string> correctEntry;
				vector<string> allPossibleStmt2;
				if(table.hasColumns(value5)){
					allPossibleStmt2 = table.getColumn(value5);
				}else{
					allPossibleStmt2 = getAllPossibleByType("prog_line");
				}					
				for(int i=0;i<allPossibleStmt2.size(); i++){
					if(value1==allPossibleStmt2.at(i)){
						correctEntry.push_back(value1);
						table.insert(value5, correctEntry);
						withHasTrue=true;
						break;
					}
				}
			}else if(value4=="attrRef"){
				if(value7=="stmt#"){
					vector<string> correctEntry;
					vector<string> allPossibleStmt2;
					if(table.hasColumns(value5)){
						allPossibleStmt2 = table.getColumn(value5);
					}else{
						allPossibleStmt2 = getAllPossibleByType("prog_line");
					}					
					for(int i=0;i<allPossibleStmt2.size(); i++){
						if(value1==allPossibleStmt2.at(i)){
							correctEntry.push_back(value1);
							table.insert(value5, correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}else if(value7=="varName"){
					//withHasTrue=false;
				}else if(value7=="procName"){
					//withHasTrue=false;
				}else if(value7=="value"){
				}
			}
		}else if(value0=="Ident"){
			if(value4=="Ident"){
				if(value1==value5){
					withHasTrue==true;
				}
			}else if(value4=="Integer"){
				//withHasTrue=false;
			}else if(value4=="prog_line"){
				//withHasTrue=false;
			}else if(value4=="attrRef"){
				if(value7=="stmt#"){
					//withHasTrue=false;
				}else if(value7=="varName" || value7=="procName"){
					vector<string> correctEntry;
					vector<string> allPossibleStmt2;
					if(table.hasColumns(value5)){
						allPossibleStmt2 = table.getColumn(value5);
					}else{
						allPossibleStmt2 = getAllPossibleByType(value6);
					}					
					for(int i=0;i<allPossibleStmt2.size(); i++){
						if(value1==allPossibleStmt2.at(i)){
							correctEntry.push_back(value1);
							table.insert(value5, correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}else if(value7=="value"){
				}
			}
		}else if(value0=="prog_line"){
			//get all possible prog_line first 
			if(table.hasColumns(value1)){
				allPossibleStmt = table.getColumn(value1);
			}else{
				allPossibleStmt = getAllPossibleByType("prog_line");
			}
			if(value4=="Ident"){
				//withHasTrue=false;
			}else if(value4=="Integer"){
				vector<string> correctEntry;
				for(int i=0; i<allPossibleStmt.size(); i++){
					if(value5==allPossibleStmt.at(i)){
						correctEntry.push_back(value5);
						table.insert(value1, correctEntry);
						withHasTrue=true;
						break;
					}
				}
			}else if(value4=="prog_line"){
				vector<string> allPossibleStmt2;
				if(table.hasColumns(value5)){
					allPossibleStmt2 = table.getColumn(value5);
				}else{
					allPossibleStmt2 = getAllPossibleByType("prog_line");
				}
				for(int i=0; i<allPossibleStmt.size(); i++){
					vector<string> correctEntry;
					for(int j=0; j<allPossibleStmt2.size(); j++){
						if(allPossibleStmt.at(i)==allPossibleStmt2.at(j)){
							correctEntry.push_back(allPossibleStmt.at(i));
							table.insert(allPossibleStmt.at(i), correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}
			}else if(value4=="attrRef"){
				if(value7=="stmt#"){
					vector<string> allPossibleStmt2;
					if(table.hasColumns(value5)){
						allPossibleStmt2 = table.getColumn(value5);
					}else{
						allPossibleStmt2 = getAllPossibleByType(value6);
					}
					for(int i=0; i<allPossibleStmt.size(); i++){
						vector<string> correctEntry;
						for(int j=0; j<allPossibleStmt2.size(); j++){
							if(allPossibleStmt.at(i)==allPossibleStmt2.at(j)){
								correctEntry.push_back(allPossibleStmt.at(i));
								table.insert(allPossibleStmt.at(i), correctEntry);
								withHasTrue=true;
								break;
							}
						}
					}
				}else if(value7=="varName"){
					//withHasTrue=false;
				}else if(value7=="procName"){
					//withHasTrue=false;
				}else if(value7=="value"){
				}
			}
		}
		
		table.shrinkTable();
	}
}

void evaluator::evaluatePattern(string _select, string _selectType, queryTree* _queryTree){
	for(int i=0; i<_queryTree->getPatternQuerySize(); i++){
		PATTERNBRANCH patternTree = _queryTree->getPatternQuery(i);
		string patternStmt = patternTree.at(0);
		string patternTypeStmt = patternTree.at(1);
		string patternValueLeft = patternTree.at(2);
		string patternValueRight = patternTree.at(4);
		string patternTypeLeft = patternTree.at(3);
		string patternTypeRight = patternTree.at(5);
		vector<string>	allStmtEntry;
		vector<string>	allLeftEntry;
		vector<string>	allRightEntry;
		if(table.hasColumns(patternStmt)){
			allStmtEntry = table.getColumn(patternStmt);
		}else{
			allStmtEntry=getAllPossibleByType(patternTypeStmt);
		}
		if(patternTypeLeft== "Ident"){
			allLeftEntry.push_back(patternValueLeft);
		}else if(patternTypeLeft=="variable"){
			if(table.hasColumns(patternValueLeft)){
				allLeftEntry = table.getColumn(patternValueLeft);
			}else{
				allLeftEntry = getAllPossibleByType(patternTypeLeft); 
			}
		}else if(patternTypeLeft=="_"){
			allLeftEntry = getAllPossibleByType("variable"); 
		}
		if(patternTypeRight=="Var_name"){
			allRightEntry.push_back(patternValueRight);
		}else if(patternTypeRight=="_"){
			allRightEntry = getAllPossibleByType("variable"); 
		}
		evaluatePatternBranch(patternStmt,patternTypeStmt,patternTypeLeft,patternValueLeft,patternTypeRight,patternValueRight, allStmtEntry, allLeftEntry,allRightEntry);
		table.shrinkTable();
	}
}

void evaluator::evaluateSuchThat(string _select, string _selectType, queryTree* _queryTree){
	if (_queryTree->getSuchThatQuerySize() == 0){
		//return convertSolution(getAllPossibleByType(_selectType));
	}else{
		for(int i=0; i<_queryTree->getSuchThatQuerySize(); i++){
			vector<VALUE> allLeftEntry;
			vector<VALUE> allRightEntry;
			QUERYBRANCH suchThatQuery = _queryTree->getSuchThatQuery(i);
			string typeLeft = suchThatQuery.at(2);
			string typeRight = suchThatQuery.at(4);
			string valueLeft = suchThatQuery.at(1);
			string valueRight = suchThatQuery.at(3);
			string queryType = suchThatQuery.at(0);
			string tableValueLeft;
			string tableValueRight;
			//checks left
			if(typeLeft=="Integer"){
				allLeftEntry.push_back(valueLeft);
				tableValueLeft="_";
			}else if(typeLeft=="_"){
				if(queryType=="Follows" || queryType=="Follows*" || 
					queryType=="Parent" || queryType=="Parent*" ||
				    queryType=="Uses" || queryType=="Modifies*"){
					allLeftEntry = getAllPossibleByType("stmt");
				}
				tableValueLeft="_";
			}//valueLeft is a declaration
			else if(table.hasColumns(valueLeft)){
				allLeftEntry = table.getColumn(valueLeft);
				tableValueLeft=valueLeft;
			}else{
				allLeftEntry = getAllPossibleByType(typeLeft);
				tableValueLeft=valueLeft;
			}
			//checks right
			if(typeRight=="Integer"){
				allRightEntry.push_back(valueRight);
				tableValueRight="_";
			}else if(typeRight=="Ident"){
				allRightEntry.push_back(valueRight);
				tableValueRight="_";
			}else if(typeRight=="_"){
				if(queryType=="Follows" || queryType=="Follows*" || 
					queryType=="Parent" || queryType=="Parent*"){
					allRightEntry = getAllPossibleByType("stmt");
				}else if(queryType=="Uses" || queryType=="Modifies"){
					allRightEntry = getAllPossibleByType("variable");
				}
				tableValueRight="_";
			}//valueRight is a declaration
			else if(table.hasColumns(valueRight)){
				allRightEntry = table.getColumn(valueRight);
				tableValueRight=valueRight;
			}else{
				allRightEntry = getAllPossibleByType(typeRight);
				tableValueRight=valueRight;
			}
			evaluateBranch(queryType,tableValueLeft, tableValueRight, allLeftEntry, allRightEntry);
			table.shrinkTable();
		}
	}
}

void evaluator::evaluateBranch(string _queryType, string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_queryType=="Follows"){
		evaluateFollowsBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Follows*"){
		evaluateFollowsStarBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Parent"){
		evaluateParentBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Parent*"){
		evaluateParentStarBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Uses"){
		evaluateUsesBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Modifies"){
		evaluateModifiesBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}

}

void evaluator::evaluatePatternBranch(string _patternStmt, string _typeStmt, string _typeLeft, string _valueLeft, string _typeRight,
		string _valueRight, vector<VALUE> _allStmtEntry, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	bool patternExist;
	vector<string> correctEntry;
	for(int i=0; i<_allStmtEntry.size(); i++){
		patternExist = false;
		string entryStmt = _allStmtEntry.at(i);
		int intStmt = atoi( entryStmt.c_str() );
		int allLeftSize = _allLeftEntry.size(); 
		int allRightSize = _allRightEntry.size(); 
		if(_typeRight=="_" && _typeLeft=="_" && allLeftSize>0 && allRightSize>0){
			patternExist = true;
		}else{
			for(int j=0; j<allLeftSize; j++){
				string entryLeft = _allLeftEntry.at(j); 
				if(modifies->isModifies(intStmt,entryLeft)){
					for(int k=0; k<allRightSize; k++){
						string entryRight = _allRightEntry.at(k);
						if(uses->isUses(intStmt, entryRight) || _typeRight=="_"){
							patternExist = true;
							break;
						}
					}
				}
				if(patternExist){ break;}
			}
		}
		if(patternExist){
			correctEntry.push_back(entryStmt);
			patternHasTrue = true;
		}
	}
	table.insert(_patternStmt, correctEntry);
}

void evaluator::evaluateFollowsBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(follows->isFollows(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
				queryHasTrue = true;
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				int intRight = atoi( entryRight.c_str() );
				if(follows->isFollows(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateFollowsStarBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(follows->isFollows(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
			}
			if(correctEntry.size()>0){
				queryHasTrue = true;
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				int intRight = atoi( entryRight.c_str() );
				if(follows->isFollowsStar(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateParentBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(follows->isFollows(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
			}
			if(correctEntry.size()>0){
				queryHasTrue = true;
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				int intRight = atoi( entryRight.c_str() );
				if(parent->isParent(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateParentStarBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(follows->isFollows(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
			}
			if(correctEntry.size()>0){
				queryHasTrue = true;
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				int intRight = atoi( entryRight.c_str() );
				if(parent->isParentStar(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateUsesBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	for(int i=0; i<_allLeftEntry.size(); i++){
		string entryLeft = _allLeftEntry.at(i);
		vector<VALUE> correctEntry; 
		for(int j=0; j<_allRightEntry.size(); j++){
			string entryRight = _allRightEntry.at(j); 
			int intLeft = atoi( entryLeft.c_str() );
			if(uses->isUses(intLeft, entryRight)){
				correctEntry.push_back(entryRight);
				queryHasTrue = true;
			}
		}
		if(correctEntry.size()>0){
			table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
		}
	}
}

void evaluator::evaluateModifiesBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	for(int i=0; i<_allLeftEntry.size(); i++){
		string entryLeft = _allLeftEntry.at(i);
		vector<VALUE> correctEntry; 
		for(int j=0; j<_allRightEntry.size(); j++){
			string entryRight = _allRightEntry.at(j); 
			int intLeft = atoi( entryLeft.c_str() );
			if(modifies->isModifies(intLeft, entryRight)){
				correctEntry.push_back(entryRight);
				queryHasTrue = true;
			}
		}
		if(correctEntry.size()>0){
			table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
		}
	}
}

vector<vector<string>> convertToStringVectorResults(vector<int> _intResults, string _type){
	vector<vector<string>> result;
	for(int i=0; i<_intResults.size(); i++){	
		string stringResult = std::to_string(static_cast<long long>( _intResults.at(i)));
		vector<string> tempStringResult;
		tempStringResult.push_back(stringResult);
		tempStringResult.push_back(_type);
		result.push_back(tempStringResult);
	}
	return result;
}

vector<string> convertToStringResults(vector<int> _intResults, string _type){
	vector<string> result;
	for(int i=0; i<_intResults.size(); i++){	
		string stringResult = std::to_string(static_cast<long long>( _intResults.at(i)));
		result.push_back(stringResult);
	}
	return result;
}

vector<string> evaluator::getAllPossibleByType(string _selectType){
	string nodeType;	
	if(_selectType == "assign"){
		nodeType = "assignNode";
		vector<int> getAll = ast->getAllStmtNumByType(nodeType);		 
		return convertToStringResults(getAll, _selectType);
	}else if(_selectType == "while"){
		vector<int> getAllWhile = ast->getAllStmtNumByType("whileNode");
		return convertToStringResults(getAllWhile, _selectType);
	}else if(_selectType == "stmt"){
		vector<int> getAll = ast->getAllStmtNumByType("assignNode");
		vector<int> getAll1 = ast->getAllStmtNumByType("whileNode");
		getAll.insert(getAll.end(), getAll1.begin(), getAll1.end());
		return convertToStringResults(getAll, _selectType);
	}else if(_selectType == "variable"){
		nodeType = "varNode";
		vector<vector<string>> varResult;
		return varTable->getAllVar();
		/*for(int i=0; i<getAll.size(); i++){
			vector<string> temp;
			temp.push_back(getAll.at(i));
			temp.push_back("variable");
			varResult.push_back(temp);
		}
		return varResult;*/
	}else if(_selectType == "constant"){
		vector<string> results = ast->getAllConstant();
		return results; 
	}else if(_selectType == "prog_line"){
		vector<string> results;
		return results; 
	}else if(_selectType == "procedure"){
		vector<string> results;
		return results; 
	}
}

vector<vector<string>> evaluator::convertSolution(vector<string> _solution){
	vector<vector<string>> returnsol;
	for(int i=0; i<_solution.size(); i++){
		vector<string> temp;
		temp.push_back(_solution.at(i));
		returnsol.push_back(temp);
	}
	return returnsol;
}