
#include "Fighter.h"

//void Fighter::fightDragon(Enemy &e,const vector<Card> &CardSet)

int Fighter::fightTroll(int damage,const vector<Card> &CardSet,int enemy_health) throw()
{
	return this->without_H_affect(damage,CardSet,enemy_health);
};

int Fighter::fightGhost(int damage,const vector<Card> &CardSet,int enemy_health) throw()
{
	return this->without_S_C_affect(damage,CardSet,enemy_health);
};

int Fighter::fightDragon(int damage,const vector<Card> &CardSet,int enemy_health) throw()
{
	return this->without_D_affect(damage,CardSet,enemy_health);
};

