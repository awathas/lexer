/**
* This file includes the definitions for "parser.h"
*
* See the header comment on "a1parser.h" for more information.
*
* Alex Athas, Jeffrey Liv
*/

#include "Parser.h"

//Parser takes in the queue of tokens
Parser::Parser(queue<Lexer::Token> tokenQueue) {
	token_queue = tokenQueue;
}

void Parser::start_parse() {
	cout << "*** Starting syntactic analysis ***\n" << endl;

	// Push $ (an end marker) onto the stack.
	push_symbol(Symbol::END_MARKER());

	// Push the start symbol onto the stack.
	push_symbol(Symbol::START_SYMBOL());

	while (!symbol_stack.empty()) {
		// Let top_symbol = top of stack and next_token = incoming token.
		Symbol top_symbol = symbol_stack.back();
		Lexer::Token next_token = token_queue.front();

		// If top_symbol is a terminal
		if (top_symbol.isTerminal()) {
			if (top_symbol.equals(next_token)) {
				token_queue.pop();
				pop_symbol();
			}
			else {
				print_error();
				break;
			}
		}
		else { // x is a nonterminal
			vector<Symbol> production = parser_table.get(top_symbol, next_token);

			// if Table[top_symbol, next_token] is nonblank then
			if (production.size() != 0) {
				pop_symbol();

				// push Table[x,a] onto stack in reverse order
				for (int i = production.size() - 1; i >= 0; i--) {
					if (production[i].name != "eps") { push_symbol(production[i]); }
				}
			}
			else {
				print_error();
				break;
			}
		}
	}
	cout << endl;
	if (token_queue.empty() && symbol_stack.empty()) {
		cout << "*** Parse successfully completed ***\n" << endl;
	}

	else {
		cout << "*** Parse unsuccessfully completed ***\n" << endl;
	}
}

/**
* Pushes the symbol object to the "top" of symbol_stack. After
* the symbol object has been pushed print_trace() is called.
*
* @param symbol the current symbol
*/
void Parser::push_symbol(Symbol symbol) {
	symbol_stack.push_back(symbol);
	print_trace();
}


/**
* Pops a symbol off the "top" of the symbol_stack and calls the function
* print_trace().
*/
void Parser::pop_symbol() {
	symbol_stack.pop_back();
	print_trace();
}

/**
* Prints all the elements in the symbol_stack and outputs the current
* front token in token_queue.
*/
void Parser::print_trace() {
	for (int i = 0; i < symbol_stack.size(); i++) {
		cout << symbol_stack[i].getAbbreviation();
	}
	if (!token_queue.empty()) {
		cout << "\t\t\t";
		token_queue.front().print();
	}
}

/**
* Prints an error message that displays the current symbol and the incoming token.
* Also prints out the line number if known.
*/
void Parser::print_error() {
	cout << "An error has occured: " << endl;

	// the token (i.e., column header)
	cout << "\tToken = " << parser_table.getColumnHeader(token_queue.front()) << endl;

	// the top-of-stack symbol (i.e., the rule LHS)
	cout << "\tTop-of-stack symbol = " << symbol_stack.back().name << endl;

	// token's line number (if known)
	string lineNumber = "";
	if (token_queue.front().lineNumber == -1) {
		lineNumber = "an unknown line";
	}
	else {
		lineNumber = "line " + to_string(token_queue.front().lineNumber);
	}
	cout << "on " << lineNumber << ".\n" << endl;
}

/**
* Checks if a symbol is a terminal
*
* @returns true if the symbol is a terminal, otherwise return false
*/
bool Parser::Symbol::isTerminal() {
	return ((name[0] >= 'a' && name[0] <= 'z') || (name[0] == '$'));
}

/**
* Checks if a symbol mataches a token by comparing the id's of the symbol and token
*
* @returns true if the symbol matches the token, otherwise return false
*/
bool Parser::Symbol::equals(Lexer::Token token) {
	return (getTokenId() == token.id);
}

