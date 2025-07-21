
#include "Deck.h"
#include "Card.h"
#include <iostream>
#include "string.h"
using namespace std;

Deck::~Deck()
{
	//deletes every card in cards** array and then the array it self.
	for (int i = 0; i < cards_num; i++){delete cards[i];}
	delete[]cards;
}

int Deck::get_cards_num()const
{
	return cards_num;
}

bool Deck::operator+=(const Deck &other)
{
	Card** temp_deck;
	temp_deck = new Card*[cards_num + other.cards_num];
	if (!temp_deck) {return false;} //memory fail
	//Iterate the array backwards and move the current cards in temp_deck
	for (int i = cards_num+other.cards_num-1; i>=other.cards_num; i--) {
		temp_deck[i] = cards[i-other.cards_num];}
	//add new cards
	for (int i = other.cards_num-1;i>=0;i--) {
		Card *new_card;
		new_card =new Card(*other.cards[i]);
		temp_deck[i] = new_card;}
	delete[]cards; //delete old array
	cards = temp_deck; //update new array
	cards_num = cards_num + other.cards_num; //update number of cards
	return true;
}

void Deck::peek_X(const int x)const
{
	if ( x < 0 ) cout << "Invalid number" << endl;
	else if (x > cards_num)
	{
		for (int i = cards_num-1; i >= 0; i--) { cout << *cards[i] << endl; }
	}
	else
	{
		for (int i = cards_num - 1; i >= cards_num - x ;i--) { cout << *cards[i] << endl; }
	}
}

Card& Deck::operator[](const int x)const
{
	if (x < 0 || x >= cards_num){
		cout << "Invalid index" << endl;
		return Card::Invalid_Card;} //return invalid card
	else {return *cards[x];}
}


bool Deck::if_valid_card(const Card& card)const
{
	//check if the card has invalid members
	if (card.get_rank() == 'X' && card.get_symbol() == 'X') {return false;}
	return true;
}
bool Deck::operator-=(const int x)
{
	if (x < 0) {cout << "Invalid number" << endl; return true;}
	else if (x >= cards_num)
	{
		//delete all array
		for (int i = 0;i < cards_num;i++) {delete cards[i];}
		delete[]cards;
		cards = nullptr; //update array to null
		cards_num = 0; //update number of cards to 0
		return true;
	}
	Card** temp_deck = new Card * [cards_num - x];
	if (!temp_deck) {return false;}
	for (int i = cards_num - 1;i >=0 ;i -- ){
		if ( i > cards_num - x - 1) {delete cards[i];}
		else {temp_deck[i] = cards[i];}
	}
	delete[]cards; //delete old array
	cards = temp_deck; //update new array
	cards_num -= x; //update number of cards
	return true;
}

ostream& operator<<(ostream& os, const Deck &deck)
{
	if (deck.cards_num == 0) {cout << "[]"; return os;}
	cout << "[";
	for (int i = 0; i < deck.cards_num; i++)
	{
		if (i == deck.cards_num - 1) {cout << *(deck.cards[i]) << "]";} //if it reach the final card in the deck
		else { cout <<*(deck.cards[i]) << ",";}

	}return os;
}

void Deck::swap_cards(const Card &card, const int i)
{
	if (i < 0 || i >= cards_num) {cout << "Invalid index" << endl;}
	else {
		delete cards[i]; //delete old card
		Card *new_card = new Card(card);
		cards[i] = new_card; //update new card
	}
}



void Deck::set_deck(Card** deck,int num)
{
	this->cards = deck;
	this->cards_num = num;
}

