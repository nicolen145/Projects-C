
#ifndef DECK_H_
#define DECK_H_
#include "Card.h"

class Deck
{
	Card** cards;
	int cards_num;
public:
	Deck() :cards(nullptr), cards_num(0){}; //Default constructor
	~Deck();
	bool operator+=(const Deck &other);
	void peek_X(const int x)const;
	bool operator-=(const int x);
	Card& operator[](const int x)const;
	friend ostream& operator<<(ostream& os, const Deck &deck);
	int get_cards_num()const;
	void swap_cards(const Card &card, const int i);
	void set_deck(Card** deck,int num);
	bool if_valid_card(const Card& card)const;
};

#endif /* DECK_H_ */
