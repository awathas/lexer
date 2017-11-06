/**
* This header file provides the prototype for a parser of the RAT17F language.
*
* The purpose of this parser is to take a the output from a hand built lexer and
* perform a syntatical analysis on it.
*
* Alex Athas, Jeffrey Liv
*/

#ifndef PARSER_H
#define PARSER_H

#include <queue>
#include <iostream>
#include <sstream>
#include <iterator>

#include "Lexer.h"

class Parser {
private:

	// Symbol struct that holds info. about the symbol
	struct Symbol {

		// The name of the symbol which is either a terminal or nonterminal
		string name;
		// Returns the token id of the given corresponding token string
		int getTokenId();
		// Returns the abbreviation of the nonterminal
		string getAbbreviation();
		// Returns true if the symbol is terminal and false otherwise
		bool isTerminal();
		// Returns true if the token matches the symbol
		bool equals(Lexer::Token);
		// A static start symbol with the name: "Pgm" 
		static Symbol START_SYMBOL();
		// A static end marker with the name: "$"
		static Symbol END_MARKER();
	};

	//A struct that holds the parse matrix called table
	struct GrammarMatrix {
	private:
		string table[15][19] = {
			//                     0      1      2      3      4      5      6      7      8      9      10     11     12     13     14     15     16     17     18
			//                     prog   print  input  id     int    float  string (      )      ,      -      +      *      /      ^      ;      {      }      $
			/* 0   Pgm     */{ "kwdprog brace1 Slist brace2",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 1   Slist   */{ "",    "Stmt semi Slist",    "",    "Stmt semi Slist",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "eps",    "" },
			/* 2   Stmt    */{ "",    "S_Out",    "",    "id equal Y1",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 3   S_Out   */{ "",    "kwdprint paren1 Elist paren2",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 4   Y1      */{ "",    "",    "kwdinput",    "E",    "E",    "E",    "E",    "E",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 5   Elist   */{ "",    "",    "",    "E Elist2",    "E Elist2",    "E Elist2",    "E Elist2",    "E Elist2",    "eps",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 6   Elist2  */{ "",    "",    "",    "",    "",    "",    "",    "",    "",    "comma Elist",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 7   E       */{ "",    "",    "",    "T X1",    "T X1",    "T X1",    "T X1",    "T X1",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 8   X1      */{ "",    "",    "",    "",    "",    "",    "",    "",    "eps",    "eps",    "Opadd T X1",    "Opadd T X1",    "",    "",    "",    "eps",    "",    "",    "" },
			/* 9   T       */{ "",    "",    "",    "F X2",    "F X2",    "F X2",    "F X2",    "F X2",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 10  X2      */{ "",    "",    "",    "",    "",    "",    "",    "",    "eps",    "eps",    "eps",    "eps",    "Opmul F X2",    "Opmul F X2",    "Opmul F X2",    "eps",    "",    "",    "" },
			/* 11  F       */{ "",    "",    "",    "Fatom",    "Fatom",    "Fatom",    "Fatom",    "paren1 E paren2",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 12  Fatom   */{ "",    "",    "",    "id",    "int",    "float",    "string",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "" },
			/* 13  Opadd   */{ "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "minus",    "plus",    "",    "",    "",    "",    "",    "",    "" },
			/* 14  Opmul   */{ "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "",    "aster",    "slash",    "caret",    "",    "",    "",    "" },
		};

		// Returns the row index based on the symbol
		int getRow(Symbol);
		// Returns the column index based on the token
		int getColumn(Lexer::Token);
	public:

		// Gets the production based on the Symbol row and Token column
		vector<Symbol> get(Symbol, Lexer::Token);
		// Gets the header of a column based on a token
		string getColumnHeader(Lexer::Token);
	};

	// Predictive Parser table
	GrammarMatrix parser_table;
	// Stack to hold the list of symbols
	vector<Symbol> symbol_stack;
	// Queue to hold the list of tokens
	queue<Lexer::Token> token_queue;
	// Pushes a symbol on the stack of symbols
	void push_symbol(Symbol);
	// Pops a symbol from the stack of symbols
	void pop_symbol();
	// Prints traces of the syntatic analysis
	void print_trace();
	// Prints an error message
	void print_error();
public:
	// Class constructor
	Parser(queue<Lexer::Token> tokenQueue);
	// Starts the syntatic analysis
	void start_parse();
};

#endif
