
#ifndef FIGHTER_H_
#define FIGHTER_H_
#include "Player.h"

class Fighter: public Player
{
public:
	Fighter(int maxCards, int maxHealth):Player(maxCards, maxHealth) {}
	virtual ~Fighter() {};
	virtual int fightTroll(int damage,const vector<Card> &CardSet,int enemy_health) throw();
	virtual int fightGhost(int damage,const vector<Card> &CardSet,int enemy_health) throw();
	virtual int fightDragon(int damage,const vector<Card> &CardSet,int enemy_health) throw();

};


#endif /* FIGHTER_H_ */
