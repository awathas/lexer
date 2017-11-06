/**
* This file includes the definitions for "lexer.h"
*
* Alex Athas, Jeffrey Liv
*/


#include "Lexer.h";

/**
* Return the Queue of tokens
*/
queue<Lexer::Token> Lexer::getTokenQueue() {
	return tokenQueue;
}

/**
* Populate the keyword map with the appropriate token ID(value) of the token type(key).
*/
Lexer::Lexer() {
	key_word = {
		{ "bool", 10 },{ "read", 11 },{ "fcn", 12 },{ "char", 13 },{ "float", 15 },
		{ "integer", 16 },{ "string", 17 },{ "if", 18 },{ "fi", 19 },{ "else", 20 },
		{ "while", 21 },{ "write", 22 },{ "print", 23 },{ "new", 24 },{ "return", 25 }
	};
}

/**
* Executes the lexer program. This function is the main entry point of the program.
*/
void Lexer::execute(void) {
	cout << "If you are not using input redirection with the program, " << endl
		<< "the program will simply return the results of a lexical analysis." << endl
		<< "To perform a syntactic analysis, please redirect input from a file to the program.\n" << endl;

	cout << "*** Starting lexical analysis ***\n" << endl;

	Token token;
	int lineNumber = 0;
	string input = "";

	//Deals with input
	while (getline(cin, input)) {
		lineNumber++;
		while (input.length() != 0) {
			token.isNothing = false;
			token = next_token(input);
			if (token.isNothing) {
				input = "";
			}
			else {
				token.lineNumber = lineNumber;
				token.print();
				tokenQueue.push(token);
			}
		}
	}
	// check for eof
	//This will only be useful if the input is redirected from a text file with source code in it.
	//Otherwise, the lexical analysis does not complete
	if (cin.eof()) {
		token.id = 0;
		token.lexeme = "";
		token.lineNumber = lineNumber;
		tokenQueue.push(token);
		token.print();
	}
	cout << endl;
	cout << "*** Lexcial analysis completed ***\n" << endl;
}

/*
* Returns the first token found in a line of text
*
* @param textLine the line of text to get a token from
* @return The token that was found
*/
Lexer::Token Lexer::next_token(string& textLine) {
	//initalizing token
	Token token;
	char currentChar = '\0';
	char nextChar = '\0';
	int column = -1;
	int state = 1;
	int previousState = 1;
	bool alreadyBackuped = false;

	for (int charPointer = 0; charPointer < textLine.length(); charPointer++) {
		// get character at current character pointer
		currentChar = textLine[charPointer];
		// get the next character
		nextChar = textLine[charPointer + 1];

		// if we have a comment then simply exit
		if (currentChar == '/' && nextChar == '/') {
			token.isNothing = true;
			return token;
		}

		if (currentChar == '%' && nextChar == '%') {
			cout << "Token: Seperator     Lexeme: %% " << endl;
			token.isNothing = true;
			return token;
		}

		if (currentChar == '#' && nextChar == '#') {
			cout << "Syntax Error: Cannot have ##" << endl;
		}

		// translate character into column index
		column = getColumnOf(currentChar);

		// set the token start pointer
		// if the token_start is not set
		if (token.start == -1) {
			// if the current charactor is not white space
			if (column != 23) {
				// set token_start to current character pointer
				token.start = charPointer;
			}
		}

		// store the current state as the previous state
		previousState = state;
		// find new state from table
		state = getNewState(state, column);

		// Resolve a lookahead problem if needed
		if (state == 0) {
			state = lookahead(previousState, currentChar, nextChar);
			if (isAcceptingState(state)) {
				charPointer--;
				alreadyBackuped = true;
			}
		}

		// check if a token has been found
		if (isAcceptingState(state)) {
			// if backup is required
			if (needsBackup(state) && !alreadyBackuped) {
				charPointer--;
			}
			alreadyBackuped = false;

			// grab the lexeme
			token.lexeme = textLine.substr(token.start, charPointer - token.start + 1);
			// reset the token start pointer
			token.start = -1;

			// set the token id
			token.id = getTokenId(state);

			// if token is an identifier
			if (token.id == 2) {
				// if the identifier is a keyword
				if (isKeyword(token.lexeme)) {
					// set token to keyword
					token.id = getKeywordId(token.lexeme);
				}
			}

			textLine.replace(0, charPointer + 1, "");
			return token;
		}
	}


	token.isNothing = true;
	return token;
}

/**
* Check if a given lexeme is a keyword.
*
* @param lexeme the lexeme to identify
*/
bool Lexer::isKeyword(string lexeme) {
	if (key_word[lexeme] != 0) return true;
	return false;
}

/**
* Checks if a state number is an accepting state.
*
* @param state the state id to check
*/
bool Lexer::isAcceptingState(int state) {
	if (state_table[state][25] != -1) return true;
	return false;
}

