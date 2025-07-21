#include "Troll.h"


class Enemy;

void Troll::fight(Player &p,const vector<Card> &Cardset,int health) throw()
{
	int do_damage = this->do_damage();
	int damage = p.fightTroll(do_damage,Cardset,this->get_health());
	this->get_hurt(damage);
}

