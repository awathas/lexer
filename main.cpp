#include <iostream>
#include <iomanip>
#include <string>
#include <map>

using namespace std;

class Lex {
private:

	int states[42][26] = {

		//                     ti  le  d   ,   ;   <   >   {   }   [   ]   (   )   *   ^   :   .   =   -   +   /   "   !   WS  NA  BackUp 

		/* 0  LOOKAHEAD    */{ -1, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1 },

		/* 1  START        */{ 1,  2,  4,  10, 11, 12, 14, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28, 30, 31, 8,  41, 1,  39, -1 },

		/* 2    IN ID      */{ -1, 2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  -1 },

		/* 3  END of ID    */{ 2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },

		/* 4    IN INT     */{ -1, 5,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  0,  5,  5,  5,  5,  5,  5,  5,  5,  -1 },

		/* 5  END INT      */{ 3,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },

		/* 6    IN FLOAT   */{ -1, 7,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  -1 },

		/* 7  END FLOAT    */{ 4,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },

		/* 8    IN STRING  */{ -1, 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  9,  8,  8,  8,  -1 },

		/* 9  END STRING   */{ 5,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 10 TOKEN ,      */{ 6,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 11 TOKEN ;      */{ 7,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 12   FOUND <    */{ -1, 13, 13, 13, 13, 37, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 35, 13, 13, 13, 13, 13, 13, 13, -1 },

		/* 13 TOKEN <      */{ 31, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },

		/* 14   FOUND >    */{ -1, 15, 15, 15, 15, 15, 38, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 36, 15, 15, 15, 15, 15, 15, 15, -1 },

		/* 15 TOKEN >      */{ 32, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },

		/* 16 TOKEN {      */{ 33, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 17 TOKEN }      */{ 34, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 18 TOKEN [      */{ 35, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 19 TOKEN ]      */{ 36, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 20 TOKEN (      */{ 37, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 21 TOKEN )      */{ 38, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 22 TOKEN *      */{ 41, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 23 TOKEN ^      */{ 42, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 24 TOKEN :      */{ 43, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 25 TOKEN .      */{ 44, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 26   FOUND =    */{ -1, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 33, 27, 27, 27, 27, 27, 27, 27, -1 },

		/* 27 TOKEN =      */{ 45, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },

		/* 28   FOUND -    */{ -1, 29, 4,  29, 29, 29, 32, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, -1 },

		/* 29 TOKEN -      */{ 46, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },

		/* 30 TOKEN +      */{ 47, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 31 TOKEN /      */{ 48, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 32 TOKEN ->     */{ 51, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 33 TOKEN ==     */{ 52, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 34 TOKEN !=     */{ 53, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 35 TOKEN <=     */{ 54, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 36 TOKEN >=     */{ 55, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 37 TOKEN <<     */{ 56, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 38 TOKEN >>     */{ 57, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 39 ERROR        */{ 99, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 40 EOF          */{ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 },

		/* 41 !            */{ -1, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 34, 39, 39, 39, 39, 39, 39, 39, 0 }

	};


	std::map<std::string, int> keyword = {

		{ "while", 1}, {"int", 2}, {"if", 3}, {"fi", 4}, {"else", 5},
		{"return", 6}, {"read", 7}, {"read", 8}, {"write", 9}, {"boolean", 10},
		{"float", 11}, {"true", 12}, {"false", 13}, {"string", 14}, {"char", 15}

	};

public:
	int getColumn(char c);
	int getState(int state, int column);
	bool acceptingState(int state);
	bool needsBackup(int state);
	int getTok_id(int state);
	int getKeywordID(string lexeme);
	bool isKeyword(string lexeme);
};



struct Token {
	int start = -1;
	int tok_id;
	string lexeme;

	Token() {
		start = -1;
		tok_id = -1;
		lexeme = "";
	}

	void print() {
		if (tok_id == 5) {

			lexeme = lexeme.substr(1, lexeme.length() - 2);

		}

		string additionalOutput = "";

		if (tok_id == 3) {

			additionalOutput += " int= " + lexeme;

		}

		if (tok_id == 4) {

			additionalOutput += " float= " + lexeme;

		}

		cout << "(Tok: id= " << right << setw(2) << to_string(tok_id) <<

			" str= \"" + lexeme + "\"" << additionalOutput << ")" << endl;
	}

};

int main() {
	Lex lex;
	Token token;
	
	string input = "";

	while (getline(cin, input)) {
		

		char currentChar = '\0';
		char nextChar = '\0';
		int column = -1;
		int state = 1;
		int previousState = 1;
		bool alreadyBackedUp = false;
		bool comment = false;

		//tokenizing inputs

		for (int i = 0; i < input.length(); i++)
		{
			//get characters one by one and set current chars and next chars
			currentChar = input[i];
			nextChar = input[i + 1];

			if (currentChar == '/' && nextChar == '/') {
				comment = true;
				currentChar = ' ';
			}

			//translate character into comlumn index
			column = lex.getColumn(currentChar);

			if (token.start == -1) {
				// if the current charactor is not white space
				if (column != 23) {
					// set token_start to current character pointer
					token.start = i;
				}
			}

			previousState = state;
			//find new state from table
			state = lex.getState(state, column);

			if (state == 0) {
				if (currentChar == '.') {
					if (isdigit(nextChar))
						state = 6;
					else
						state = 5;

				}
				if (lex.acceptingState(state)) {
					i--;
					alreadyBackedUp = true;
				}
			}

			//check if token has been found
			//aka checking for accepting states
			if (lex.acceptingState(state)) {
				//backup
				if (lex.needsBackup(state) && !alreadyBackedUp) {
					i--;
				}
				alreadyBackedUp = false;
			}

			token.lexeme = input.substr(token.start, i - token.start + 1);
			token.start = -1;
			token.tok_id = lex.getTok_id(state);

			//if token is identifier
			if (token.tok_id == 2) {
				//if identifer is a keyword
				if (lex.isKeyword(token.lexeme)) {
					//set token to keyword
					token.tok_id = lex.getKeywordID(token.lexeme);
				}
			}

			token.print();
			state = 1;

			if (comment) {
				break;
			}
		}
	}
}

int Lex::getColumn(char c) {
	if (isalpha(c) || c == '_' ) 
		return 1;
	if (isdigit(c))
		return 2;
	if (isspace(c))
		return 3;

	switch (c) {
	case ',':
		return 3;
		break;
	case ';':
		return 4;
		break;
	case '<':
		return 5;
		break;
	case '>':
		return 6;
		break;
	case '{':
		return 7;
		break;
	case '}':
		return 8;
		break;
	case '[':
		return 9;
		break;
	case ']':
		return 10;
		break;
	case '(':
		return 11;
		break;
	case ')':
		return 12;
		break;
	case '*':
		return 13;
		break;
	case '^':
		return 14;
		break;
	case ':':
		return 15;
		break;
	case '.':
		return 16;
		break;
	case '=':
		return 17;
		break;
	case '-':
		return 18;
		break;
	case '+':
		return 19;
		break;
	case '/':
		return 20;
		break;
	case '"':
		return 21;
		break;
	case '!':
		return 22;
		break;
	default:
		return 24;
	}

	return 39;		
	
}

int Lex::getState(int state, int column) {
	return states[state][column];
}

bool Lex::needsBackup(int state) {
	if (states[state][25] == 1)
		return true;
	else
		return false;
}



bool Lex::acceptingState(int state) {
	if (states[state][25] != -1)
		return true;
	else
		return false;
}

int Lex::getTok_id(int state) {
	return states[state][0];
}

int Lex::getKeywordID(string lexeme) {
	if (keyword[lexeme] != 0)
		return keyword[lexeme];
	return 3;
}

bool Lex::isKeyword(string lexeme) {
	if (keyword[lexeme] != 0)
		return true;
	else
		return false;
}