/**
* Checks if the character pointer needs to be backed up.
*
* @param state the state's id
*/
bool Lexer::needsBackup(int state) {
	if (state_table[state][25] == 1) return true;
	return false;
}

/**
* Returns the id of a keyword.
*
* Returns 0 for an invalid keywords.
*
* @param keyword the keyword value
*/
int Lexer::getKeywordId(string keyword) {
	if (key_word[keyword] != 0) return key_word[keyword];
	return 3; // return state "end id"
}

/**
* Gets the corresponding state table's column index of an input character.
*
* @param c the input character
*/
int Lexer::getColumnOf(char c) {
	if (isalpha(c) || c == '_' || c == '#')  return 1;
	if (isdigit(c))              return 2;
	if (isspace(c))              return 23;
	switch (c) {
	case ',':                return 3;       break;
	case ';':				 return 4;       break;
	case '<':                return 5;       break;
	case '>':                return 6;       break;
	case '{':                return 7;       break;
	case '}':                return 8;       break;
	case '[':                return 9;       break;
	case ']':                return 10;      break;
	case '(':                return 11;      break;
	case ')':                return 12;      break;
	case '*':                return 13;      break;
	case '^':                return 14;      break;
	case ':':                return 15;      break;
	case '.':                return 16;      break;
	case '=':                return 17;      break;
	case '-':                return 18;      break;
	case '+':                return 19;      break;
	case '/':                return 20;      break;
	case '"':                return 21;      break;
	case '!':                return 22;      break;
	default:                 return 24;
	}
	return 39; // Return state "error". This should never be executed since the switch statement has a default case
}

/**
* Returns the token id from the given state.
*
* Returns a 0 for an invalid state id.
*
* @param state the state's id
*/
int Lexer::getTokenId(int state) {
	return state_table[state][0];
}

/**
* Returns a new state based on the current state and the input
*
* @param state  the current state's id
* @param column the column index that corresponds to the given input
*/
int Lexer::getNewState(int state, int column) {
	return state_table[state][column];
}

/**
* Returns the id of the correct state for ambiguous tokens.
*
* @param previous_state the previous state's id
* @param current_char   the current character
* @param next_char      the character at the next character pointer
*/
int Lexer::lookahead(int previous_state, char current_char, char next_char) {
	switch (previous_state) {
	case 4: // If the previous state was "in int"
		if (current_char == '.') {
			if (isdigit(next_char)) { return 6; } // Return state "in float"
			else { return 5; } // Return state "end int"
		}
		break;
	default: return 39; // Return state "error"
	}
	return 39; // Return state "error". This should never be executed since the switch statement has a default case
}

/**
* Token default constructor
*
* Assigns the default values for the member variables start, id and lexeme.
*/
Lexer::Token::Token() {
	start = -1;
	id = -1;
	isNothing = false;
	lexeme = "";
}

/**
* Prints the token in a special format.
*
* (Tok: id= [TOKEN_ID] line= [LINE_NUMBER] str= "[LEXEME]" (int||float) = [VALUE])
*/
void Lexer::Token::print() {
	string temp_lexeme = lexeme;
	if (id == 5) { // if token is a string
		temp_lexeme = temp_lexeme.substr(1, temp_lexeme.length() - 2);
		cout << "Token: String     Lexeme: " << temp_lexeme << endl;
	}
	string additionalOutput = "";
	if (id == 3) { // if token is an int
		cout << "Token: Integer     Lexeme: " << temp_lexeme << endl;
	}
	else if (id == 4) { // if token is a float
		cout << "Token: Float     Lexeme: " << temp_lexeme << endl;
	}
	else if (id <= 25 && id >= 10)
	{
		cout << "Token: Keyword     Lexeme: " << temp_lexeme << endl;
	}
	else if (id == 2)
	{
		cout << "Token: Identifier     Lexeme: " << temp_lexeme << endl;
	}
	else if (id == 6 || id == 7 || (id <= 38 && id >= 33))
	{
		cout << "Token: Seperator     Lexeme: " << temp_lexeme << endl;
	}
	else if ((id >= 12 && id <= 15) || id == 22 || (id >= 45 && id <= 48) || id >= 52 && id <= 55)
	{
		cout << "Token: Operator     Lexeme: " << temp_lexeme << endl;
	}
	else if (id == 0)
	{
		cout << endl;
	}
	else
	{
		//cout << "(Tok: id= " << right << setw(2) << to_string(id) << " line= " << to_string(lineNumber) <<
		//	" str= \"" + temp_lexeme + "\"" << additionalOutput << ")" << endl;
	}
}

/**
* Return an eof token
*/
Lexer::Token Lexer::Token::EOF_TOKEN() {
	Token token;
	token.lexeme = "$";
	token.id = 0;
	return token;
}
