
#include "Pokemon.h"


//helper function thats finds type by name and return the type:
Pokemon_type* find_type(char *type_name){
	Pokemon_type* temp_type = NULL;
	for (int i=0;i<type_num;i++){
		if (strcmp(all_types[i]->name,type_name) == 0){
			temp_type = all_types[i];
			break;}
	}return temp_type;
}

//helper function thats finds pokemon by name and return the pokemon:
Pokemon* find_pokemon(char *pokemon_name){
	Pokemon* temp_pokemon = NULL;
		for (int i=0;i<poke_num;i++){
			if (strcmp(pokemons[i]->name,pokemon_name) == 0){
				temp_pokemon = pokemons[i];
				break;}
		}return temp_pokemon;
}

//helper function for adding a type to the all_types list
//all_types list is the list that stores all the types in the program.
//(type num is a global value thats stores the number of types given when running the program.
int add_type(Pokemon_type* type){
	if (type == NULL){
		printf("Error :Function add_type Received a null pointer.\n");
		return 0;
	}
	//Iterate over types list
	for (int i=0;i<type_num;i++){
		//if found an empty place add type.
		if (all_types[i] == NULL){
			all_types[i] = type;
			break;
		}
	}return 1;
}

//helper function for adding a pokemon to the pokemons list
//pokemons list is the list that stores all the pokemons in the program.
//poke num is a global value thats stores the number of pokemons given when running the program.
int add_Pokemon(Pokemon* poke){
	if (poke == NULL){
		printf("Error :Function add_Pokemon Received a null pointer.\n");
		return 0;
	}
	//Iterate over pokemons list
	for (int i=0;i<poke_num;i++){
		//if found an empty place add pokemon.
		if (pokemons[i] == NULL){
			pokemons[i] = poke;
			break;
		}
	}return 1;
}

//helper function for adding/deleting to/from effective against me and other lists:
int check_if_in_array(Pokemon_type** array,int num,char *name){
	//first check if the list is empty, if empty return 1 (not in list)
	if (num == 0){
		return 1;}
	//if the list is not empty and valid try to find the type:
	for(int i=0;i<num;i++){
		if (strcmp(array[i]->name,name) == 0){
			return 0; //found.
		}
	}return 1;//not found
}

//helper function, finds length of string:
int find_len(char *string){
	int len = 0;
	//Iterate over the string
	while(*(string + len) != '\0'){
		len++;
	}return len++;

}


status print_pokemons_by_type(Pokemon_type* type){
	status result = success; //Initialize status value
	Pokemon* temp_pokemon = NULL; //initialize temp pokemon
	//Iterate over the pokemons list:
	for (int i=0;i<poke_num;i++){
		temp_pokemon = pokemons[i];
		if (strcmp(temp_pokemon->type->name,type->name) == 0){
			//if type name is matched print pokemon (and check if printing succeeded)
			result = print_pokemon(temp_pokemon);
			if (result == not_pointer){
				return not_pointer;
			}
		}
	}return success;
}

Pokemon_bio_info* make_bio(float height,float weight,int attack){
	//make new allocation for the bio
	Pokemon_bio_info* bio = (Pokemon_bio_info*)malloc(sizeof(Pokemon_bio_info));
	if (bio == NULL){
		return NULL; //return NULL to the make_pokemon function.
	}else{
		//if the allocation succeeded update members
		bio->height = height;
		bio->weight = weight;
		bio->attack = attack;
	return bio;} //return to the make_pokemon function.
}


