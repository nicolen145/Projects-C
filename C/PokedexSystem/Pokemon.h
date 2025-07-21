

#ifndef POKEMON_H_
#define POKEMON_H_


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "Defs.h"


//the structs:

typedef struct Pokemon_type {
    char* name;
    int num_of_pokemons;
    struct Pokemon_type** effective_against_me_list;
    int num_of_effective_against_me;
    struct Pokemon_type** effective_against_others_list;
    int num_of_effective_against_others;
} Pokemon_type;



typedef struct {

	//of one pokemon
	float height;
	float weight;
	int attack;

}Pokemon_bio_info;



typedef struct{

	char* name;
	char* species;
	Pokemon_type* type;
	Pokemon_bio_info* bio_info;

} Pokemon ;


//global variables of the program:
extern Pokemon_type** all_types;
extern Pokemon** pokemons;
extern int type_num;
extern int poke_num;


//declaration of the functions:
Pokemon_type* find_type(char *type_name);
Pokemon* find_pokemon(char *poke_name);
int add_type(Pokemon_type* type);
int add_Pokemon(Pokemon* poke);
int check_if_in_array(Pokemon_type** array,int num,char* name);
int find_len(char string[]);
status print_pokemons_by_type(Pokemon_type* type);
Pokemon_bio_info* make_bio(float height,float weight,int attack);
status make_pokemon(char *name,char *species,float height,float weight,int attack,char *type);
status make_pokemon_type(char *name);
status add_to_eff_me(Pokemon_type* A,Pokemon_type* B);
status add_to_eff_others(Pokemon_type* A,Pokemon_type* B);
status del_from_eff_me(Pokemon_type* A,char *name);
status del_from_eff_others(Pokemon_type* A,char *name);
status print_pokemon(Pokemon* pokemon);
status print_types(Pokemon_type* type);
int clean_memory_pokemons();
int clean_memory_types();



#endif /* POKEMON_H_ */
