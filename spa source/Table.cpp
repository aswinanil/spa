#include "Table.h"
#include <iostream>

typedef unordered_map<COLNAME, int> COLTOINDEXMAP;

Table::Table(){
	shrunkBefore = false;
	firstSinceShrink = true;
}

Table::~Table(){
	deleteTable();
}

void Table::checkRep(){
	//invariant 1: toKeep size == column size
	int toKeepSize = toKeep.size();
	for(int i = 0; i < table.size(); i++){
		if(table.at(i)->size() != toKeepSize){
			try{
				throw 1;
			}
			catch (int e){
				cout << "checkRep exception. toKeep size is not the same as column size." <<endl;
			}
		}
	}
}

bool Table::hasColumns(COLNAME colName){
	COLTOINDEXMAP::const_iterator hasCol = colToIndex.find(colName);
	if(hasCol == colToIndex.end())
		return false;
	else
		return true;
}

vector<VALUE> Table::getColumn(COLNAME colName){
	vector<VALUE> toReturn;
	COLTOINDEXMAP::const_iterator hasCol = colToIndex.find(colName);

	if(hasCol == colToIndex.end())	//column does not exist, return empty vector
		return toReturn;
	else{
		vector<VALUE>* colList = table.at(hasCol->second);
		for(vector<VALUE>::iterator it = colList->begin(); it != colList->end(); it++){
			toReturn.push_back(*it);
		}
		sort(toReturn.begin(), toReturn.end());

		vector<VALUE> col;
		for(vector<VALUE>::iterator it = toReturn.begin(); it != toReturn.end(); it++){
			if(col.size() == 0)
				col.push_back(*it);
			if(*it != col.back())
				col.push_back(*it);
		}

		return col;
	}
}

vector<VALUE> Table::getFullColumn(COLNAME colName){
	vector<VALUE> column;
	COLTOINDEXMAP::const_iterator hasCol = colToIndex.find(colName);

	if(hasCol == colToIndex.end()){
		return column;
	}
	else{
		vector<VALUE>* colList = table.at(hasCol->second);
		return *colList;
	}
}

void Table::insert(COLNAME colName, vector<VALUE> values){
	//check if colName exist
	bool hasCol = hasColumns(colName);
	checkRep();
	if(hasCol){
		//we simply go through and mark toKeep
		vector<VALUE> toMatch = getFullColumn(colName);
		for(int currRow = 0; currRow < toKeep.size(); currRow++){
			//if(firstSinceShrink)
			for(int i = 0; i < values.size(); i++){
				if(toMatch.at(currRow) != values.at(i)){
					toKeep.at(currRow) = false;
				}
				else{
					toKeep.at(currRow) = true;
					break;
				}
			}
		}
	}
	else{
		//we do cartesian product
		colToIndex.insert(COLTOINDEXMAP::value_type(colName, table.size()));

		vector<vector<VALUE>*> newTable;
		vector<bool> newToKeep;

		int numOfCols = table.size() + 1;
		for(int i = 0; i < numOfCols; i++){
			newTable.push_back(new vector<VALUE>());
		}

		if(table.size() == 0){
			for(int i = 0; i < values.size(); i++){
				newTable.at(0)->push_back(values.at(i));
				newToKeep.push_back(true);
			}
		}
		else{
			for(int currRow = 0; currRow < table.at(0)->size(); currRow++){
				for(int k = 0; k < values.size(); k++){
					newToKeep.push_back(true);
					int currCol;
					for(currCol = 0; currCol < table.size(); currCol++){
						newTable.at(currCol)->push_back(table.at(currCol)->at(currRow));
					}
					if(toKeep.at(currRow) == false){
						newTable.at(currCol)->push_back("-1");
						newToKeep.at(currRow) == false;
						break;
					}
					newTable.at(currCol)->push_back(values.at(k));
				}
			}
		}
		deleteTable();
		toKeep = newToKeep;
		table = newTable;
		checkRep();
	}
}

void Table::insert(COLNAME valueCol, VALUE val, COLNAME relatedCol, vector<VALUE> relatedSet){
	bool hasValueCol = hasColumns(valueCol);
	bool hasRelatedCol = hasColumns(relatedCol);

	if(hasValueCol){
		if(hasRelatedCol){
			insertWithTwoCols(valueCol, val, relatedCol, relatedSet);
			checkRep();
		}
		else{
			//insert relatedCol into unordered map
			insertWithNewValueCol(valueCol, val, relatedCol, relatedSet);
			checkRep();
		}
	}
	else{
		if(hasRelatedCol){
			//insert valueCol into unordered map
			insertWithNewSetCol(valueCol, val, relatedCol, relatedSet);
			checkRep();
		}
		else{
			//insert both stuff into unordered map
			insertWithNoCols(valueCol, val, relatedCol, relatedSet);	
			checkRep();
		}
	}
	firstSinceShrink = false;
}

