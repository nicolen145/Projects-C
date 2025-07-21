#include <vector>
#include <string>
#include "Deck.h"
#include "Fighter.h"
#include "Ranger.h"
#include "Sorcerer.h"
#include "Ghost.h"
#include "Troll.h"
#include "Dragon.h"
#include <iostream>
#include <sstream>
#include <limits>
#include "Errors.h"

using namespace std;

//helper func for calculating total value of the set
int total_value(vector<Card> &Cardset)
{
	int value = 0;
	for (size_t i = 0; i < Cardset.size(); ++i) {
		value += Cardset[i].get_value();
	}
	return value;
}




vector<Card> make_cards_set(const string& input)
{
	vector<Card> Cardset;
	stringstream ss(input);
	string card;
    while (ss >> card) {
    	Card c = Card(card[0],card[1]);
        Cardset.push_back(c);
    }
    return Cardset;
}

vector<Card> remove_aces(const vector<Card> &v)
{
	vector<Card> without_aces;
	for (size_t i = 0; i < v.size(); ++i)
		{
			if (v[i].get_rank() != 'A') {without_aces.push_back(v[i]);}
		}
	return without_aces;
}

//helper func to check if the set is valid
bool set_is_valid(vector<Card> &Cardset)
{
	int size = Cardset.size();
	if (size == 1) {return true;} //valid in all cases.
	if (size == 2) //valid if one card is any valid card and the other is an Ace.
	{
		if (Cardset[0].get_rank() == 'A' || Cardset[1].get_rank() == 'A') {return true;}
	}
	vector<Card> without_aces;
	without_aces = remove_aces(Cardset);
	if (size - without_aces.size() > 1) {return false;} //there is more then one ace in a set of size >= 3
	int total = total_value(without_aces);
	if (total <=10 )
	{
		pair<char,int> p;
		if (without_aces[0].get_value() <= 5 )
		{
			p.first = without_aces[0].get_rank();
			p.second = 1;
		}

		for (size_t i = 1; i < without_aces.size(); ++i)
		{
			if (without_aces[i].get_rank() == p.first) {p.second ++;}
			else {return false;}
		}
		if (p.second == 2) {return true;} // valid if: 2X-2Y/3X-3Y/4X-4Y/5X-5Y
		else if (p.second == 3 && (p.first == '2' || p.first == '3') ) {return true;} //valid if: 2X-2Y-2Z/3X-3Y-3Z
		else if (p.second == 4 && p.first == '2') {return true;} //valid if: 2X-2Y-2Z-2T
	}
	//if not any of the cases the set is not valid
	return false;

}

void remove_from_hand(vector<Card> &set,Deck *hand)
{
	for (size_t i = 0; i < set.size(); i++)
	{
		hand->remove_card(set[i]);
	}
}


int main()
{
	Player *p = nullptr;
	Enemy *e = nullptr;
	Deck *deck = new Deck;
	Deck *hand = new Deck;
	int i;
	bool result;
	Card c;
	string input;
	vector<Card> Cardset;
	//Initialize deck:
	cout << "Initialize deck" << endl;
	try {
		while (1)
		{
			cin >> input;
			//stop condition for initialize deck:
			if (input == "00") {break;}
			c = Card(input[0],input[1]);
			//fill deck with cards (with new operator += thats receives a card):
			*deck += c;
		}
	}
	catch (exception &exc) {
		cout << exc.what() << endl;
		//clean memory and exit
		delete deck;
		delete hand;
		delete p;
		delete e;
		return 1;
	}
	//choose player:
	cout << "Choose player character:" << endl;
	cout << "(1) Fighter (2) Sorcerer (3) Ranger" << endl;
	int choise;
	cin >> choise;
	switch(choise)
	{
	case 1:
		p = new Fighter(6,60);
		break;
	case 2:
		p = new Sorcerer(8,40);
		break;
	case 3:
		p = new Ranger(7,50);
		break;
	}
	//choose enemy:
	cout << "Choose enemy character:" << endl;
	cout << "(1) Troll (2) Ghost (3) Dragon" << endl;
	cin >> choise;
	switch(choise)
	{
	case 1:
		e = new Troll(100,5);
		break;
	case 2:
		e = new Ghost(50,15);
		break;
	case 3:
		e = new Dragon(75,10);
		break;
	}
	////Initialize hand:
	try {
		while (hand->get_cards_num() < p->get_max() && deck->is_empty() == false){
			//fill hand cards from deck:
			*hand += (*deck)[deck->get_cards_num()-1];
			*deck -= 1;
		}
	}
	catch (exception &exc) {
		cout << exc.what() << endl;
		//clean memory and exit
		delete deck;
		delete hand;
		delete p;
		delete e;
		return 1;
	}
	//while both player and enemy are alive:
	try {
		while (!(p->is_defeated()) && !(e->is_defeated()))
		{
			cout << "Player health: " << p->get_health() << endl;
			cout << "Enemy health: " << e->get_health() << endl;
			cout << "Player hand\n" << *hand << endl;
			cout << "Insert card set to play" << endl;
			cin >> ws;
			getline(cin,input);
			if (input == "exit")
			{
				//clean memory and exit
				delete deck;
				delete hand;
				delete p;
				delete e;
				return 0;
			}
			Cardset = make_cards_set(input);
			//if the set is not valid:
			while (!(set_is_valid(Cardset)))
			{
				cout << "Card set is not valid" << endl;
				cout << "Insert card set to play" << endl;
				//cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin >> ws;
				getline(cin,input);
				if (input == "exit")
				{
					//clean memory and exit
					delete deck;
					delete hand;
					delete p;
					delete e;
					return 0;
				}
				Cardset = make_cards_set(input);
			}
			//if the set is valid:
			if (set_is_valid(Cardset)) {remove_from_hand(Cardset,hand);}
			//fight:
			e->fight(*p,Cardset,e->get_health());
			// fill hand cards:
			i = 0;
			while (hand->get_cards_num() < p->get_max() && i < 2 && !(p->is_defeated()) && !(e->is_defeated()))
			{
				if (deck->is_empty() == false)
					*hand += (*deck)[deck->get_cards_num()-1];
					*deck -= 1;
					i++;
			}
		}
	}
	catch (exception &exc) {
		cout << exc.what() << endl;
		//clean memory and exit
		delete deck;
		delete hand;
		delete p;
		delete e;
		return 1;
	}
	//if one of them is defeated check who won:
	if (e->is_defeated())
	{
		cout << "Player won" << endl;
	}
	if (p->is_defeated())
	{
		cout << "Player lost" << endl;
	}
	//clean memory:
 	delete deck;
	delete hand;
	delete p;
	delete e;
	//exit game:
	return 0;
}