status make_pokemon(char *name,char *species,float height,float weight,int attack,char *type){
	//use the helper function to find the length of the pokemon name:
	int len = find_len(name);
	//make new alloction for the name
	char *pokemon_name = (char*)malloc((len+1)*sizeof(char));
	// the allocation did'nt work stop now.
	if (pokemon_name == NULL){
		return failure;}
	strcpy(pokemon_name, name);
	//use the helper function to find the length of the species:
	len = find_len(species);
	char *pokemon_species = (char*)malloc((len+1)*sizeof(char));
	if (pokemon_species == NULL){
		// the allocation did'nt work stop now.
		return failure;}
	strcpy(pokemon_species,species);
	//find the type's pointer of the pokemon with the helper funcion:
	Pokemon_type* temp_type = find_type(type);
	if (temp_type == NULL){
		// the allocation did'nt work stop now.
		return failure;}
	//make new bio for the pokemon:
	Pokemon_bio_info *pokemon_bio = make_bio(height,weight,attack);
	if (pokemon_bio == NULL){
		// the allocation did'nt work stop now.
		return failure;}
	//finally make new allocation for the pokemon:
	Pokemon* pokemon = (Pokemon*)malloc(sizeof(Pokemon));
	//update members:
	pokemon->name = pokemon_name;
	pokemon->species = pokemon_species;
	pokemon->type = temp_type;
	pokemon->bio_info = pokemon_bio;
	temp_type->num_of_pokemons++; //update num pf pokemons for the type.
	//add to the pokemons list (the list thats stores all the pokemons in the program):
	add_Pokemon(pokemon);
	return success;
}

status make_pokemon_type(char *name){
	//use the helper function to find the length of the type name:
	int len = find_len(name);
	//make new allocation for the name (+1 for the '\0')
	char *type_name = (char*)malloc((len+1)*sizeof(char));
	if (type_name == NULL){ //if the malloc didn't work.
		return failure;}
	strcpy(type_name,name); //copy the name to the type name pointer.
	//make new allocation for the new type:
	Pokemon_type* type = (Pokemon_type*)malloc(sizeof(Pokemon_type));
	//update all the members:
	type->name = type_name;
	type->num_of_effective_against_me = 0;
	type->num_of_effective_against_others = 0;
	type->effective_against_me_list = NULL; //initialize to NULL so the list won't received an invalid value
	type->effective_against_others_list = NULL; //initialize to NULL so the list won't received an invalid value
	type->num_of_pokemons = 0;
	//add to the all_types list (the list thats stores all the types in the program):
	add_type(type);
	return success;
}






status add_to_eff_me(Pokemon_type* A,Pokemon_type* B){
	if (A == NULL || B == NULL){ //check if a pointer
		printf("Error: Function add_to_eff_me Received a null pointer.\n");
		return not_pointer;
	}else{
		if (A->num_of_effective_against_me == 0){ // if the list is empty make new allocation
			A->effective_against_me_list=(Pokemon_type**)malloc(sizeof(Pokemon_type*));
			if (A->effective_against_me_list==NULL){
				return failure; //allocation didn't work
			}else{
				A->effective_against_me_list[0] = B; //add to list
				A->num_of_effective_against_me++;
				return success;}
		}else{//the list isn't empty and need to do realloc
			//use the helper function to check if the type is already in the array:
			if (check_if_in_array(A->effective_against_me_list,A->num_of_effective_against_me,B->name) == 1){
				//make a temporary list to save all the previous data:
				Pokemon_type** prev = (Pokemon_type**)malloc(A->num_of_effective_against_me*sizeof(Pokemon_type*));
				if (prev == NULL){
					return failure;
				}else{
					for (int i=0;i<A->num_of_effective_against_me;i++){
						prev[i] = A->effective_against_me_list[i]; //store all data
					}
					int prev_num = A->num_of_effective_against_me;
					//update the allocation with realloc
					Pokemon_type** updated = (Pokemon_type**)realloc(A->effective_against_me_list,(A->num_of_effective_against_me + 1)*sizeof(Pokemon_type*));
					if (updated == NULL){ //the allocation didn't work
						free(prev);
						return failure;
					}else{//update the temp:
						for (int i=0;i<prev_num;i++){
							updated[i] = prev[i];
						}
						updated[prev_num] = B; //add the new type.
						A->num_of_effective_against_me++;
						A->effective_against_me_list = updated;
						free(prev);
						return success;}
				}
			}else{
				printf("This type already exist in the list.\n");
				return in_list;
			}
		}
	}
}