bool match(VALUE first, VALUE second, VALUE val, vector<VALUE> relatedSet){
	if(first != val)
		return false;

	for(int i = 0; i < relatedSet.size(); i++){
		if(second == relatedSet.at(i))
			return true;
	}

	return false;
}

 void Table::insertWithTwoCols(COLNAME valueCol, VALUE val, COLNAME relatedCol, vector<VALUE> relatedSet){
	vector<vector<VALUE>*> newTable;
	vector<bool> newToKeep;

	vector<VALUE> first = getFullColumn(valueCol);
	vector<VALUE> second = getFullColumn(relatedCol);
	COLTOINDEXMAP::const_iterator  firstIt = colToIndex.find(valueCol);
	int firstIndex = firstIt->second;
	COLTOINDEXMAP::const_iterator  secondIt = colToIndex.find(relatedCol);
	int secondIndex = secondIt->second;

	//special case of simply appending
	if(shrunkBefore == false && table.size() == 2){
		for(int i = 0; i < relatedSet.size(); i++){
			table.at(0)->push_back(val);
			table.at(1)->push_back(relatedSet.at(i));
			toKeep.push_back(true);
		}
		return;
	}

	for(int i = 0; i < table.size(); i++)
		newTable.push_back(new vector<VALUE>());
	
	for(int currRow = 0; currRow < toKeep.size(); currRow++){
		if(first.at(currRow) == "-1" || second.at(currRow) == "-1"){
			//we match the one that is not "-1"
			bool found = false;
			if(first.at(currRow) == "-1"){
				//we match the second
				for(int i = 0; i < relatedSet.size(); i++){
					if(second.at(currRow) == relatedSet.at(i)){
						//we found a match, create new row
						found = true;
						newToKeep.push_back(true);
						for(int currCol = 0; currCol < table.size(); currCol++){
							if(currCol == firstIndex)
								newTable.at(currCol)->push_back(val);
							else
								newTable.at(currCol)->push_back(table.at(currCol)->at(currRow));
						}
					}
				}	
			}
			else{
				//we match the first
				if(first.at(currRow) == val){
					found = true;
					for(int i = 0; i < relatedSet.size(); i++){
						newToKeep.push_back(true);
						for(int currCol = 0; currCol < table.size(); currCol++){
							if(currCol == secondIndex)
								newTable.at(currCol)->push_back(relatedSet.at(i));
							else
								newTable.at(currCol)->push_back(table.at(currCol)->at(currRow));
						}
					}
				}
			}

			if(found == false){
				//copy as it is and invalidate this row
				newToKeep.push_back(false);
				for(int currCol = 0; currCol < table.size(); currCol++){
					newTable.at(currCol)->push_back(table.at(currCol)->at(currRow));
				}
			}
		}
		else{	//there's no "-1"
			//so we match the 2 pairs
			if(match(first.at(currRow), second.at(currRow), val, relatedSet)){
				//we found the match, so we copy as it is
				newToKeep.push_back(true);	
			}
			else{	
				//we didn't find a match, invalidate this row
				if(firstSinceShrink == true)
					newToKeep.push_back(false);
				else
					newToKeep.push_back(toKeep.at(currRow));
			}
			for(int currCol = 0; currCol < table.size(); currCol++){
				newTable.at(currCol)->push_back(table.at(currCol)->at(currRow));
			}
		}
	}

	toKeep = newToKeep;
	deleteTable();
	table = newTable;
}

void Table::insertWithNewValueCol(COLNAME valueCol, VALUE val, COLNAME relatedCol, vector<VALUE> relatedSet){
	colToIndex.insert(COLTOINDEXMAP::value_type(relatedCol, table.size()));
	
	vector<vector<VALUE>*> newTable;
	vector<bool> newToKeep;

	int numOfCols = table.size() + 1;
	for(int i = 0; i < numOfCols; i++)
		newTable.push_back(new vector<VALUE>());

	//get existing column matching valueCol
	vector<VALUE> toMatch = getFullColumn(valueCol);

	for(int currRow = 0; currRow < toMatch.size(); currRow++){
		if(toMatch.at(currRow) != val){
			//push existing values first
			int currColumn;
			for(currColumn = 0; currColumn < table.size(); currColumn++){				
				VALUE existValue = table.at(currColumn)->at(currRow);
				newTable.at(currColumn)->push_back(existValue);
			}
			//push back "-1" for new column
			newTable.at(currColumn)->push_back("-1");
			newToKeep.push_back(false);
			continue;
		}
		else{
			//push back existing item for each new item
			int currColumn;
			for(int currNew = 0; currNew < relatedSet.size(); currNew++){
				for(currColumn = 0; currColumn < table.size(); currColumn++){
					VALUE existValue = table.at(currColumn)->at(currRow);
					newTable.at(currColumn)->push_back(existValue);
				}
				newTable.at(currColumn)->push_back(relatedSet.at(currNew));
				if(toKeep.at(currRow) == true)
					newToKeep.push_back(true);
				else
					newToKeep.push_back(false);
			}
					
		}
	}
	deleteTable();
	table = newTable;
	toKeep = newToKeep;	
}

