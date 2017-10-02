#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>


using namespace std;

class Lexer {
private:
	int state_table[40][25] = { 1 };
	map<string, int> keyWord;

};

struct Token {
	int start = -1;
	int id;
	string lexme;

	Token() {
		start = -1;
		id = -1;
		lexme = "";
	}
};

int main()
{


	cout << endl;
	system("pause");
	return 0;
}
