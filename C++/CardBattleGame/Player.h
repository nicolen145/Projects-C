
#ifndef PLAYER_H_
#define PLAYER_H_
#include "Card.h"
#include <iostream>
#include <vector>
class Enemy;

using namespace std;

class Player
{
	int health_points;
	const int max_cards;
	const int max_health;

public:
	Player(int maxCards, int maxHealth) : health_points(maxHealth), max_cards(maxCards), max_health(maxHealth){}
	virtual ~Player() {};
	virtual int fightDragon(int damage,const vector<Card> &CardSet,int enemy_health) throw() =0;
	virtual int fightTroll(int damage,const vector<Card> &CardSet,int enemy_health) throw() =0;
	virtual int fightGhost(int damage,const vector<Card> &CardSet,int enemy_health) throw() =0;
	bool is_defeated()const throw(){return health_points <= 0;}
	void add_health(int health) throw();
	int get_health() const throw(){return health_points;}
	int set_total_value(const vector<Card> &CardsSet)const throw();
	bool set_has_sign(const vector<Card> &CardsSet,char sign)const throw();
	void get_hurt(int damage) throw();
	int without_D_affect(int damage,const vector<Card> &CardsSet,int enemy_health) throw();
	int without_H_affect(int damage,const vector<Card> &CardsSet,int enemy_health) throw();
	int without_S_C_affect(int damage,const vector<Card> &CardsSet,int enemy_health) throw();
	int get_max() throw(){return max_cards;}

};

#endif /* PLAYER_H_ */
