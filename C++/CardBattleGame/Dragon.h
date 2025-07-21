
#ifndef DRAGON_H_
#define DRAGON_H_

#include "Enemy.h"
#include <vector>
using namespace std;

class Dragon:public Enemy
{
public:
	Dragon(int health,int damage) :Enemy(health,damage) {}
	virtual ~Dragon() {};
	virtual void fight(Player &p,const vector<Card> &Cardset,int health) throw();

};


#endif /* DRAGON_H_ */
