/*
 ============================================================================
 Name        : Pokedex.c
 Author      : nicole
 Version     :
 Copyright   :
 Description : Pokedex main program
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Pokemon.h"
#include "Defs.h"
#define buffer 300 //maximum length of chars in input line


Pokemon_type** all_types = NULL;
Pokemon** pokemons = NULL;
int type_num = 0; //needs to get the numOFTypes
int poke_num = 0; //needs to get the numOFPokemons


//function thats read from the file and save the data
status read_file(char *file_name){
	FILE* file;
	file = fopen(file_name,"r");
	if (file == NULL){
		printf("Invalid file path.\n");
		return failure;
		}else{
			//helper arguments:
			char line[buffer]; //maximum length of line.
			char *types_data[buffer]; //stores data on types
			char *pokemon_data[6];  //stores data on pokemons
			char *list_type[2] = {"effective-against-me", "effective-against-other"};
			int part = 0; //part 1 is types, part 2 is pokemons
			bool types_in_list = false;
			status result = success;
			char *token;
			int i;
			int len_data_types;
			char *delim = "\t,: \r\n"; //delimiters for the strtok() funcion
			while (fgets(line, sizeof(line), file)){
				if (strcmp(line,"Types\r\n") == 0){
					part++; //types part (1)
					continue;
				}
				if(part == 1 && types_in_list == false){
					//the types part where the func saving to all_types
					token = strtok(line,delim);
					while (token != NULL){
						result = make_pokemon_type(token);
						if (result == failure){
							printf("Memory Problem\n");
							clean_memory_pokemons();
							clean_memory_types();
							return failure;
						}else if (result == not_pointer){
							printf("the program encountered an error due to an invalid pointer - exiting program.");
							clean_memory_pokemons();
							clean_memory_types();
							}
						token = strtok(NULL,delim);
					}
					types_in_list = true; //after the types is saved
				}
				if (part == 1 && types_in_list == true && strcmp(line,"Pokemons\r\n") != 0 && (int)strlen(line) > 21){
					//the part where all the data is stores
					token = strtok(line,delim);
					i = 0;
					len_data_types = 0;
					while(token != NULL){
						//store all the data in list
						types_data[i] = token;
						i++;
						len_data_types++;
						token = strtok(NULL,delim);
					}
					Pokemon_type* type = find_type(types_data[0]);
					int j = 2;
					if (strcmp(types_data[1],list_type[0]) == 0){ //checks if need to be add to effective against me
						while (j < len_data_types){
							Pokemon_type* type_b = find_type(types_data[j]);
							result = add_to_eff_me(type,type_b);
							j++;
						}
						//if failed then its a memory problem, exit the program
							if (result == failure){
								printf("Memory Problem\n");
								clean_memory_pokemons();
								clean_memory_types();
								return failure;
							}else if (result == not_pointer){
								//if its a not valid pointer problem
								printf("the program encountered an error due to an invalid pointer - exiting program.");
								clean_memory_pokemons();
								clean_memory_types();
								return failure;
							}
					}else if (strcmp(types_data[1],list_type[1]) == 0){ //checks if need to be add to effective against others
						j = 2;
						while (j < len_data_types){
							Pokemon_type* type_b = find_type(types_data[j]);
							result = add_to_eff_others(type,type_b);
							j++;
						}//if failed then its a memory problem, exit the program
							if (result == failure){
								printf("Memory Problem\n");
								clean_memory_pokemons();
								clean_memory_types();
								return failure;
							}else if (result == not_pointer){
								//if its a not valid pointer problem
								printf("the program encountered an error due to an invalid pointer - exiting program.");
								clean_memory_pokemons();
								clean_memory_types();
								return failure;
							}

					}
				}
				if (strcmp(line,"Pokemons\r\n") == 0){
					part++; //part pokemons (2)
					continue;
				}
				if (part == 2 && types_in_list == true ){
					token = strtok(line,delim);
					i = 0;
					while (token != NULL) {
						//store pokemon data in array
						pokemon_data[i] = token;
						token = strtok(NULL, delim);
						i++;

					}
					//make pokemon after each array
					result = make_pokemon(pokemon_data[0],pokemon_data[1],(float)atof(pokemon_data[2]),(float)atof(pokemon_data[3]),(int)atoi(pokemon_data[4]),pokemon_data[5]);
					if (result == failure){
						//if failed then its a memory problem, exit the program
						printf("Memory Problem\n");
						clean_memory_pokemons();
						clean_memory_types();
						return failure;
					}
				}
			}
		}
	fclose(file);
	return success;

}


int main(int argc,char *argv[]){
	//save the program arguments:
	type_num = atoi((const char*)argv[1]);
	poke_num = atoi ((const char*)argv[2]);
	char *file_name = argv[3];
	 //make sure the program is clean:
	clean_memory_pokemons();
	clean_memory_types();
	//allocate memory for a new run
	all_types = calloc(type_num,sizeof(Pokemon_type*));
	pokemons = calloc(poke_num,sizeof(Pokemon*));
	//helper arguments:
	status file_result = success;
	status result = success;
	char type_name[buffer];
	char type_name_B[buffer];
	if (all_types == NULL || pokemons == NULL){
		//if allocation didn't work exit:
		printf("Memory Problem\n");
		clean_memory_pokemons();
		clean_memory_types();
		return 1;
	}else{
		file_result = read_file(file_name);
		if (file_result == failure){
			return 1;
		}else{
			char input;
			while (1){
				printf("Please choose one of the following numbers:\n");
				printf("1 : Print all Pokemons\n");
				printf("2 : Print all Pokemons types\n");
				printf("3 : Add type to effective against me list\n");
				printf("4 : Add type to effective against others list\n");
				printf("5 : Remove type from effective against me list\n");
				printf("6 : Remove type from effective against others list\n");
				printf("7 : Print Pokemon by name\n");
				printf("8 : Print Pokemons by type\n");
				printf("9 : Exit\n");
				scanf(" %c",&input);
				switch(input)
				{
					case '1': //print all pokemons (iterate over pokemons list):
						for(int i=0;i<poke_num;i++){
							print_pokemon(pokemons[i]);
						}
						break;
					case '2': //print all types (iterate over all_types list):
						for (int i = 0; i< type_num; i++){
							print_types(all_types[i]);
						}
						break;
					case '3'://add type to effective against me list
						printf("Please enter type name:\n");
						scanf("%s",type_name);
						if (find_type(type_name) != NULL){
							// if type A exist in the program:
							printf("Please enter type name to add to %s effective against me list:\n",type_name);
							scanf("%s",type_name_B);
							if (find_type(type_name_B) != NULL){
								//if type B exist in program
								Pokemon_type* A = find_type(type_name); //store type A
								Pokemon_type* B = find_type(type_name_B); //store type B
								result = add_to_eff_me(A,B);
								if (result == success){
									print_types(A);
								break;}
								else if (result == failure){
									printf("Memory Problem\n");
									clean_memory_pokemons();
									clean_memory_types();
									return 0;} //exit the program
								else if (result == not_pointer){
									printf("the program encountered an error due to an invalid pointer - exiting program.");
									clean_memory_pokemons();
									clean_memory_types();
									return 0;} //exit the program
								else {break;}
							}else{printf("Type name doesn't exist.\n");
						break;}
						}else{printf("Type name doesn't exist.\n");
							break;}
					case '4'://add type to effective against me list
						printf("Please enter type name:\n");
						scanf("%s",type_name);
						if (find_type(type_name) != NULL){
							// if type A exist in the program:
							printf("Please enter type name to add to %s effective against others list:\n",type_name);
							scanf("%s",type_name_B);
							if (find_type(type_name_B) != NULL){
								//if type B exist in program
								Pokemon_type* A = find_type(type_name); //store type A
								Pokemon_type* B = find_type(type_name_B); //store type B
								result = add_to_eff_others(A,B);
								if (result == success){
									print_types(A);
									break;}
								else if (result == failure){
									printf("Memory Problem\n");
									clean_memory_pokemons();
									clean_memory_types();
									return 0;} //exit the program
								else if (result == not_pointer){
									printf("the program encountered an error due to an invalid pointer - exiting program.");
									clean_memory_pokemons();
									clean_memory_types();
									return 0;} //exit the program
								else {break;}
							}else{printf("Type name doesn't exist.\n");
						break;}
						}else{printf("Type name doesn't exist.\n");
							break;}
					case '5': //remove type from effective against me list
						printf("Please enter type name:\n");
						scanf("%s",type_name);
						if (find_type(type_name) != NULL){
							// if type A exist in the program:
							printf("Please enter type name to remove from %s effective against me list:\n",type_name);
							scanf("%s",type_name_B);
							Pokemon_type* A = find_type(type_name);
							result = del_from_eff_me(A,type_name_B);
							//this function checks if type B is in the list and print a message if not
							if (result == success){
								//use print_types function to print
								print_types(A);
								break;}
							else if(result == failure ){
								printf("Memory Problem\n");
								clean_memory_pokemons();
								clean_memory_types();
								return 0;} //exit the program
							else if (result == not_pointer){
								printf("the program encountered an error due to an invalid pointer - exiting program.");
								clean_memory_pokemons();
								clean_memory_types();
								return 0;}//exit the program
							else {break;}
					}else{printf("Type name doesn't exist.\n");
					break;
					}
					case '6': //remove type from effective against others list
						printf("Please enter type name:\n");
						scanf("%s",type_name);
						if (find_type(type_name) != NULL){
							printf("Please enter type name to remove from %s effective against others list:\n",type_name);
							scanf("%s",type_name_B);
							Pokemon_type* A = find_type(type_name);
							result = del_from_eff_others(A,type_name_B);
							//this function checks if type B is in the list and print a message if not
							if (result == success){
								//use print_types function to print
								print_types(A);
							break;}
							else if(result == failure ){
								printf("Memory Problem\n");
								clean_memory_pokemons();
								clean_memory_types();
								return 0;} //exit the program
							else if (result == not_pointer){
								printf("the program encountered an error due to an invalid pointer - exiting program.");
								clean_memory_pokemons();
								clean_memory_types();
								return 0;}//exit the program
							else {break;}
					}else{printf("Type name doesn't exist.\n");
					break;
					}
					case '7'://print Pokemon by name
						printf("Please enter Pokemon name:\n");
						char pokemon_name[buffer]; //store the pokemon name
						scanf("%s",pokemon_name);
						Pokemon* pokemon = find_pokemon(pokemon_name); //find_pokemon returns a pointer to pokemon
						if (pokemon != NULL){
							//if found:
							result = print_pokemon(pokemon);
							if (result == not_pointer){
							//if pointer:
								printf("the program encountered an error due to an invalid pointer - exiting program.");
								clean_memory_pokemons();
								clean_memory_types();
								return 0;
							}
						break;
						}else{printf("The Pokemon doesn't exist.\n");
							break;}
					case '8': //print pokemons by type
						printf("Please enter type name:\n");
						char type_name_of_pokemon[buffer]; //store type
						scanf("%s",type_name_of_pokemon);
						Pokemon_type* type_of_pokemon = find_type(type_name_of_pokemon); //returns pointer to the type
						if (type_of_pokemon != NULL){
							if (type_of_pokemon->num_of_pokemons > 0){ //if there are pokemons with this type.
								printf("There are %d Pokemons with this type:\n",type_of_pokemon->num_of_pokemons);
								result = print_pokemons_by_type(type_of_pokemon);
								if (result == not_pointer){
									printf("the program encountered an error due to an invalid pointer - exiting program.");
									clean_memory_pokemons();
									clean_memory_types();
									return 0; //exit the program
								}
								break;}
							else{printf("There are no Pokemons with this type.\n");
								break;}
						}else{printf("Type name doesn't exist.\n");
						break;}
					case '9'://clean memory and exit
						printf("All the memory cleaned and the program is safely closed.\n");
						clean_memory_pokemons();
						clean_memory_types();
						return 0; //exit the program
					default: //if the user select a not valid number
						printf("Please choose a valid number.\n");
						continue;


				}
			}
		}
	}return 0;
}