void Table::insertWithNewSetCol(COLNAME valueCol, VALUE val, COLNAME relatedCol, vector<VALUE> relatedSet){
	colToIndex.insert(COLTOINDEXMAP::value_type(valueCol, table.size()));

	vector<VALUE> toMatch = getFullColumn(relatedCol);
	table.push_back(new vector<VALUE>());
	vector<bool> newToKeep;

	for(int currRow = 0; currRow < toKeep.size(); currRow++){
		newToKeep.push_back(false);
		for(int i = 0; i < relatedSet.size(); i++){
			if(toMatch.at(currRow) == relatedSet.at(i)){
				newToKeep.at(currRow) = true;
				table.back()->push_back(val);
				break;
			}
		}
		if(newToKeep.at(currRow) == false)
			table.back()->push_back("-1");
		if(toKeep.at(currRow) == false)
			newToKeep.at(currRow) = false;
	}

	toKeep = newToKeep;
}

void Table::insertWithNoCols(COLNAME valueCol, VALUE val, COLNAME relatedCol, vector<VALUE> relatedSet){
	colToIndex.insert(COLTOINDEXMAP::value_type(valueCol, table.size()));
	colToIndex.insert(COLTOINDEXMAP::value_type(relatedCol, table.size()+1));
	
	vector<vector<VALUE>*> newTable;
	vector<bool> newToKeep;

	int numOfCols = table.size() + 2;
	for(int i = 0; i < numOfCols; i++){
		newTable.push_back(new vector<VALUE>());
	}

	if(table.size() == 0){
		for(int i = 0; i < relatedSet.size(); i++){
			newTable.at(0)->push_back(val);
			newTable.at(1)->push_back(relatedSet.at(i));
			newToKeep.push_back(true);
		}
	}
	else{//cartesian product
		for(int currRow = 0; currRow < table.at(0)->size(); currRow++){
			if(toKeep.at(currRow) == false)
				continue;
			for(int k = 0; k < relatedSet.size(); k++){
				newToKeep.push_back(true);	
				int currCol;
				for(currCol = 0; currCol < table.size(); currCol++){
					newTable.at(currCol)->push_back(table.at(currCol)->at(currRow));
				}
				newTable.at(currCol)->push_back(val);
				newTable.at(currCol+1)->push_back(relatedSet.at(k));
			}
		}
	}

	deleteTable();
	toKeep = newToKeep;
	table = newTable;

}


void Table::shrinkTable(){
	shrunkBefore = true;
	firstSinceShrink = true;

	vector<vector<VALUE>*> newTable;
	vector<bool> newToKeep;
	for(int cols = 0; cols < table.size(); cols++){
		newTable.push_back(new vector<VALUE>());
	}

	for(int currRow = 0; currRow < toKeep.size(); currRow++){
		if(toKeep.at(currRow)){
			for(int currCol = 0; currCol < table.size(); currCol++){
				newTable.at(currCol)->push_back(table.at(currCol)->at(currRow));
			}

			newToKeep.push_back(true);
		}
		
	}
	/*
	vector<bool> newToKeep;
	for(int i = 0; i < newTable.at(0)->size(); i++){
		newToKeep.push_back(true);
	}
	*/

	table = newTable;
	toKeep = newToKeep;
	checkRep();
}

void Table::deleteTable(){
	for(int i = 0; i < table.size(); i++){
		delete(table.at(i));
	}
}

void Table::print(){
	for(COLTOINDEXMAP::iterator it = colToIndex.begin(); it != colToIndex.end(); it++){
		cout << it->first << "\t";
	}
	cout << "toKeep" << endl;

	for(int currRow = 0; currRow < toKeep.size(); currRow++){
		for(int currCol = 0; currCol < table.size(); currCol++){
			cout << table.at(currCol)->at(currRow) << "\t";
		}
		cout << toKeep.at(currRow);
		cout <<endl;
	}

	cout << endl;
}