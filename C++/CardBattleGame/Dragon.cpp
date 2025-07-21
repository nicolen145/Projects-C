#include "Dragon.h"


class Enemy;

void Dragon::fight(Player &p,const vector<Card> &Cardset,int health) throw()
{
	int do_damage = this->do_damage();
	int damage = p.fightDragon(do_damage,Cardset,this->get_health());
	this->get_hurt(damage);
}