/**
* Returns the token id of the symbol
*
* @returns the matching token id. If symbol does not match a token, returns -1
*/
int Parser::Symbol::getTokenId() {
	if (name == "kwdprog") { return 10; }
	else if (name == "brace1") { return 33; }
	else if (name == "brace2") { return 34; }
	else if (name == "semi") { return 7; }
	else if (name == "id") { return 2; }
	else if (name == "equal") { return 45; }
	else if (name == "kwdinput") { return 22; }
	else if (name == "kwdprint") { return 23; }
	else if (name == "paren1") { return 37; }
	else if (name == "paren2") { return 38; }
	else if (name == "comma") { return 6; }
	else if (name == "int") { return 3; }
	else if (name == "float") { return 4; }
	else if (name == "string") { return 5; }
	else if (name == "plus") { return 47; }
	else if (name == "minus") { return 46; }
	else if (name == "aster") { return 41; }
	else if (name == "slash") { return 48; }
	else if (name == "caret") { return 42; }
	else if (name == "$") { return 0; }
	else { return -1; }
}

/**
* Returns an abbreviation of the symbol
*
* @returns a shortened symbol name
*/
string Parser::Symbol::getAbbreviation() {
	if (name == "E") { return "E."; }
	else if (name == "Elist") { return "EL."; }
	else if (name == "Elist2") { return "E2."; }
	else if (name == "F") { return "F."; }
	else if (name == "Fatom") { return "Fa."; }
	else if (name == "Opadd") { return "O+."; }
	else if (name == "Opmul") { return "O*."; }
	else if (name == "Pgm") { return "Pg."; }
	else if (name == "S_Asgn") { return "S=."; }
	else if (name == "S_In") { return "SI."; }
	else if (name == "S_Out") { return "SO."; }
	else if (name == "Slist") { return "SL."; }
	else if (name == "Stmt") { return "S."; }
	else if (name == "Stmts") { return "Ss."; }
	else if (name == "T") { return "T."; }
	else if (name == "X1") { return "X1."; }
	else if (name == "X2") { return "X2."; }
	else if (name == "Y1") { return "Y1."; }
	else if (name == "aster") { return "*."; }
	else if (name == "brace1") { return "{."; }
	else if (name == "brace2") { return "}."; }
	else if (name == "caret") { return "^."; }
	else if (name == "comma") { return ",."; }
	else if (name == "equal") { return "=."; }
	else if (name == "float") { return "fl."; }
	else if (name == "id") { return "id."; }
	else if (name == "int") { return "it."; }
	else if (name == "kwdinput") { return "ki."; }
	else if (name == "kwdprint") { return "ko."; }
	else if (name == "kwdprog") { return "kp."; }
	else if (name == "minus") { return "-."; }
	else if (name == "paren1") { return "(."; }
	else if (name == "paren2") { return ")."; }
	else if (name == "plus") { return "+."; }
	else if (name == "semi") { return ";."; }
	else if (name == "slash") { return "/."; }
	else if (name == "string") { return "st."; }
	else if (name == "$") { return "$"; }
	else if (name == "eps") { return ""; }
	else { return "-1"; }
}

/**
* Returns a symbol representing a start symbol
*/
Parser::Symbol Parser::Symbol::START_SYMBOL() {
	Symbol start_symbol;
	start_symbol.name = "Pgm";
	return start_symbol;
}

/**
* Returns a symbol representing an end marker
*/
Parser::Symbol Parser::Symbol::END_MARKER() {
	Symbol end_marker;
	end_marker.name = "$";
	return end_marker;
}

