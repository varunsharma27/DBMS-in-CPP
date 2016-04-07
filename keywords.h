#ifndef __KEYWORDS_H							// INCLUDE GUARD, otherwise multiple definitions error
#define __KEYWORDS_H

#include<iostream>
#include<fstream>
#include<string>
#include"commonf.h"

using namespace std;

string db;
// FUNCTION PROTOTYPES BEGIN
void createTable(vector<string>&);
void createDatabase(vector<string>&);
void insert(vector<string>&);
bool useDatabase(string);
void deleteDatabase(string);
//FUNCTION PROTOTYPES END

/* CREATE TABLE */
void createTable(vector<string>& tok){
	if (db != ""){
		if (findTable(tok[0]) != string::npos){
			cout << endl << "Error: Table already exists" << endl;
			return;
		}
		if (tok[1] == "(" && checkType(tok[2])){

			string table = "<<t>>\n" + tok[0] + "\n" + tok[2] + "\n>>t<<\n";
			WriteDB(db, table, 0);
			cout << endl << "Table created" << endl;
		}
		else{
			cout << endl << "Wrong syntax or illegal datatypes" << endl;
		}
	}
	else {
		cout << endl << "No database selected" << endl;
	}
}

/* CREATE DATABASE */
void createDatabase(vector<string>& tok){
	if (fileExists(tok[0])) {
		cout << endl << "Error: Database already exists" << endl;
		return;
	}
	WriteDB(tok[0], "", 1);
	if (fileExists(tok[0]))
		cout << endl << "Database created" << endl;
}

/* INSERT INTO TABLE */
void insert(vector<string>& tok){
	if (db != ""){
		unsigned long loc = findTable(tok[0]);
		if (loc != string::npos){
			string attrib = getLine(loc + 1);
			/* CHECK TYPE CHECKS IF THE ENTERED VALUES MATCH THE TABLE DATATYPES */
			if (checkType(tok[3], getAttribTypes(attrib))){
				tok.erase(tok.begin());
				vectorToLower(tok, 0);
				if (match(tok[0], "values")){
					tok.erase(tok.begin(), tok.begin() + 2);
					WriteDB(db, tok[0], 0, loc + 2, 0);
					cout << endl << "Inserted" << endl;
				}
			}
			else{
				cout << endl << "Attribute types did not match" << endl;
			}
		}
		else{
			cout << endl << "Table not found" << endl;
		}
	}
	else{
		cout << endl << "No Database selected" << endl;
	}
}

/* USE DATABASE */
bool useDatabase(string usedb){
	if (fileExists(usedb)){
		db = usedb;
		return true;
	}
	return false;
}

/* DROP DATABASE */
void dropDatabase(string D){
	if (!fileExists(D)){
		cout << endl << "Database does not exist" << endl;
		return;
	}
	if (match(D,db)) db == "";
	if (deleteFile(D))
		cout << endl << "Database dropped" << endl;
	else
		cout << endl << "Error dropping database" << endl;
}

/* DROP TABLE */
void dropTable(string D){
	unsigned long start = findTable(D);
	if (start == string::npos){
		cout << endl << "Table does not exist" << endl;
		return;
	}
	start -= 1;
	vector<unsigned long> table = search(">>t<<", start, 0);
	unsigned long nolines = (table[0] - start) + 1;
	DeleteFromFile(db, 0, nolines, 0, start, 0);
	cout << endl << "Table deleted" << endl;

}

#endif