status add_to_eff_others(Pokemon_type* A,Pokemon_type* B){
	if (A == NULL || B == NULL){ //check if the function received a pointer
		printf("Error: Function add_to_eff_others Received a null pointer.\n");
		return not_pointer;
	}else{
		if (A->num_of_effective_against_others == 0){ // if the list is empty make new allocation
			A->effective_against_others_list=(Pokemon_type**)malloc(sizeof(Pokemon_type*));
			if (A->effective_against_others_list==NULL){
				return failure; //allocation didn't work
			}else{
				A->effective_against_others_list[0] = B; //add to list
				A->num_of_effective_against_others++;
				return success;}
		}else{//the list isn't empty and need to do realloc
			//use the helper function to check if the type is already in the array:
			if (check_if_in_array(A->effective_against_others_list,A->num_of_effective_against_others,B->name) == 1){
				//make a temporary list to save all the previous data:
				Pokemon_type** prev = (Pokemon_type**)malloc(A->num_of_effective_against_others*sizeof(Pokemon_type*));
				if (prev == NULL){
					return failure;
				}else{
					for (int i=0;i<A->num_of_effective_against_others;i++){
						prev[i] = A->effective_against_others_list[i]; //store all data
					}
					int prev_num = A->num_of_effective_against_others;
					//update the alloctaion with realloc
					Pokemon_type** updated = (Pokemon_type**)realloc(A->effective_against_others_list,(A->num_of_effective_against_others + 1)*sizeof(Pokemon_type*));
					if (updated == NULL){ //the alloction didn't work
						free(prev);
						return failure;
					}else{//update the temp:
						for (int i=0;i<prev_num;i++){
							updated[i] = prev[i];
						}
						updated[prev_num] = B; //add the new type.
						A->num_of_effective_against_others++;
						A->effective_against_others_list = updated;
						free(prev);
						return success;}
				}
			}else{
				printf("This type already exist in the list.\n");
				return in_list;
			}
		}
	}
}





status del_from_eff_me(Pokemon_type* A,char *name){
	if (A == NULL){//check if the function received a pointer
		printf("Error: Function del_from_eff_me Received a null pointer.\n");
		return not_pointer;
	}else{ //use the helper function to check if the type is in the array:
			if (check_if_in_array(A->effective_against_me_list,A->num_of_effective_against_me,name) == 0 ){
				if (A->num_of_effective_against_me == 1){
					free(A->effective_against_me_list); //the list will be empty.
					A->num_of_effective_against_me--; //update the num.
					A->effective_against_me_list = NULL; //to make sure the list does'nt point to a not valid memory space.
					return success;}
				//make a temporary list to save all the previous data:
				Pokemon_type** prev = (Pokemon_type**)malloc(A->num_of_effective_against_me*sizeof(Pokemon_type*));
				if (prev == NULL){ //if allocation didn't work
					return failure;
				}else{
					for (int i=0;i<A->num_of_effective_against_me;i++){
						prev[i] = A->effective_against_me_list[i]; //store all data
					}
					int prev_num = A->num_of_effective_against_me;
					//update the alloctaion with realloc:
						Pokemon_type** updated = (Pokemon_type**)realloc(A->effective_against_me_list, A->num_of_effective_against_me*sizeof(Pokemon_type*));
						if (updated == NULL){ //allocation didn't work
							free(prev);
							return failure;
						}else{
							int p_i = 0; //counter for previous num of types
							int n_i = 0; //couner for new num of types
							//update to the list the types that should remain and skip the deleted type.
							while(n_i < prev_num-1){ //until it reach the end of the list
								if (strcmp(prev[p_i]->name,name) != 0){
									//insert if the type isn't the one that needs to be deleted:
									updated[n_i] = prev[p_i];
									p_i++;
									n_i++;
								}else{
									p_i++;
								}
							}
							A->effective_against_me_list = updated;//update the pointer.
							A->num_of_effective_against_me-- ; //update the num
							free(prev);
							return success;
							}

				} //if the type does'nt in the list or the list is empty:
			}else printf("Type name doesn't exist in the list.\n");
			return not_in_list;
		}
}



