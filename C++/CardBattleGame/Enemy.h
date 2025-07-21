
#ifndef ENEMY_H_
#define ENEMY_H_
#include <vector>
#include "Card.h"
#include "Player.h"
using namespace std;

class Enemy
{
	int health_points;
	const int damage_points;
public:
	Enemy(int health,int damage): health_points(health), damage_points(damage) {}
	virtual ~Enemy() {};
	virtual void fight(Player &p,const vector<Card> &Cardset,int health) throw() =0;
	bool is_defeated()const throw() {return health_points <= 0;}
	void get_hurt(int damage) throw(){health_points -= damage;}
	int do_damage()const throw(){return damage_points;}
	int get_health()const throw(){return health_points;}

};

#endif /* ENEMY_H_ */
