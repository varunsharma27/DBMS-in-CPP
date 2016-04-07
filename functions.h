#ifndef __FUNCTIONS_H							// INCLUDE GUARD, otherwise multiple definitions error
#define __FUNCTIONS_H

#include<iostream>
#include<string>
#include<vector>
#include"keywords.h"
#include"commonf.h"

using namespace std;

//Function PROTOTYPES
void interpret(string);
void listen();
//End of Function PROTOTYPES

/* LISTENER
Listens for SQL commands*/
void listen(){
	string command;
	cout << "\n[SQL]: ";
	getline(cin, command, '\n');
	interpret(command);
}

/* INTERPRETER
interprets the meaning and functions of a command*/
void interpret(string cmd){
	vector<string> token;
	// Extract Tokens
	if (match(cmd, "")) return;								// Otherwise assertion error
	extractTokens(cmd, token);
	// Clean Tokens
	cleanTokens(token);										//to clean the tokens and remove empty strings
	vectorToLower(token, 0);

	if (match(token[0],"create")){							// match for CREATE
		token.erase(token.begin());
		vectorToLower(token, 0);
		if (match(token[0], "table")){						// match for create -> TABLE
			token.erase(token.begin());
			createTable(token);
		}
		else if (match(token[0], "database")){				// match for create -> DATABASE
			token.erase(token.begin()+0);
			createDatabase(token);
		}
		else{
			cout << endl << "Error: illegal create command" << endl;
		}
	}
	else 
		if (match(token[0], "use")){						// match for USE
			if (useDatabase(token[1]))
				cout << endl << "Database selected" << endl;
			else
				cout << endl << "Database not found" << endl;
		}
	else
		if (match(token[0], "exit")){						// match for EXIT
			exit(0);
		}
	else 
		if (match(token[0], "insert")){						// match for INSERT
			if (match(token[1], "into")){
				token.erase(token.begin(), token.begin()+2);
				insert(token);
			}
		}
	else
		if (match(token[0], "drop")){						// match for DROP
			token.erase(token.begin());
			vectorToLower(token, 0);
			if (match(token[0], "database")){				// match for drop -> DATABASE
				dropDatabase(token[1]);
			}
			else
			if (match(token[0], "table")){					// match for drop -> TABLE
				dropTable(token[1]);
			}
			else
				cout << endl << "Error: Illegal drop command" << endl;
		}
	else
		if (match(token[0], "print")){
		vector<vector<string>> p;
		for (unsigned long i = 0; i < 6; ++i){
			p.push_back(token);
		}
		printTable(p);
		}
	else cout << endl << "No command matched" << endl;

}

#endif
