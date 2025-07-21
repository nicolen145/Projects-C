
#ifndef ERRORS_H_
#define ERRORS_H_
#include <iostream>
#include <exception>
#include <string>
using namespace std;

class MemoryError: public exception
{
	virtual const char* what() const throw(){
		return "Memory error";
	}
};

class DeckRanOut: public exception
{
	virtual const char* what() const throw(){
		return "Deck ran out";
	}
};

class InvalidIndex: public exception
{
	virtual const char* what() const throw(){
		return "Invalid index";
	}
};


#endif /* ERRORS_H_ */
