#include "Card.h"
#include <iostream>
using namespace std;



Card::Card(char r,char s,int v)
{
	rank = r;
	symbol = s;
	value = set_value(r);
}

Card::Card(const Card &other)
{
	rank = other.rank;
	symbol = other.symbol;
	value = other.value;
}

int Card::symbol_rank()const throw()
{
	if (symbol == 'C') {return 4;}
	if (symbol == 'S') {return 3;}
	if (symbol == 'H') {return 2;}
	if (symbol == 'D') {return 1;}
	return 0;

}
bool Card::operator==(const Card& other)const throw()
{
	return rank == other.rank && symbol == other.symbol;
}

bool Card::operator!=(const Card& other)const throw()
{
	return !(*this == other);
}

bool Card::operator<(const Card& other)const throw()
{
	if (rank == other.rank){
		return (symbol_rank() < other.symbol_rank());
	}
	//Additional checking for T and J card (because they have the same value but J is bigger).
	else if (rank == 'T' and other.rank == 'J') {return true;}
	else if (rank == 'J' and other.rank == 'T') {return false;}
	else {return value < other.value;}
}

bool Card::operator<=(const Card& other)const throw()
{
	return (*this < other) || (*this == other);
}

bool Card::operator>(const Card& other)const throw()
{
	return !(*this<other);
}

bool Card::operator>=(const Card& other)const throw()
{
	return (*this > other) || (*this == other);
}

Card& Card::operator=(const Card& other) throw()
{
	this->rank = other.rank;
	this->symbol = other.symbol;
	this->value = other.value;
	return *this;
}

//calculate the value according to the rank:
int Card::set_value(char r) throw()
{
	if (r == 'X') {return 0;} //the invalid card
	if (r == 'A') {return  1;}
	if (r == 'J') {return 10;}
	if (r == 'T') {return 10;}
	if (r == 'Q') {return 15;}
	if (r == 'K') {return 20;}
	else {return rank - '0';} //Calculation with ASCII table for cards 2-8
}

ostream& operator<<(ostream& os, const Card& card) throw()
{
	char r = card.rank;
	char s = card.symbol;
    os << r << s;
    return os;
}

char Card::get_rank()const throw()
{
	return rank;
}
char Card::get_symbol()const throw()
{
	return symbol;
}

//helper function for case 2 in main
bool Card::compere(const Card& other,const char* opr)const throw()
{
	if (strcmp(opr,">") == 0) {return *this > other;}
	if (strcmp(opr,">=") == 0) {return *this >= other;}
	if (strcmp(opr,"<") == 0) {return *this < other;}
	if (strcmp(opr,"<=") == 0) {return *this <= other;}
	if (strcmp(opr,"==") == 0) {return *this == other;}
	return false;

}


void Card::set_rank(char r) throw()
{
	rank = r;
	value = this->set_value(r);
}
void Card::set_symbol(char s) throw()
{
	symbol = s;
}
