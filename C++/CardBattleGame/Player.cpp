
#include "Player.h"

void Player::add_health(int health) throw()
{
	int temp_health = health_points + health;
	if (temp_health >= max_health) {temp_health = max_health;}
	if (temp_health - health_points > 0)
	{
		cout << "Player healed " << temp_health - health_points << " points of damage" << endl;
	}
	health_points = temp_health;
}

int Player::set_total_value(const vector<Card> &Cardset)const throw()
{
	int value = 0;
	for (size_t i = 0; i < Cardset.size(); ++i) {
		value += Cardset[i].get_value();
	}
	return value;
}

bool Player::set_has_sign(const vector<Card> &Cardset,char sign)const throw()
{
	for (size_t i = 0; i < Cardset.size(); ++i) {
		if (Cardset[i].get_symbol() == sign) {
			return true;}
	}
	return false;
}

void Player::get_hurt(int damage) throw()
{
	health_points -= damage;
	cout << "Player took "<< damage <<" points of damage" <<endl;
}

int Player::without_D_affect(int damage,const vector<Card> &CardsSet,int enemy_health) throw()
{
	int total = set_total_value(CardsSet);
	if (set_has_sign(CardsSet,'S') && set_has_sign(CardsSet,'C') ) {total = total*2;}
	cout << "Player dealt "<< total <<" points of damage" << endl;
	if (set_has_sign(CardsSet,'H')) {add_health(total);}
	if (total >= enemy_health) {return total;}
	get_hurt(damage);
	return total;
}

int Player::without_H_affect(int damage,const vector<Card> &CardsSet,int enemy_health) throw()
{
	int total = set_total_value(CardsSet);
	if (set_has_sign(CardsSet,'S') && set_has_sign(CardsSet,'C') ) {total = total*2;}
	if (set_has_sign(CardsSet,'D')) {damage-=total;}
	if (damage <= 0) {damage = 0;}
	cout << "Player dealt "<< total <<" points of damage" << endl;
	if (total >= enemy_health) {return total;}
	get_hurt(damage);
	return total;
}

int Player::without_S_C_affect(int damage,const vector<Card> &CardsSet,int enemy_health) throw()
{
	int total = set_total_value(CardsSet);
	if (set_has_sign(CardsSet,'D')) {damage-=total;}
	if (damage <= 0) {damage = 0;}
	cout << "Player dealt "<< total <<" points of damage" << endl;
	if (set_has_sign(CardsSet,'H')) {add_health(total);}
	if (total >= enemy_health) {return total;}
	get_hurt(damage);
	return total;
}
