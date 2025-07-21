
#include "Deck.h"
#include "Card.h"
#include <iostream>
#include "string.h"
using namespace std;

Deck::~Deck() throw()
{
	//deletes every card in cards** array and then the array it self.
	for (int i = 0; i < cards_num; i++){
		delete cards[i];}
	delete[]cards;
}

int Deck::get_cards_num()const throw()
{
	return cards_num;
}

void Deck::operator+=(const Deck &other)
{
	Card** temp_deck;
	temp_deck = new Card*[cards_num + other.cards_num];
	if (!temp_deck) {throw MemoryError();} //memory fail
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
}

void Deck::operator+=(const Card &newCard)
{
	Card** temp_deck;
	temp_deck = new Card*[cards_num + 1];
	if (!temp_deck) {throw MemoryError();}
	for (int i = cards_num; i>0; i--) {
		temp_deck[i] = cards[i-1];}
	Card *new_card = new Card(newCard);
	temp_deck[0] = new_card;
	delete[]cards;
	cards = temp_deck;
	cards_num ++;
}

void Deck::peek_X(const int x)const throw ()
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
		throw InvalidIndex();}
	else {return *cards[x];}
}


void Deck::operator-=(const int x)
{
	if (x >= cards_num)
	{
		//delete all array
		for (int i = 0;i < cards_num;i++) {delete cards[i];}
		delete[]cards;
		cards = nullptr; //update array to null
		cards_num = 0; //update number of cards to 0
	}
	else if ( x < cards_num && x > 0){
		Card** temp_deck = new Card * [cards_num - x];
		if (!temp_deck) {throw MemoryError();}
		for (int i = cards_num - 1;i >=0 ;i -- ){
			if ( i > cards_num - x - 1) {
   				delete cards[i];}
			else {temp_deck[i] = cards[i];}
			}
		delete[]cards; //delete old array
		cards = temp_deck; //update new array
		cards_num -= x; //update number of cards
		}
	else {cout << "Invalid number" << endl;}
}

ostream& operator<<(ostream& os, const Deck &deck) throw()
{
	if (deck.cards_num == 0) {cout << " "; return os;}
	for (int i = 0; i < deck.cards_num; i++)
	{
		if (i == deck.cards_num - 1) {cout << *(deck.cards[i]) << " ";} //if it reach the final card in the deck
		else { cout <<*(deck.cards[i]) << " ";}

	}return os;
}

void Deck::swap_cards(const Card &card, const int i)
{
	if (i < 0 || i >= cards_num) {throw InvalidIndex();}
	else {
		delete cards[i]; //delete old card
		Card *new_card = new Card(card);
		cards[i] = new_card; //update new card
	}
}



void Deck::set_deck(Card** deck,int num) throw()
{
	this->cards = deck;
	this->cards_num = num;
}

void Deck::remove_card(const Card &card)
{
	if (cards_num == 1)
	{
		delete cards[0];
		delete[]cards;
		cards = nullptr;
		cards_num --;
	}
	Card** temp_deck = new Card * [cards_num - 1];
	if (!temp_deck) {throw MemoryError();}
	int i = cards_num - 2;
	int j = cards_num - 1;
	while (i > -1)
	{
		if (*cards[j] == card)
		{
			delete cards[j];
			j--;
		}
		else
		{
			temp_deck[i] = cards[j];
			i--;
			j--;
		}
	}
	// the while loop will skip the card in cards[0] so if this card is the card that needs to be deleted:
	if (*cards[0] == card) {delete cards[0];}
	delete[]cards;
	cards = temp_deck;
	cards_num --;
}

bool Deck::is_empty()
{
	if (cards_num == 0)
		throw DeckRanOut();
	else
		return false;
}

