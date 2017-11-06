/**
* Main entry point of the program.
* 
* Responsible for executing the lexer and the parser.
* Also responsible for linking the output of the lexer to the parser.
*
* Alex Athas, Jeffrey Liv
*/

#include "Parser.h"
#include "Lexer.h"

int main() {
	Lexer lexer;

	//Lexer will initiate and start storing token ID's into the queue.
	lexer.execute();

	//Parser will receive a token queue and set it to its member variable token_queue
	Parser parser(lexer.getTokenQueue());

	//Parser will start parsing with the new queue that was given from lexer.getTokenQueue();
	parser.start_parse();


	return 0;
}
