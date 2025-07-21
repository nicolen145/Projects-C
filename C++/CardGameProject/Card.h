/*
 * Card.h
 *
 *  Created on: 12 Feb 2024
 *      Author: nicole
 */

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
	int set_value(const char r);
	int symbol_rank()const;
public:
	Card(char r='D',char s='D',int v = 0); //Constructor (D for default)
	Card(const Card &other); //Copy constructor
	~Card();
	static Card Invalid_Card; //static card for operator [] in deck calss
	void set_rank(char r);
	void set_symbol(char s);
	bool operator==(const Card& other)const;
	bool operator!=(const Card& other)const;
	bool operator<(const Card& other)const;
	bool operator<=(const Card& other)const;
	bool operator>(const Card& other)const;
	bool operator>=(const Card& other)const;
	Card& operator=(const Card& other);
	friend ostream& operator<<(ostream& os, const Card &card);
	bool compere(const Card& other,const char* opr)const;
	char get_rank()const;
	char get_symbol()const;
};

#endif /* CARD_H_ */
