Decks_game:Card.o Deck.o main.o
	g++ Card.o Deck.o main.o -o Decks_game
	
Card.o: Card.cpp Card.h
	g++ -c Card.cpp
	
Deck.o: Deck.cpp Deck.h Card.h
	g++ -c Deck.cpp
	
main.o: main.cpp Card.h Deck.h
	g++ -c main.cpp

clean:
	rm -f *.o Decks_game
