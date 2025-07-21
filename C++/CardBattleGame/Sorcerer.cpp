
#include "Sorcerer.h"


int Sorcerer::fightTroll(int damage,const vector<Card> &CardSet,int enemy_health) throw()
{
	return this->without_D_affect(damage,CardSet,enemy_health);
};


int Sorcerer::fightGhost(int damage,const vector<Card> &CardSet,int enemy_health) throw()
{
	return this->without_H_affect(damage,CardSet,enemy_health);
};

int Sorcerer::fightDragon(int damage,const vector<Card> &CardSet,int enemy_health) throw()
{
	return this->without_S_C_affect(damage,CardSet,enemy_health);
};


