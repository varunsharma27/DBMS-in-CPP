#ifndef __COMMONF_H								// INCLUDE GUARD, otherwise multiple definitions error
#define __COMMONF_H

#include<iostream>
#include<string>
#include<vector>
#include<cstdio>					// included for remove(<filename>) function
#include<limits>					// Nothing fancy, just included to get maximum value of unsigned short type

using namespace std;

extern string db;
//FUNCTION PROTOTYPES
void extractTokens(string, vector<string>&);
void vectorToLower(vector<string>&, int);
void ToLower(string);
bool match(string, string);								// compares two strings
void cleanTokens(vector<string>&);
unsigned long findTable(string);						// returns line number of table name in DB file
void WriteDB(string, string, int);
bool checkType(string);
vector<string> extractData(string, int);
string getLine(unsigned long);
vector<string> getAttribTypes(string);
bool isFloat(string);
bool fileExists(string);
bool deleteFile(string);
void DeleteFromFile(string, unsigned long, unsigned long, int, unsigned long, unsigned long);
vector<unsigned long> search(string, unsigned long, unsigned long);
//vector<unsigned long> Where(string, string, string, string);
unsigned long attribNumber(string, string);
//TEMPLATES
template<class obj> bool inVect(obj check, vector<obj> vect);
template<class obj> vector<obj> addVect(vector<obj> vect);
template<class obj> bool inPad(obj index, obj pad, vector<obj> vect);
template<class obj> obj inColumn(obj index, vector<obj> vect);
//END OF TEMPLATES
//END OF PROTOTYPES

/* IF ANY UNSIGNED LONG RETURN TYPE FUNCTION IS UNSUCCESSFUL IN ITS JOB, IT RETURNS string::npos 
  IT IS EQUIVALENT TO A FALSE RETURN */

class date{
protected:
	unsigned __int8 dd, mm, yy;										// unsigned__int8 type uses only 1 byte. Range: 0 to 255
	unsigned short yyyy;											// Range: 0 to 65,535
public:
	date(){
		this->dd = 00;
		this->mm = 00;
		this->yy = 00;
		this->yyyy = 2000;
	}
	date(int d, int m, int y){
		if (d < 31 && m < 13 && y < 100){
			this->dd = d;
			this->mm = m;
			this->yy = y;
			if (y >= 40){
				this->yyyy = 1900 + y;
			}
			else if (y < 40){
				this->yyyy = 2000 + y;
			}
			correctDate(*this);
		}
		else
		if (d < 31 && m < 13 && y < USHRT_MAX){							// USHRT_MAX contains max value for unsigned short type
			this->dd = d;
			this->mm = m;
			this->yy = y % 100;
			this->yyyy = y;
			correctDate(*this);
		}
		else{
			correctDate(*this);											// If the size range is not satisfied, object is destroyed
		}
	}				// End of constructor
	/* Function which returns a vector of DD-MM-YYYY (or YY) from a string */
	vector<int> ddmmyyyy_fromString(string d){
		string temp;
		vector<int> datev;
		for (unsigned __int8 i = 0; i < d.length(); i++){
			if (d[i] != '-'){
				temp = temp + d[i];
			}
			if (d[i] == '-'){
				datev.push_back(stoi(temp));
				temp.clear();
			}
		}
		if (!match(temp,"")){
			datev.push_back(stoi(temp));
			temp.clear();
		}
		return datev;
	}

	date(const date& d){						// Simple copy constructor
		this->dd = d.dd;
		this->mm = d.mm;
		this->yy = d.yy;
		this->yyyy = d.yyyy;
	}
	date(const string& d){						// Copy constructor which initiates a date object from string input: "dd-mm-yy"
		if (d.length() < 11){					// or "dd-mm-yyyy"
			vector<int> datev = ddmmyyyy_fromString(d);
			this->dd = datev[0];
			this->mm = datev[1];
			if (datev[2] < 100){
				this->yy = datev[2];
				if (datev[2] >= 40)
					this->yyyy = datev[2] + 1900;
				else
					this->yyyy = datev[2] + 2000;
			}
			else{
				this->yyyy = datev[2];
				this->yy = datev[2] % 100;
			}
		}
	}