status del_from_eff_others(Pokemon_type* A,char *name){
	if (A == NULL){//check if the function received a pointer
		printf("Error: Function del_from_eff_me Received a null pointer.\n");
		return not_pointer;
	}else{ //use the helper function to check if the type is in the array:
			if (check_if_in_array(A->effective_against_others_list,A->num_of_effective_against_others,name) == 0 ){
				if (A->num_of_effective_against_others == 1){
					free(A->effective_against_others_list); //the list will be empty.
					A->num_of_effective_against_others--; //update the num.
					A->effective_against_others_list = NULL; //to make sure the list does'nt point to a not valid memory space.
					return success;}
				//make a temporary list to save all the previous data:
				Pokemon_type** prev = (Pokemon_type**)malloc(A->num_of_effective_against_others*sizeof(Pokemon_type*));
				if (prev == NULL){ //if allocation didn't work
					return failure;
				}else{
					for (int i=0;i<A->num_of_effective_against_others;i++){
						prev[i] = A->effective_against_others_list[i]; //store all data
					}
					int prev_num = A->num_of_effective_against_others;
					//update the alloctaion with realloc:
						Pokemon_type** updated = (Pokemon_type**)realloc(A->effective_against_others_list, A->num_of_effective_against_others*sizeof(Pokemon_type*));
						if (updated == NULL){ //allocation didn't work
							free(prev);
							return failure;
						}else{
							int p_i = 0; //counter for previous num of types
							int n_i = 0; //couner for new num of types
							//update to the list the types that should remain and skip the deleted type.
							while(n_i < prev_num-1){ //until it reach the end of the list
								if (strcmp(prev[p_i]->name,name) != 0){
									//insert if the type isn't the one that needs to be deleted:
									updated[n_i] = prev[p_i];
									p_i++;
									n_i++;
								}else{
									p_i++;
								}
							}
							A->effective_against_others_list = updated;//update the pointer.
							A->num_of_effective_against_others-- ; //update the num
							free(prev);
							return success;
							}

				} //if the type does'nt in the list or the list is empty:
			}else printf("Type name doesn't exist in the list.\n");
			return not_in_list;
		}
}



status print_pokemon(Pokemon* pokemon){
	if ( pokemon != NULL ) {
		Pokemon_type* type = pokemon->type;
		Pokemon_bio_info* bio = pokemon->bio_info;
		printf("%s :\n",pokemon->name);
		printf("%s, %s Type.\n",pokemon->species, type->name);
		printf("Height: %.2f m    Weight: %.2f kg    Attack: %d\n\n", bio->height, bio->weight, bio->attack);
		return success;
	}//if the function received a not valid pointer:
	printf("Error: Function print_pokemon Received a null pointer.\n");
	return not_pointer;

};



status print_types(Pokemon_type* type){
	if ( type != NULL )
	{
		printf("Type %s -- %d pokemons\n",type->name, type->num_of_pokemons);
		int len_eff_me = type->num_of_effective_against_me;
		int len_eff_others = type->num_of_effective_against_others;
		if (len_eff_me != 0) { //if the list effective against me not empty:
			printf("\tThese types are super-effective against %s:",type->name);
			for (int i=0; i< len_eff_me ;i++) {
				//print types one by one.
				char *curr_type = type->effective_against_me_list[i]->name;
					if (i == len_eff_me - 1){
						// if we reach the end of the list:
						printf("%s\n",curr_type);
						//if it is still not the end:
					} else {printf("%s ,",curr_type);}
			}
		}
		if (len_eff_others != 0) {//if the list effective against others not empty:
			printf("\t%s moves are super-effective against:",type->name);
			for (int i=0; i< len_eff_others ;i++) {
				//print types one by one.
				char *curr_type = type->effective_against_others_list[i]->name;
				if (i == len_eff_others - 1){
					// if we reach the end of the list:
					printf("%s\n",curr_type);
					//if it is still not the end:
				} else {printf("%s ,",curr_type);}
			}
		}
		printf("\n");
		return success;
		//if the function received a not valid pointer:
	} else {printf("Error: Function print_types Received a null pointer.\n");
			return not_pointer;}
}



int clean_memory_pokemons(){
	if (pokemons != NULL){
		for (int i=0;i<poke_num;i++){
			//for every pokemon in the program:
			if (pokemons[i] != NULL){
				free(pokemons[i]->name);
				free(pokemons[i]->species);
				free(pokemons[i]->bio_info);
				free(pokemons[i]);}
		}
		//finally free the list that stores all the pokemons:
		free(pokemons);
	}
	return 1;
}



int clean_memory_types(){
	if (all_types != NULL){
		for (int i=0;i<type_num;i++){
			//for every type in the program:
			if (all_types[i] != NULL) {
			free(all_types[i]->name);
			free(all_types[i]->effective_against_me_list);
			free(all_types[i]->effective_against_others_list);
			free(all_types[i]);}
		}
		//finally free the list that stores all the types:
		free(all_types);
	}
	return 1;
}


