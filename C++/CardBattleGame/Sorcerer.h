
#ifndef SORCERER_H_
#define SORCERER_H_
#include "Player.h"

class Sorcerer: public Player
{
public:
	Sorcerer(int maxCards, int maxHealth) :Player(maxCards, maxHealth) {}
	virtual ~Sorcerer() {};
	virtual int fightDragon(int damage,const vector<Card> &CardSet,int enemy_health) throw();
	virtual int fightTroll(int damage,const vector<Card> &CardSet,int enemy_health) throw();
	virtual int fightGhost(int damage,const vector<Card> &CardSet,int enemy_health) throw();
};


#endif /* SORCERER_H_ */
