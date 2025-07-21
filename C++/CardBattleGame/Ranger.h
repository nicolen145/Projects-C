

#ifndef RANGER_H_
#define RANGER_H_
#include "Player.h"

class Ranger: public Player
{
public:
	Ranger(int maxCards, int maxHealth) :Player(maxCards, maxHealth){}
	virtual ~Ranger() {};
	virtual int fightDragon(int damage,const vector<Card> &CardSet,int enemy_health) throw();
	virtual int fightTroll(int damage,const vector<Card> &CardSet,int enemy_health) throw();
	virtual int fightGhost(int damage,const vector<Card> &CardSet,int enemy_health) throw();
};


#endif /* RANGER_H_ */
