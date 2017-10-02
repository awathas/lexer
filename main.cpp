#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <fstream>

using namespace std;

class Lex {

private:
	//FSM States Chart
	int states[42][26] = {

		//                     ti  le  d   ,   ;   <   >   {   }   [   ]   (   )   *   #   :   .   =   -   +   /   "   !   WS  NA  BackUp 

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

	//Map of all known keywords
	std::map<std::string, int> keyword = {
		{ "bool", 10 },{ "read", 11 },{ "@", 12 },{ "char", 13 },{ "float", 15 },
		{ "string", 16 },{ "integer", 17 },{ "if", 18 },{ "fi", 19 },{ "else", 20 },
		{ "while", 21 },{ "write", 22 },{ "False", 23 },{ "True", 24 },{ "return", 25 }
	};


	//Functions list
public:
	bool isKeyword(string lexeme);
	bool isAcceptingState(int state);
	bool needsBackup(int state);
	int getKeywordId(string lexeme);
	int getColumnOf(char c);
	int getTokenId(int state);
	int getNewState(int state, int column);
	int lookAhead(int previous_state, char current_char, char next_char);
};


//Token object
struct Token {
	int start = -1;
	int id;
	string lexeme;

	Token() {
		start = -1;
		id = -1;
		lexeme = "";
	}

	//Print Function
	void print() {
		if (id == 5) 
		{
			lexeme = lexeme.substr(1, lexeme.length() - 2);
		}

		//Prints keywords
		if (id <= 25 && id >= 10)
		{
			cout << "(Token: keyword" << right << setw(2) << " str= \"" + lexeme + "\"" << ")" << endl;
		}
		
		//Prints identifiers
		else if (id == 2)
		{
			cout << "(Token: identifier" << right << setw(2) << " str = \"" + lexeme + "\"" << ")" << endl;
		}

		//Prints Seperators
		else if ((id >= 6 && id < 8) || (id >= 33 && id < 39) || (id >= 43 && id < 45))
		{
			cout << "(Token: seperator" << right << setw(2) << " str = \"" + lexeme + "\"" << ")" << endl;
		}
		
		//Prints Integers
		else if (id == 3)
		{
			cout << "(Token: integer" << right << setw(2) << " str = \"" + lexeme + "\"" << ")" << endl;
		}

		//Prints floats
		else if (id == 4)
		{
			cout << "(Token: float" << right << setw(2) << " str = \"" + lexeme + "\"" << ")" << endl;
		}

		// Prints Opearators
		else if ((id >= 31 && id < 33) || (id >= 41 && id < 48) || (id >= 51 && id < 58))
		{
			cout << "(Token: operator" << right << setw(2) << " str = \"" + lexeme + "\"" << ")" << endl;
		}

		//Prints everything else if necessary
		else
		{
			cout << "(Token: id = " << right << setw(2) << to_string(id) << " str= \"" + lexeme + "\"" << ")" << endl;
		}
	}
};


//MAIN
int main() {
	//Declare objects
	Lex lex;
	Token token;
	ifstream inFile;
	inFile.open(test.txt);
	string input = "";

	while (inFile >> input) {
		// increment the line number
		
		char currentChar = '\0';
		char nextChar = '\0';
		int column = -1;
		int state = 1;
		int previousState = 1;
		bool alreadyBackedup = false;
		bool comment = false;

		// tokenize the input
		for (int i = 0; i < input.length(); i++) {
			// get character at current character pointer
			currentChar = input[i];

			// get the next character
			nextChar = input[i + 1];

			// if we have a comment
			if (currentChar == '/' && nextChar == '/') {
				comment = true;
				currentChar = ' ';
			}

			if (currentChar == '#' && nextChar == '#') {
				cout << "Invalid syntax: ## is not allowed" << endl;
			}

			// translate character into column index
			column = lex.getColumnOf(currentChar);

			// set the token start pointer
			// if the token_start is not set
			if (token.start == -1) {

				// if the current charactor is not white space
				if (column != 23) {

					// set token_start to current character pointer
					token.start = i;
				}
			}

			// store the current state as the previous state
			previousState = state;

			// find new state from table
			state = lex.getNewState(state, column);

			// Perform a lookahead if needed
			if (state == 0) {
				state = lex.lookAhead(previousState, currentChar, nextChar);
				if (lex.isAcceptingState(state)) {
					i--;
					alreadyBackedup = true;
				}
			}

			// check if a token has been found
			if (lex.isAcceptingState(state)) {

				// back up the character pointer if needed
				if (lex.needsBackup(state) && !alreadyBackedup) {
					i--;
				}

				alreadyBackedup = false;

				// grab the lexeme
				token.lexeme = input.substr(token.start, i - token.start + 1);

				// reset the token start pointer
				token.start = -1;

				// set the token id
				token.id = lex.getTokenId(state);

				// if token is an identifier
				if (token.id == 2) {

					// if the identifier is a keyword
					if (lex.isKeyword(token.lexeme)) {

						// set token to keyword
						token.id = lex.getKeywordId(token.lexeme);
					}
				}

				token.print();

				// reset the state
				state = 1;
			}

			if (comment) {
				break;
			}
		} 
	} 

	 // check for eof

	if (cin.eof()) {
		token.id = 0;
		token.lexeme = "";
		token.print();
	}
	return 0;
}

int Lex::getColumnOf(char c) {
	//Checks is character is a letter or underscore
	if (isalpha(c) || c == '_')
		return 1;

	//Checks if it is a digit or number
	if (isdigit(c))
		return 2;

	//Checks for white space
	if (isspace(c))
		return 23;

	//Checks for all other characters (Seprators, opeartors, etc..)
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

	case '#':
		return 1;
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
	return 39; // This should never be executed since the switch statement has a default case
}

int Lex::getNewState(int state, int column) {
	return states[state][column];
}

int Lex::lookAhead(int previous_state, char current_char, char next_char) {
	switch (previous_state) {
	case 4:
		if (current_char == '.') {
			if (isdigit(next_char)) { return 6; }
			else { return 5; }
		}
		break;
	default: return 39;
	}
	return 39; // This should never be executed since the switch statement has a default case
}

//Checks to see if the state is an accpeting state based on the character
bool Lex::isAcceptingState(int state) {
	if (states[state][25] != -1) return true;
	return false;
}

bool Lex::needsBackup(int state) {
	if (states[state][25] == 1) return true;
	return false;
}

//Get the keyword id so it can be later on used to display
int Lex::getKeywordId(string lexeme) {
	if (keyword[lexeme] != 0)
		return keyword[lexeme];
	return 3;

}

//Gets the token id so it can be later on used to display the correct token
int Lex::getTokenId(int state) {
	return states[state][0];
}

//Checks to see if a lexeme is a keyword
bool Lex::isKeyword(string lexeme) {
	if (keyword[lexeme] != 0)
		return true;
	return false;
}

