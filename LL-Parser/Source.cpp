/**
* Main entry point of the program.
* 
* Responsible for executing the A2 lexer and the A1 parser.
* Also responsible for linking the output of the lexer to the parser.
*
* @author Anthony Nguyen, Sae Hun Kim
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