	void operator=(date d){
		this->dd = d.dd;
		this->mm = d.mm;
		this->yy = d.yy;
		this->yyyy = d.yyyy;
	}

	bool leap(date d){										// Checks if date year is LEAP YEAR or not
		if (d.yyyy % 4 == 0){
			if (d.yyyy % 100 == 0 && d.yyyy % 400 != 0){
				return false;
			}
			return true;
		}
		return false;
	}
	/* GET DAY MONTH YEAR */
	int getDay(){
		return this->dd;
	}
	int getMonth(){
		return this->mm;
	}
	int getYear(){
		return this->yyyy;
	}

	/* CORRECT DATE
	 Adds and subtracts days months and years to keep the date in limits*/
	void correctDate(date& d){
		date res;
		if (d.mm == 2){												// month: February
			if (d.dd > 28 && !leap(d)){								// day > 28, not leap
				res.dd = d.dd - 28;
				res.mm = d.mm + (d.dd / 28) + (d.dd / 59);
				res.yyyy = d.yyyy;
				res.yy = d.yy;
				if (res.mm > 12){
					res.mm = res.mm - 11;
					res.yyyy = res.yyyy + 1;
					res.yy = res.yy + 1;
				}
			}
			else if (d.dd > 29 && leap(d)){							// day > 29, leap
				res.dd = d.dd - 29;
				res.mm = d.mm + (d.dd / 29) + (d.dd / 60);
				res.yyyy = d.yyyy;
				res.yy = d.yy;
				if (res.mm > 12){
					res.mm = res.mm - 11;
					res.yyyy = res.yyyy + 1;
					res.yy = res.yy + 1;
				}
			}
			else {													// else day < 28
				res.dd = d.dd;
				res.mm = d.mm;
				res.yyyy = d.yyyy;
				res.yy = d.yy;
				if (res.mm > 12){
					res.mm = res.mm - 11;
					res.yyyy = res.yyyy + 1;
					res.yy = res.yy + 1;
				}
			}
		}
		/* months with 31 days */
		else if (d.mm == 1 || d.mm == 3 || d.mm == 5 || d.mm == 7 || d.mm == 8 || d.mm == 10 || d.mm == 12){
			if (d.dd > 31){
				res.dd = d.dd - 31;
				if (d.mm == 7)
					res.mm = d.mm + (d.dd / 31) + (d.dd / 62);
				else if (d.mm == 1)
					res.mm = d.mm + (d.dd / 31) + (d.dd / 59);
				else
					res.mm = d.mm + (d.dd / 31) + (d.dd / 61);
				res.yyyy = d.yyyy;
				res.yy = d.yy;
				if (res.mm > 12){
					res.mm = res.mm - 11;
					res.yyyy = res.yyyy + 1;
					res.yy = res.yy + 1;
				}
			}
			else{
				res.dd = d.dd;
				res.mm = d.mm;
				res.yyyy = d.yyyy;
				res.yy = d.yy;
				if (res.mm > 12){
					res.mm = res.mm - 11;
					res.yyyy = res.yyyy + 1;
					res.yy = res.yy + 1;
				}
			}
		}
		/* months with 30 days */
		else if (d.mm == 4 || d.mm == 6 || d.mm == 9 || d.mm == 11){
			if (d.dd > 30){
				res.dd = d.dd - 30;
				res.mm = d.mm + (d.dd / 30) + (d.dd / 61);
				res.yyyy = d.yyyy;
				res.yy = d.yy;
				if (res.mm > 12){
					res.mm = res.mm - 11;
					res.yyyy = res.yyyy + 1;
					res.yy = res.yy + 1;
				}
			}
			else{
				res.dd = d.dd;
				res.mm = d.mm;
				res.yyyy = d.yyyy;
				res.yy = d.yy;
				if (res.mm > 12){
					res.mm = res.mm - 11;
					res.yyyy = res.yyyy + 1;
					res.yy = res.yy + 1;
				}
			}
		}
		d = res;
	}
	date operator+(string d){										// Overloaded + operator to add dates
		date result;
		vector<int> datev = ddmmyyyy_fromString(d);
		result.dd = this->dd + datev[0];
		result.mm = this->mm + datev[1];
		if (datev[2] < 100)
			result.yy = this->yy + datev[2];
		else
			result.yyyy = this->yyyy + datev[2];
		correctDate(result);
		return result;
	}
	/* OVERLOADED > OPERATOR */
	bool operator>(date d){
		if (this->yyyy > d.yyyy) return true;
		else
			if (this->yyyy = d.yyyy){
			if (this->mm > d.mm) return true;
			else
				if (this->mm = d.mm){
				if (this->dd > d.dd) return true;
				}
			}
		return false;
	}
	/* OVERLOADED < OPERATOR */
	bool operator<(date d){
		if (this->yyyy < d.yyyy) return true;
		else
			if (this->yyyy = d.yyyy){
			if (this->mm < d.mm) return true;
			else
				if (this->mm = d.mm){
				if (this->dd < d.dd) return true;
				}
			}
		return false;
	}
	/* OVERLOADED == OPERATOR */
	bool operator==(date d){
		if (this->yyyy == d.yyyy && this->yy == d.yy && this->mm == d.mm && this->dd == d.dd) return true;
		return false;
	}
	// - operator

};

