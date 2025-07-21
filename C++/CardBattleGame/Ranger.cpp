
#include "Ranger.h"

int Ranger::fightTroll(int damage,const vector<Card> &CardSet,int enemy_health) throw()
{
	return this->without_S_C_affect(damage,CardSet,enemy_health);
};


int Ranger::fightGhost(int damage,const vector<Card> &CardSet,int enemy_health) throw()
{
	return this->without_D_affect(damage,CardSet,enemy_health);
};

int Ranger::fightDragon(int damage,const vector<Card> &CardSet,int enemy_health) throw()
{
	return this->without_H_affect(damage,CardSet,enemy_health);
};


