
#ifndef CARD_H_
#define CARD_H_
#include "string.h"
#include <iostream>
using namespace std;

class Card
{
	char rank;
	char symbol;
	int value;
	int set_value(const char r) throw();
	int symbol_rank()const throw();
public:
	Card(char r='D',char s='D',int v = 0) ; //Constructor (D for default)
	Card(const Card &other) ; //Copy constructor
	~Card()  {};
	void set_rank(char r) throw();
	void set_symbol(char s) throw();
	bool operator==(const Card& other)const throw();
	bool operator!=(const Card& other)const throw();
	bool operator<(const Card& other)const throw();
	bool operator<=(const Card& other)const throw();
	bool operator>(const Card& other)const throw();
	bool operator>=(const Card& other)const throw();
	Card& operator=(const Card& other) throw();
	friend ostream& operator<<(ostream& os, const Card &card) throw();
	bool compere(const Card& other,const char* opr)const throw();
	char get_rank()const throw();
	char get_symbol()const throw();
	int get_value()const throw(){return value;};
};

#endif /* CARD_H_ */
