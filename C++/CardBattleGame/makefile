Decks_game:Card.o Deck.o Player.o Troll.o Ghost.o Dragon.o Fighter.o Sorcerer.o Ranger.o Main.o
	g++ Card.o Deck.o Player.o Troll.o Ghost.o Dragon.o Fighter.o Sorcerer.o Ranger.o Main.o -o Decks_game 
	
Card.o: Card.cpp Card.h
	g++ -c Card.cpp
	
Deck.o: Deck.cpp Deck.h Card.h Errors.h
	g++ -c Deck.cpp
	
Player.o: Player.cpp Player.h Card.h
	g++ -c Player.cpp

Troll.o: Troll.cpp Troll.h Enemy.h
	g++ -c Troll.cpp

Ghost.o: Ghost.cpp Ghost.h Enemy.h
	g++ -c Ghost.cpp
	
Dragon.o: Dragon.cpp Dragon.h Enemy.h
	g++ -c Dragon.cpp

Fighter.o: Fighter.cpp Fighter.h Player.h
	g++ -c Fighter.cpp

Sorcerer.o: Sorcerer.cpp Sorcerer.h Player.h
	g++ -c Sorcerer.cpp
	
Ranger.o: Ranger.cpp Ranger.h Player.h
	g++ -c Ranger.cpp
	

Main.o: Main.cpp Card.h Enemy.h Deck.h Player.h Troll.h Ghost.h Dragon.h Fighter.h Sorcerer.h Ranger.h Errors.h
	g++ -c Main.cpp

clean:
	rm -f *.o Decks_game
