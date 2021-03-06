/*
The class Table stores a representation of query clauses.
It contains the values that satisfies the clauses.

Typically, a clause will check if the Table contains the columns using the hasColumns() method.
If it exist, it will get the Vector of possible values using the getColumn() method.
After evaluating and retrieving the values from the PKB, these values will be inserted using the insert() method.
After evaluating a clause, the shrinkTable() must be called.
The table will be returned to the main evaluator for merging, by calling the merge() method.
*/

#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <unordered_map>

using namespace std;

typedef string COLNAME;
typedef string VALUE;

class Table{
public:
	//Constructor, Destructor
	Table();
	~Table();

	//hasColumn() returns TRUE if COLNAME exists in the table, and FALSE otherwise.
	bool hasColumns(COLNAME);

	/*
	getColumn() returns a vector of values that are under the column COLNAME.
	If such a column does not exist, an empty vector will be returned.

	This method should be called only after shrinking table, otherwise the values might not be valid. Eg. "-1"
	*/
	vector<VALUE> getColumn(COLNAME);

	/*
	insert() inserts the VALUE with all the corresponding VALUES (eg. Follows(1,2), Follows(1,3)
	will call insert of value 1, and a vector containing {2,3}.
	insert() will ensure that the Table does not hold any duplicated entries.

	insert(COLNAME valueCol, VALUE val, COLNAME relatedCol, vector<VALUE> relatedColSet)

	4 cases:
	1) valueCol exist, relatedCol does not exist. (can only happen after at least 1 clause has been evaluated, therefore there must be at least 2 columns)
		-insert new column called relatedCol.
		-we do cartesian product of (valCol, s2, ...) x (relatedCol)
	
	2) valueCol does not exist, relatedCol exist. (Follows(s1, s2), Parent(s3, s2)
		-insert valueCol

	3) both valueCol and relatedCol does not exist. (can happen for brand new table, or for a clause with brand new variables)
		-insert both columns
		-if it was a brand new table
			-just insert happily
		-else
			-we do cartesian product of (s1, s2, ...) x (s3, s4)

	4) both valueCol and relatedCol exist. (can happen for a new clause that uses the same variable as a previous clause eg. Follows(s1, s2) Parent(s1, s2))
		-we do intersection of (s1, s2, ...) AND (s1, s2)
		-i.e we get to shrink table

	Properties:
	1) It is never possible to have only 1 column exist.
		We always insert in related pairs of numbers.

	
	Properties on End Result of table:
	1) if there exist an entry fully filled (ie the entire row), then the query is TRUE.
	2) To get the possible values (eg. s1 in select s1 ...), then it is the set of column of s1, provided the row it is in is all filled.
	3) The table can only contain missing values while the clause is being evaluated.
	*/
	void insert(COLNAME, VALUE, COLNAME, vector<VALUE>);
	void insert(COLNAME, vector<VALUE>);

	/*
	after inserting ALL possible combinations for that 1 single clause, there might
	be some leftover entries in the table that did not satisfy the new clause.
	These entries can be removed to shrink the table size.
	It is important that this method be called only after evaluating the WHOLE clause.
	*/
	void shrinkTable();

	/*
	mergeTable() will merge the this.table and Table.table. 
	The new table will be returned, and no modifications will be made to the parameters.
	*/
	Table mergeTable(Table*);

	void print();

private:
	void deleteTable();
	void insertWithConstant(COLNAME, vector<VALUE>);
	void insertWithTwoCols(COLNAME, VALUE, COLNAME, vector<VALUE>);
	void insertWithNewValueCol(COLNAME, VALUE, COLNAME, vector<VALUE>);
	void insertWithNewSetCol(COLNAME, VALUE, COLNAME, vector<VALUE>);
	void insertWithNoCols(COLNAME, VALUE, COLNAME, vector<VALUE>);
	vector<VALUE> getFullColumn(COLNAME);
	void checkRep();

	vector<vector<VALUE>*> table;
	unordered_map<COLNAME, int> colToIndex;
	vector<bool> toKeep;
	bool shrunkBefore;
	bool firstSinceShrink;
};