/* TOKEN EXTRACTOR
extracts endividual tokens from the command string
stores tokens in string vector
ORDER OF PRIORITY: "" > space */
void extractTokens(string cmd, vector<string> &tok){
	string temp;

	for (unsigned int i = 0; i < cmd.length(); i++){
	check:
		if (cmd[i] == '('){
			i++;
			if (!match(temp,"")){
				tok.push_back(temp);
				tok.push_back("(");
				temp.clear();
			}
			else{
				tok.push_back("(");
			}
			for (; i < cmd.length(); i++){
				if (cmd[i] != ')'){
					temp = temp + cmd[i];
				}
				if (cmd[i] == ')'){
					tok.push_back(temp);
					temp.clear();
					tok.push_back(")");
					i++;
					goto check;
				}
			}
		}
		if (cmd[i] == '"'){
			i++;
			if (!match(temp,"")){
				tok.push_back(temp);
				temp.clear();
			}
			for (; i < cmd.length(); i++){
				if (cmd[i] != '"'){
					temp = temp + cmd[i];
				}
				if (cmd[i] == '"'){
					tok.push_back(temp);
					temp.clear();
					i++;
					goto check;
				}
			}
		}
		if (cmd[i] != ' ' && cmd[i] != ',' && cmd[i] != '+' && cmd[i] != '-' && cmd[i] != '/' && cmd[i] != '*' && cmd[i] != '%' && cmd[i] != '='){
			temp = temp + cmd[i];
		}
		if (cmd[i] == ' ' || cmd[i] == ',' || cmd[i] == '+' || cmd[i] == '-' || cmd[i] == '/' || cmd[i] == '*' || cmd[i] == '%' || cmd[i] == '='){
			if (!match(temp, "")){
				tok.push_back(temp);
				temp.clear();
				if (cmd[i] == '+') tok.push_back("+");
				else
				if (cmd[i] == '-') tok.push_back("-");
				else
				if (cmd[i] == '/') tok.push_back("/");
				else
				if (cmd[i] == '*') tok.push_back("*");
				else
				if (cmd[i] == '%') tok.push_back("%");
				else
				if (cmd[i] == '=') tok.push_back("=");
			}
		}
	}
	if (!match(temp,"")){
		tok.push_back(temp);
		temp.clear();
	}
}

/* MATCH TWO STRINGS
Match Srings (string 1, string 2)*/
bool match(string M, string C){
	if (M.length() == C.length()){
		for (unsigned int i = 0; i < M.length(); i++){
			if (M[i] == C[i]){
				continue;
			}
			else{
				return false;
			}
		}
		return true;
	}
	else{
		return false;
	}
}

/* VECTOR TO LOWER
 CONVERT STRING AT GIVEN INDEX IN A STRING VECTOR TO LOWER CASE */
void vectorToLower(vector<string>& tok, int index){
	for (unsigned int i = 0; i < tok[index].length(); i++){
		if (tok[index][i] >= 65 && tok[index][i] <= 90){
			tok[index][i] += 32;
		}
	}
}

