
#ifndef DECK_H_
#define DECK_H_
#include "Card.h"
#include "Errors.h"

class Deck
{
	Card** cards;
	int cards_num;
public:
	/*
	i did not write throw(name of error) after every function
	because when i do that the compiler write this error:
	"ISO C++17 does not allow dynamic exception specifications"
	*/
	Deck() :cards(nullptr), cards_num(0){}; //Default constructor
	~Deck();
	void operator+=(const Deck &other);
	void operator+=(const Card &newCard);
	void peek_X(const int x)const throw();
	void operator-=(const int x);
	Card& operator[](const int x)const;
	friend ostream& operator<<(ostream& os, const Deck &deck) throw();
	int get_cards_num()const throw();
	void swap_cards(const Card &card, const int i) ;
	void set_deck(Card** deck,int num) throw();
	bool is_empty();
	void remove_card(const Card &card);
};

#endif /* DECK_H_ */
