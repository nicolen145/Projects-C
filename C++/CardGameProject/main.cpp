
#include "Deck.h"
#include "Card.h"
#include <iostream>
using namespace std;

int main() {
	const int max = 100; //max chars in input
	Deck *deck = nullptr; //Unseals the deck to null
	int choice;
	bool result = false;
	int input = 0;
	char input_str[max]; //array of chars for input
	char inputs[3][max]; //inputs array
	Card card1,card2;
	Card** cards = nullptr;
	int cards_num;
	Deck *temp_deck;
	while (1)
	{
		cout << "Menu:" << endl;
		cout << "1. Build Empty Deck" << endl;
		cout << "2. Compare Cards" << endl;
		cout << "3. Add Cards" << endl;
		cout << "4. Remove Cards" << endl;
		cout << "5. Look at Top Cards" << endl;
		cout << "6. Card at" << endl;
		cout << "7. Swap Card" << endl;
		cout << "8. Print Deck" << endl;
		cout << "9. Delete Deck" << endl;
		cout << "10. Exit" << endl;
		cin >> choice;
		switch(choice)
		{
			case 1: // Build Empty Deck
				if (deck != nullptr) {cout << "Deck already exists" << endl;}
				else {deck = new Deck;}//make new deck:
				break;
			case 2: //Compare Cards
				for (int i = 0; i < 3; i++){
					cin >> input_str;
					strcpy(inputs[i],input_str);
				}
				card1.set_rank(inputs[0][0]); card1.set_symbol(inputs[0][1]);
				card2.set_rank(inputs[1][0]); card2.set_symbol(inputs[1][1]);
				result = card1.compere(card2,inputs[2]);
				if (result == true) {cout << "True" << endl;}
				else {cout << "False" << endl;}
				break;
			case 3: //Add Cards
				cin >> input_str;
				cards_num = stoi(input_str);
				if (cards_num < 0) {cout << "Invalid number" << endl; break;}
				cards = new Card*[cards_num];
				if (!cards) {cout << "Memory Problem" << endl; delete deck; return 0;}
				//make new cards
				for (int i = stoi(input_str)-1 ; i >= 0 ; i--){
					cin >> input_str;
					Card *new_card;
					new_card = new Card(input_str[0],input_str[1]);
					cards[i] = new_card;
				}
				//make temp deck for adding
				temp_deck = new Deck;
				temp_deck->set_deck(cards,cards_num );
				result = *deck += *temp_deck;
				delete temp_deck;
				temp_deck = nullptr;
				if (!result) {cout << "Memory Problem" << endl; delete deck; return 0;}
				break;
			case 4: //Remove Cards
				cin >> input_str;
				input = stoi(input_str); //the valid input checking happens inside the -= operator
				result = *deck -= input;
				if (result == false) {cout << "Memory Problem" << endl; delete deck; return 0;}
				break;
			case 5: //Look at Top Cards
				cin >> input_str;
				deck->peek_X(stoi(input_str)); //the valid input checking happens inside the function
				break;
			case 6: //Card at
				cin >> input_str;
				input = stoi(input_str);
				//check if card is valid (using if_valid_card function the uses the static card Invalid_Card)
				if (deck->if_valid_card((*deck)[input]) == false) {break;}
				cout << (*deck)[input] << endl;
				break;
			case 7: //Swap Card
				for (int i = 0; i < 2; i++){
					cin >> input_str;
					strcpy(inputs[i],input_str);
				}
				input = stoi(inputs[0]);//the valid input checking happens inside the function
				card1.set_rank(inputs[1][0]); card1.set_symbol(inputs[1][1]);
				deck->swap_cards(card1,input);
				break;
			case 8: //Print Deck
				cout << *deck << endl;
				break;
			case 9: //Delete Deck
				if (deck != nullptr) {
					delete deck;
					deck = nullptr;}
				break;
			case 10: //Exit
				if (deck != nullptr) {delete deck;} //if the deletion in case 9 was not called
				return 0;
			default:
				cout << "Invalid choice" << endl;
				continue;
		}
	}return 0;
}