/* STRING TO LOWER
 CONVERT STRING TO LOWER CASE */
void ToLower(string s){
	for (unsigned int i = 0; i < s.length(); i++){
		if (s[i] >= 65 && s[i] <= 90){
			s[i] += 32;
		}
	}
}

/* TOKEN CLEANER
Cleans the tokens by removing empty strings*/
void cleanTokens(vector<string>& tok){
	for (unsigned int i = 0; i < tok.size(); i++){
		if (tok[i].empty()){
			tok.erase(tok.begin() + i);
		}
	}
}

/* TABLE FINDER 
Searches for a table in the current database and returns the line number */
unsigned long findTable(string table){
	unsigned long loc = 0;											// stores line number 
	string lineBuffer;
	fstream findT;
	findT.open(db, fstream::in);
	while (getline(findT, lineBuffer)){
		loc++;
		if (lineBuffer.find("<<t>>") != string::npos){				
			if (getline(findT, lineBuffer)){
				loc++;
				if (lineBuffer.find(table) != string::npos){
					return loc-1;										// loc - 1 because line 1 is 1 and needs to be 0
				}
			}
		}
	}
	findT.close();
	return string::npos;
}

/* WRITE TO FILE
 WRITEDB (FILE_NAME, DATA, MODE) */
void WriteDB(string F, string S, int mode){
	ofstream file;
	if (mode == 0){											// mode 0 is used to append
		file.open(F, ios::app);								// Adding ios::out causes problems
	}
	else if (mode == 1){									// mode 1 is used to delete all previous
		file.open(F, ios::trunc);							// data in the file and write new
	}
	file << S;
	file.close();
}
/* WRITE TO FILE OVERLOADED (FILE_NAME, DATA, MODE, LINE_NUMBER, CHARACTER_NUMBER) 
 Works by reading the contents of the database file, storing them in a file Buffer (string vector),
  making changes to the file buffer, deleting the old contents of the file, writing the new contents to the file*/
void WriteDB(string F, string S, int mode, unsigned long line, unsigned long char_num){
	ifstream ifile;
	ofstream ofile;
	vector<string> fileBuffer;
	string lineBuffer;
	ifile.open(F);
	while (getline(ifile, lineBuffer)){
		fileBuffer.push_back(lineBuffer);
	}
	/* MODE 0 inserts data by creating a new line at the given line number*/
	if (mode == 0){
		fileBuffer.insert(fileBuffer.begin() + line, S);
	}
	/* MODE 1 inserts data in an existing line at the specified location*/
	if (mode == 1){	
		string::size_type pos = char_num;
		fileBuffer[line].insert(pos, S);
	}
	ifile.close();
	ofile.open(F, ios::trunc);
	ofile << "";
	ofile.close();
	ofile.open(F);
	for (vector<string>::size_type i = 0; i < fileBuffer.size(); i++){
		ofile << fileBuffer[i] << "\n";
	}
	ofile.close();

}

/* CHECK TYPE 
Two variants of this function: one checks for table attributes and types, 
the other checks for tuple values against a given string vector containing datatypes (list)*/
bool checkType(string T, vector<string> type){
	vector<string> tok = extractData(T, 0);
	if (type.size() != tok.size()) return false;
	for (unsigned int i = 0; i < tok.size(); i++){
		ToLower(type[i]);
		if (match(type[i], "varchar") || match(type[i], "date")){
			if (match(type[i], "date")){
				date d = tok[i];
				string s1 = to_string(d.getDay()) + "-" + to_string(d.getMonth()) + "-" + to_string(d.getYear());
				string s2 = to_string(d.getDay()) + "-" + to_string(d.getMonth()) + "-" + to_string(d.getYear() % 100);
				if (match(s1, tok[i]) || match(s2, tok[i])){
					continue;
				}
				else return false;
			}
			else if (match(type[i], "varchar")){
				continue;
			}
			else return false;
		}
		else 
		if (match(type[i], "number")){
			int d = stoi(tok[i]);
			string c = to_string(d);
			if (match(c, tok[i])){
				continue;
			}
			else return false;
		}
		else 
		if (match(type[i], "float")){
			if (isFloat(tok[i])){
				continue;
			}
			else return false;
		}
	}
	return true;										
}

