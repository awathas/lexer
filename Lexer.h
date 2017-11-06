/**
* Header file for Lexer and is used is Lexer.cpp
* Alex Athas, Jeffrey
*/

#ifndef LEXER_H
#define LEXER_H

#include <queue>
#include <map>
#include <string>
#include <iomanip>
#include <iostream>

using namespace std;

class Lexer {
public:
	// An object representing a token of the lexicon
	struct Token {
		int start;
		int id;
		int lineNumber;
		bool isNothing;
		string lexeme;

		Token();
		void print();

		static Token EOF_TOKEN();
	};

	//Constructor for lexer class
	Lexer();
	// Execute the lexer
	void execute();
	// Finds and returns the next token
	Token next_token(string& textLine);
	// Returns the queue of tokens.
	queue<Token> getTokenQueue();
private:
	// FSM States Chart
	int state_table[42][26] = {
		//                     0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25
		//                     ti  lu  d   ,   ;   <   >   {   }   [   ]   (   )   *   ^   :   .   =   -   +   /   "   !   WS  NA  BackUp 
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

	//Store tokens into a queue.
	queue<Token> tokenQueue;
	// A map that has the keyword as the "key" and its token id as the "value"
	map<string, int> key_word;
	// Check if a given lexeme is a keyword.
	bool isKeyword(string lexeme);
	// Checks if a state number is an accepting state.
	bool isAcceptingState(int state);
	// Checks if the character pointer needs to be backed up.
	bool needsBackup(int state);
	// Returns the id of a keyword.
	int getKeywordId(string lexeme);
	// Gets the corresponding state table's column index of an input character.
	int getColumnOf(char c);
	// Return token id from the given state.
	int getTokenId(int state);
	// Returns a new state based on the current state and the input.
	int getNewState(int state, int column);
	// Returns the id of the correct state for ambiguous tokens.
	int lookahead(int previous_state, char current_char, char next_char);
};

#endif
