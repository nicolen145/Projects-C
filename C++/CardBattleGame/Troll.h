
#ifndef TROLL_H_
#define TROLL_H_

#include "Enemy.h"
#include <vector>
using namespace std;

class Troll:public Enemy
{
public:
	Troll(int health,int damage) :Enemy(health,damage) {}
	virtual ~Troll(){};
	virtual void fight(Player &p,const vector<Card> &Cardset,int health) throw();

};


#endif /* TROLL_H_ */