/* OVERLOADED CHECKTYPE 
CHECKS IF THE STRING IS OF TYPE: <attrib_name> <data_type>, <attrib_name> <data_type>,.....
WHERE DATATYPES ARE: NUMBER, FLOAT, DATE, VARCHAR */
bool checkType(string T){
	vector<string> tok = extractData(T, 1);
	cleanTokens(tok);
	for (unsigned int i = 1; i < tok.size(); i=i+2){
		ToLower(tok[i]);
		if (match(tok[i], "number") || match(tok[i], "float") || match(tok[i], "date") || match(tok[i], "varchar")){
			continue;
		}
		else return false;
	}
	return true;									
}

/* DATA EXTRACTOR
extracts data from a tuple or table attribute list if mode is set to 0, it takes only 1 string between two commas
if it is set to 1, then it just returns a vector of strings which were separated by ' ' or ',' */
vector<string> extractData(string d, int mode){
	string temp;
	vector<string> tok;
	if (mode == 0){
		bool comma = true;
		bool space = true;
		for (unsigned int i = 0; i < d.length(); i++){
			if (d[i] == '"' && (comma || space)){
				i++;
				if (!match(temp,"")){
					tok.push_back(temp);
					temp.clear();
				}
				for (; i < d.length(); i++){
					if (d[i] != '"'){
						temp = temp + d[i];
					}
					if (d[i] == '"'){
						tok.push_back(temp);
						temp.clear();
						i++;
						comma = false;
						space = false;
						break;
					}
				}
			}
			if (comma || space){
				if (d[i] != ',' && d[i] != ' '){
					temp = temp + d[i];
					space = false;
					comma = true;
				}
				if (d[i] == ',' || d[i] == ' '){
					if (!match(temp, "")){
						tok.push_back(temp);
						temp.clear();
					}
					if (d[i] == ',')
						space = true;
					if (d[i] == ' ')
						comma = false;
				}
			}
			if (!(comma || space) && d[i] == ','){
				comma = true;
				space = true;
			}
		}
		if (!match(temp,"")){
			tok.push_back(temp);
			temp.clear();
		}
	}
	/*----------- MODE 1 STARTS------------*/
	else if (mode == 1){
		for (unsigned int i = 0; i < d.length(); i++){
				if (d[i] != ',' && d[i] != ' '){
					temp = temp + d[i];
				}
				if (d[i] == ',' || d[i] == ' '){
					tok.push_back(temp);
					temp.clear();
				}
		}
		if (!match(temp, "")){
			tok.push_back(temp);
			temp.clear();
		}
	}
	return tok;
}

/* GET LINE FROM FILE
 GET LINE (LINE NUMBER OF LINE) 
 IT FETCHES THE LINE FROM THE SELECTED DATABASE */
string getLine(unsigned long line){
	ifstream ifile;
	string lineBuffer;
	ifile.open(db);
	unsigned long i = 0;
	while (i<=line){
		getline(ifile, lineBuffer);
		i++;
	}
	return lineBuffer;
}

/* GET ATTRIBUTE TYPES
EXTRACT DATA TYPES FROM A STRING*/
vector<string> getAttribTypes(string S){
	vector<string> tok = extractData(S, 1);
	cleanTokens(tok);
	for (unsigned int i = 0; i < tok.size(); i++){
		tok.erase(tok.begin() + i);
	}
	return tok;
}

/* IS FLOAT 
 CHECKS IF STRING IS FLOAT */
bool isFloat(string F){
	if (F.length() > 20) return false;					// If legth of string is > 20, it is safe to assume that it is not a float
	int dec = 0, bef = 0, aft = 0;
	if ((F[0] >= 48 && F[0] <= 57) || F[0] == 43 || F[0] == 45 || F[0] == 46){
		if (F[0] == 46) dec++;
		if (F[0] >= 48 && F[0] <= 57) bef++;
		for (unsigned int i = 0; i < F.length(); i++){
			if ((F[i] >= 48 && F[i] <= 57) && dec == 0) bef++;
			else
				if ((F[i] >= 48 && F[i] <= 57) && dec != 0) aft++;
				else
					if (F[i] == 46) dec++;
					else
						if (dec > 1) return false;
						else return false;
		}
		return true;
	}
	return false;
}