/**
* Returns the row index of the predictive parser table based on a symbol
*
* @param symbol a symbol representing a row of the predictive parser table
* @return the matching row index. If no match is found, returns a -1
*/
int Parser::GrammarMatrix::getRow(Parser::Symbol symbol) {
	if (symbol.name == "Pgm") { return  0; }
	else if (symbol.name == "Slist") { return  1; }
	else if (symbol.name == "Stmt") { return  2; }
	else if (symbol.name == "S_Out") { return  3; }
	else if (symbol.name == "Y1") { return  4; }
	else if (symbol.name == "Elist") { return  5; }
	else if (symbol.name == "Elist2") { return  6; }
	else if (symbol.name == "E") { return  7; }
	else if (symbol.name == "X1") { return  8; }
	else if (symbol.name == "T") { return  9; }
	else if (symbol.name == "X2") { return  10; }
	else if (symbol.name == "F") { return  11; }
	else if (symbol.name == "Fatom") { return  12; }
	else if (symbol.name == "Opadd") { return  13; }
	else if (symbol.name == "Opmul") { return  14; }
	else /* Error */ { return -1; }
}

/**
* Returns the column index of the predictive parser table based on a token
*
* @param token a token representing a column of the predictive parser table
* @return the matching column index. If no match is found, returns a -1
*/
int Parser::GrammarMatrix::getColumn(Lexer::Token token) {
	if (token.id == 10) { return 0; }
	else if (token.id == 23) { return 1; }
	else if (token.id == 22) { return 2; }
	else if (token.id == 2) { return 3; }
	else if (token.id == 3) { return 4; }
	else if (token.id == 4) { return 5; }
	else if (token.id == 5) { return 6; }
	else if (token.id == 37) { return 7; }
	else if (token.id == 38) { return 8; }
	else if (token.id == 6) { return 9; }
	else if (token.id == 46) { return 10; }
	else if (token.id == 47) { return 11; }
	else if (token.id == 41) { return 12; }
	else if (token.id == 48) { return 13; }
	else if (token.id == 42) { return 14; }
	else if (token.id == 7) { return 15; }
	else if (token.id == 33) { return 16; }
	else if (token.id == 34) { return 17; }
	else if (token.id == 0) { return 18; }
	else    /* Error */ { return -1; }
}

/**
* Returns the column header of the predictive parser table based on a token
*
* @param token a token representing a column of the predictive parser table
* @return the matching column header. If no match is found, returns an error column header
*/
string Parser::GrammarMatrix::getColumnHeader(Lexer::Token token) {
	if (token.id == 10) { return "kwdprog"; }
	else if (token.id == 23) { return "kwdprint"; }
	else if (token.id == 22) { return "kwdinput"; }
	else if (token.id == 2) { return "id"; }
	else if (token.id == 3) { return "int"; }
	else if (token.id == 4) { return "float"; }
	else if (token.id == 5) { return "string"; }
	else if (token.id == 37) { return "parens1"; }
	else if (token.id == 38) { return "parens2"; }
	else if (token.id == 6) { return "comma"; }
	else if (token.id == 46) { return "minus"; }
	else if (token.id == 47) { return "plus"; }
	else if (token.id == 41) { return "aster"; }
	else if (token.id == 48) { return "slash"; }
	else if (token.id == 42) { return "caret"; }
	else if (token.id == 7) { return  "semi"; }
	else if (token.id == 33) { return "brace1"; }
	else if (token.id == 34) { return "brace2"; }
	else if (token.id == 0) { return  "$"; }
	else    /* Error */ { return "Error: unknown token in A1 Grammar"; }
}

/**
* Returns a production rule from a predictive parser table based on the
* current symbol and the incoming token.
*
* @param symbol the current symbol
* @param token the incoming token
* @return a production rule
*/
vector<Parser::Symbol> Parser::GrammarMatrix::get(Parser::Symbol symbol, Lexer::Token token) {
	string production = table[getRow(symbol)][getColumn(token)];

	istringstream ss(production);
	istream_iterator<string> begin(ss), end;

	vector<string> symbol_names(begin, end);

	vector<Parser::Symbol> symbols;
	for (int i = 0; i < symbol_names.size(); i++) {
		Parser::Symbol s;
		s.name = symbol_names[i];
		symbols.push_back(s);
	}

	return symbols;
}
