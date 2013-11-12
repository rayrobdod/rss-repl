

#include "parseCommand.h"

#define BUFFER_SIZE      256
#define QUOTE_CHAR       L'\"'
using std::list;


list<wchar_t*>* parseCommand() {
	wchar_t currentChar = L'\0';
	int isQuoted = 0;
	list<wchar_t*>* params = new list<wchar_t*>();
	wchar_t* currentString = (wchar_t*) malloc(BUFFER_SIZE * sizeof(wchar_t));
	int currentStringIndex = 0;
	
	while (currentChar != L'\n') {
		currentChar = fgetwc(stdin);
		
		if (isQuoted) {
			if (currentChar == QUOTE_CHAR) {
				isQuoted = 0;
			} else {
				currentString[currentStringIndex] = currentChar;
				currentStringIndex++;
			}
		} else {
			if (currentChar == QUOTE_CHAR) {
				isQuoted = 1;
			} else if (currentChar == L' ') {
				params->push_back(currentString);
				currentString = (wchar_t*) malloc(BUFFER_SIZE * sizeof(wchar_t));
				currentStringIndex = 0;
				
			} else {
				currentString[currentStringIndex] = currentChar;
				currentStringIndex++;
			}
		}
	}
	
	params->push_back(currentString);
	return params;
}