/* CHECK IF FILE EXISTS */
bool fileExists(string F){
	ifstream ifile(F);
	return ifile.good();
}

/* DELETE GIVEN FILE */
bool deleteFile(string F){
	if (remove(F.c_str()) == 0){
		return true;
	}
	else return false;
}

/* DELETE FROM FILE (FILE_NAME, LENGTH, NUMBER_OF_LINES, MODE, START_LINE_NUMBER, START_CHARACTER_NUMBER)
Works by reading the contents of the database file, storing them in a file Buffer (string vector),
making changes to the file buffer, deleting the old contents of the file, writing the new contents to the file*/
void DeleteFromFile(string F, unsigned long char_len, unsigned long no_lines, int mode, unsigned long line, unsigned long char_num){
	ifstream ifile;
	ofstream ofile;
	vector<string> fileBuffer;
	string lineBuffer;
	ifile.open(F);
	while (getline(ifile, lineBuffer)){
		fileBuffer.push_back(lineBuffer);
	}
	/* MODE 0 deletes data by deleting given number of lines from given starting point*/
	if (mode == 0){
		fileBuffer.erase(fileBuffer.begin() + line, fileBuffer.begin() + (line + no_lines));
	}
	/* MODE 1 deletes data within a line at the specified location */
	if (mode == 1){
		string::size_type pos = char_num;
		fileBuffer[line].erase(pos, char_len);
	}
	ifile.close();
	ofile.open(F, ios::trunc);
	ofile << "";
	ofile.close();
	ofile.open(F);
	for (vector<string>::size_type i = 0; i < fileBuffer.size(); i++){
		ofile << fileBuffer[i] << "\n";
	}
	ofile.close();

}

/* SEARCH 
 SEARCHES FOR GIVEN STRING IN CURRENT DB STARTING FROM THE GIVEN CHARACTER
 RETURNS AN UNSIGNED LONG VECTOR WHERE FIRST ELEMENT IS LINE NUMBER AND SECOND ELEMENT IS CHARACTER NUMBER
 IF THE ELEMENT IS NOT FOUND, IT RETURNS string::npos */
vector<unsigned long> search(string stext, unsigned long start_line, unsigned long start_char){
	unsigned long line = start_line, char_num = start_char;
	vector<unsigned long> ret_vals;
	ifstream ifile;
	vector<string> fileBuffer;
	string lineBuffer;
	ifile.open(db);
	while (getline(ifile, lineBuffer)){
		fileBuffer.push_back(lineBuffer);
	}
	for (; line < fileBuffer.size(); line++){
		char_num = fileBuffer[line].find(stext);
		if (char_num != string::npos){
			ret_vals.push_back(line);
			ret_vals.push_back(char_num);
			return ret_vals;
		}
	}
	ret_vals.push_back(string::npos);
	return ret_vals;
}

unsigned long attribNumber(string attribName, string tableName){
	unsigned long line = findTable(tableName);
	string att_string = getLine(line + 1);
	vector<string> attrib = extractData(att_string, 0);
	for (unsigned long i = 0; i < attrib.size(); ++i){					// i'll avoid using i++ in standalone statements as it is slower
		if (match(attrib[i], attribName)) return i;
	}
	return string::npos;
}

string get_val_at(unsigned long row, unsigned long attrib_num){
	/* if for some reason the row isn't fetched and it tries to return an element of the vector that does not exist 
		this try catch will prevent Assertion error */
	try{
		string row_string = getLine(row);
		vector<string> row_vect = extractData(row_string, 0);
		return row_vect[attrib_num];
	}
	catch (exception e){
		return "~~**~~";
	}
}

template<class obj>
bool inVect(obj check, vector<obj> vect){
	for (unsigned long i = 0; i < vect.size(); ++i){
		if (check == vect[i]) return true;
	}
	return false;
}

template<class obj>
vector<obj> addVect(vector<obj> vect){
	vect[0] = vect[0] + 1;
	for (unsigned long i = 1; i < vect.size(); ++i){
		vect[i] = vect[i] + vect[i - 1] + 1;
	}
	return vect;
}

