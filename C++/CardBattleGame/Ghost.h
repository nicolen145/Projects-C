
#ifndef GHOST_H_
#define GHOST_H_
#include "Enemy.h"
#include <vector>
using namespace std;

class Ghost:public Enemy
{
public:
	Ghost(int health,int damage):Enemy(health,damage) {}
	virtual ~Ghost() {};
	virtual void fight(Player &p,const vector<Card> &Cardset,int health)throw();

};


#endif /* GHOST_H_ */
