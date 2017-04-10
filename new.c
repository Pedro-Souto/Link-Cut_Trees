#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int *readNumbers(char *line){
	int l = strlen(line);
	int *array = malloc(sizeof(int)*l);

 	for(int i = 0; i < l; i++){
		array[i] = line[i]-'0';
		printf("%i\t",array[i]);
 	}
 	printf("\n");
 	return array;
}

int findSpace(char *line) {
	int i = 0;

	while(!isspace(line[i])) i++;

	return i+1;
}

int main(int argc, char const *argv[]) {

	int edge1 = 0;
	int edge2 = 0;
	FILE *stream;
	char line[10240];

	stream = stdin;
	if (stream == NULL) {
		printf("ERRO!!\n");
	    exit(EXIT_FAILURE);
	}
	while ((fgets(line, sizeof line, stream)) != NULL) {

	    int i;

	    for (i = 0; i < strlen(line); i++) {

	    	int size = 2;
	    	
	    	char* comutador_link = "L ";
	    	char* comutador_cut = "C ";
	    	char* comutador_connect = "Q "; 
	    	char* comutador_exit = "X";
			
	    	if (0 == strncmp(&(line[i]),comutador_link,size)) {

	    		int iterator = findSpace(line);
	    		edge1 = readNumbers(&line[iterator])[0];
	    		edge2 = readNumbers(&line[iterator])[3];
	    		printf("%i\t%i\n", edge1, edge2);
	    		break;

	    	} else if (0 == strncmp(&(line[i]),comutador_cut,size)) {

	    		printf("TOP2\n");
	    		break;

	    	} else if (0 == strncmp(&(line[i]),comutador_connect,size)) {

	    		printf("TOP3\n");
	    		break;

	    	} else if (0 == strncmp(&(line[i]),comutador_exit,size-1)) {
	    		printf("TOP4\n");
	    		break;
	    	} else {
	    		printf("TOP5\n");
	    		break;
	    	}
		}
	}

	fclose(stream);
	exit(EXIT_SUCCESS);
    return 0;
}