template<class obj>
bool inPad(obj index, obj pad, vector<obj> vect){
	if (inVect(index, vect)) return false;
	if (index > 0 && index <= pad) return true;
	if (index < vect[vect.size() - 1] && index >(vect[vect.size() - 1] - pad)) return true;
	for (unsigned long i = 0; i < vect.size(); ++i){
		if (index < vect[i] && index >= (vect[i] - pad)) return true;
		if (index > vect[i] && index <= (vect[i] + pad)) return true;
	}
	return false;
}

template<class obj>
obj inColumn(obj index, vector<obj> vect){
	unsigned long column = 0;
	for (unsigned long i = 0; i < vect.size(); ++i){
		if (index < vect[i]) return i;
	}
	return string::npos;
}

/* PRINT TABLE ( VECTOR2 < VECTOR1 <STRING> > > )
 TAKES VECTOR OF STRING VECTOR AS ARGUMENT
 EACH STRING IN VECTOR 1 REPRESENTS AN ELEMENT OF EACH COLUMN
 EACH VECTOR1 IN VECTOR2 REPRESENTS A ROW
 TABLE[rows][columns]*/
void printTable(vector<vector<string>> table){
	unsigned long pad = 3, max_width = 34;				// 3 padding left, 3 padding right, max width 34, Total column width = 40
	unsigned long total_width = 1;
	bool elementPrinted = false;
	vector<unsigned long> width;
	for (unsigned int i = 0; i < table[0].size(); ++i){
		width.push_back(0);
	}
	for (unsigned int i = 0; i < table[0].size(); ++i){
		for (unsigned int j = 0; j < table.size(); ++j){
			if (width[i] < table[j][i].length()) width[i] = table[j][i].length();
			if (width[i] >= max_width){				// If a particular string is wider than max_width, limit column to max_width
				width[i] = max_width;
				break;
			}
		}
		width[i] = width[i] + 2 * pad;
		total_width = total_width + width[i] + 1;
	}
	
	vector<unsigned long> width_index = addVect(width);
	//start printing loop
	for (unsigned long i = 0; i < table.size(); ++i){
		cout << endl;
		// FIRST ROW
		if (i == 0){
			// TOP LINE
			for (unsigned long j = 0; j < total_width; ++j){
				cout << "_";
			}
			cout << endl;
			// COLUMN NAMES
			for (unsigned long j = 0, k = 0; j < total_width; ++j){
				if (j == 0 || inVect(j, width_index)){
					cout << "|";
					elementPrinted = false;
					continue;
				}
				else
				if (inPad(j, pad, width_index) || elementPrinted){
					cout << " ";
					continue;
				}
				else{
					unsigned long column = inColumn(j, width_index);
					for (; k < table[i][column].length() && k < (width[column] - 2*pad); ++k){
						cout << table[i][column][k];
					}
					j = j + k - 1;
					elementPrinted = true;
					k = 0;
				}
			}
			cout << endl;
			// LOWER LINE
			for (unsigned long j = 0; j < total_width; ++j){
				cout << "-";				
			}
			cout << endl;
		}
		else{
			for (unsigned long j = 0, k = 0; j < total_width; ++j){
				if (j == 0 || inVect(j, width_index)){
					cout << "|";
					elementPrinted = false;
					continue;
				}
				else
					if (inPad(j, pad, width_index) || elementPrinted){
					cout << " ";
					continue;
					}
					else{
						unsigned long column = inColumn(j, width_index);
						for (; k < table[i][column].length() && k < (width[column] - 2 * pad); ++k){
							cout << table[i][column][k];
						}
						j = j + k - 1;
						elementPrinted = true;
						k = 0;
					}
			}
			cout << endl;
		}
		if (i == (table.size() - 1)){
			for (unsigned long j = 0; j < total_width; ++j){
				cout << "-";				
			}
			cout << endl;
		}
	}
}

// not sure about the parameters... need to change
//vector<unsigned long> Where(string table, vector<string> conditions, string, string){
//	vector<unsigned long> ret_vals;
	/*
	NOT COMPLETED YET!!! UNDER CONSTRUCTION
	*/
//}


#endif
