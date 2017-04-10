#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node Node;

struct Node
{   int sz, label;
    Node *p, *pp, *l, *r;
};

void algoSelector(char* selector, int edge1, int edge2);

int findNumber(char *line,int start) {
    int i = 0;
    char *str = &line[2];

    while(!isspace(line[i])) i++;

    if (start == 1){
        str[i] = '\0';
        return atoi(str);
    }else{
        str += i;
        return atoi(str);
    }
}

void update(Node *x){
	x->sz = 1;
    if(x->l) x->sz += x->l->sz;
    if(x->r) x->sz += x->r->sz;
}

void rotr(Node *x) {
	Node *y, *z;
    y = x->p, z = y->p;
    if((y->l = x->r)) y->l->p = y;
    x->r = y, y->p = x;
    if((x->p = z)){
    	if(y == z->l) z->l = x;
        else z->r = x;
    }
    x->pp = y->pp;
    y->pp = 0;
    update(y);
}

void rotl(Node *x)
{   Node *y, *z;
    y = x->p, z = y->p;
    if((y->r = x->l)) y->r->p = y;
    x->l = y, y->p = x;
    if((x->p = z)){
    	if(y == z->l) z->l = x;
        else z->r = x;
    }
    x->pp = y->pp;
    y->pp = 0;
    update(y);
}

void splay(Node *x)
{   Node *y, *z;
    while(x->p){
    	y = x->p;
        if(y->p == 0){
        	if(x == y->l) rotr(x);
            else rotl(x);
        }
        else {
        	z = y->p;
            if(y == z->l){ 
            	if(x == y->l) rotr(y), rotr(x);
                else rotl(x), rotr(x);
            }
            else {
            	if(x == y->r) rotl(y), rotl(x);
                else rotr(x), rotl(x);
            }
        }
    }
    update(x);
}

Node *access(Node *x) {
    splay(x);
    if(x->r){
        x->r->pp = x;
        x->r->p = 0;
        x->r = 0;
        update(x);
    }

    Node *last = x;
    while(x->pp){
        Node *y = x->pp;
        last = y;
        splay(y);
        if(y->r){
            y->r->pp = y;
            y->r->p = 0;
        }
        y->r = x;
        x->p = y;
        x->pp = 0;
        update(y);
        splay(x);
    }
    return last;
}

Node *root(Node *x) {
	access(x);
    while(x->l) x = x->l;
    splay(x);
    return x;
}

void cut(Node *x){
	access(x);
    x->l->p = 0;
    x->l = 0;
    update(x);
}

void link(Node *x, Node *y){
	access(x);
    access(y);
    x->l = y;
    y->p = x;
    update(x);
}

Node *lca(Node *x, Node *y){
	access(x);
    return access(y);
}

int depth(Node *x){
	access(x);
    return x->sz - 1;
}

void connectedQ(Node *x, Node *y){
	if (root(x) == root(y)){
		printf("T\n");
	} else
		printf("F\n");
}

int main(int argc, char *argv[]) {

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

	    		edge2 = findNumber(line,2);
                edge1 = findNumber(line,1);
                algoSelector(comutador_link, edge1, edge2);

	    	} else if (0 == strncmp(&(line[i]),comutador_cut,size)) {

                edge2 = findNumber(line,2);
                edge1 = findNumber(line,1);
                algoSelector(comutador_cut, edge1, edge2);

	    	} else if (0 == strncmp(&(line[i]),comutador_connect,size)) {

	    		edge2 = findNumber(line,2);
                edge1 = findNumber(line,1);
                algoSelector(comutador_connect,edge1,edge2);

	    	} else if (0 == strncmp(&(line[i]),comutador_exit,size-1)) {
	    		algoSelector(comutador_exit, edge1,edge2);
	    	} else {
                printf("OLA\n");
	    	}
		}
	}

	fclose(stream);
	exit(EXIT_SUCCESS);
    return 0;
}

void algoSelector(char* selector, int edge1, int edge2){

	struct Node newNode1;
	newNode1.label = edge1;
	struct Node newNode2;
	newNode2.label = edge2;

	if (strcmp(selector,"L ") == 0) {
		link(&newNode1, &newNode2);
	} else if (strcmp(selector,"C ") == 0) {
		cut(&newNode1);
	} else if (strcmp(selector,"Q ") == 0) {
		connectedQ(&newNode1,&newNode2);
	}
}
