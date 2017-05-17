#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node *Node;

struct node{   
    int sz, label;
    struct node *p,*pp, *l, *r;
};

void update(Node x){
    x->sz = 1;
    if(x->l != NULL) {
        x->sz += x->l->sz;
    }
    if(x->r != NULL) {
        x->sz += x->r->sz;
    }
}

void rotr(Node x) {
    Node y, z;
    y = x->p, z = y->p;
    if((y->l = x->r) != NULL) {
        y->l->p = y;
    }
    x->r = y, y->p = x;
    if((x->p = z) != NULL){
        if(y == z->l) {
            z->l = x;
        } else{
            z->r = x;
        }
    }
    x->pp = y->pp;
    y->pp = NULL;
    update(y);
    x = y;
}

void rotl(Node x)
{   Node y, z;
    y = x->p, z = y->p;
    if((y->r = x->l) != NULL) {
        y->r->p = y;
    }
    x->l = y, y->p = x;
    if((x->p = z) != NULL){
        if(y == z->l){
            z->l = x;
        } else {
            z->r = x;
        }
    }
    x->pp = y->pp;
    y->pp = NULL;
    update(y);
    x = y;
}

void splay(Node x){
    Node y, z;
    while(x->p != NULL){
        y = x->p;
        if(y->p == NULL){
            if(x == y->l) {
                rotr(x);
            } else {
                rotl(x);
            }
        } else {
            z = y->p;
            if(y == z->l){ 
                if(x == y->l) {
                    rotr(y), rotr(x);
                } else {
                    rotl(x), rotr(x);
                }
            } else {
                if(x == y->r) {
                    rotl(y), rotl(x);
                } else {
                    rotr(x), rotl(x);
                }
            }
        }
    }
    update(x);
}

Node access(Node *x, int label) {
    splay(x[label]);
    if(x[label]->r != NULL){
        x[label]->r->pp = x[label];
        x[label]->r->p = NULL;
        x[label]->r = NULL;
        update(x[label]);
    }

    Node last = x[label];
    while(x[label]->pp != NULL){
        Node y = x[label]->pp;
        last = y;
        splay(y);
        if(y->r != NULL){
            y->r->pp = y;
            y->r->p = NULL;
        }
        y->r = x[label];
        x[label]->p = y;
        x[label]->pp = NULL;
        update(y);
        splay(x[label]);
    }
    return last;
}

Node flip(Node *x, int label) {
    splay(x[label]);
    if(x[label]->l != NULL){
        x[label]->l->pp = x[label];
        x[label]->l->p = NULL;
        x[label]->l = NULL;
        update(x[label]);
    }

    Node last = x[label];
    while(x[label]->pp != NULL){
        Node y = x[label]->pp;
        last = y;
        splay(y);
        if(y->r != NULL){
            y->r->pp = y;
            y->r->p = NULL;
        }
        y->l = x[label];
        x[label]->p = y;
        x[label]->pp = NULL;
        update(y);
        splay(x[label]);
    }
    return last;
}

Node reRoot(Node *x,int label){
    Node last = flip(x,label);
    while(last->l != NULL) {
        last = last->l;
    }
    splay(last);
    return last;    
}

Node root(Node *x,int label) {
    Node last = access(x,label);
    while(last->l != NULL) {
        last = last->l;
    }
    splay(last);
    return last;
}

Node cut(Node *x, Node *y, int edge1, int edge2){
    
    Node newRoot = reRoot(x,edge1);
    access(y,edge2);

    newRoot->p = NULL;
    y[edge2]->l = NULL;

    update(newRoot);
    update(y[edge2]);

    return newRoot;
}

void link(Node *x, Node *y, int edge1, int edge2){
    access(x,edge1);
    access(y,edge2);
    x[edge1]->l = y[edge2];
    y[edge2]->p = x[edge1];
    update(x[edge1]);
    update(y[edge2]);
}

int connectedQ(Node *x, Node *y,int edge1, int edge2){
    if (root(x,edge1)->label == root(y,edge2)->label){
        return 1;
    } else
        return -1;
}

Node lca(Node *x, Node *y, int edge1, int edge2){
    access(x,edge1);
    return access(y,edge2);
}

int depth(Node *x, int label){
    Node last = access(x,label);
    return last->sz - 1;
}

void algoSelector(Node *newNode, char* selector, int edge1, int edge2){

    if (strcmp(selector,"L ") == 0) {
        if(connectedQ(newNode,newNode,edge1,edge2) != 1) {
            link(newNode, newNode,edge1, edge2);
        }
    } else if (strcmp(selector,"C ") == 0) {
        if(connectedQ(newNode,newNode,edge1,edge2) == 1) {
            cut(newNode, newNode,edge1, edge2);
        }
    } else if (strcmp(selector,"Q ") == 0) {
        if (connectedQ(newNode,newNode,edge1,edge2) == 1){
            printf("T\n");
        } else
            printf("F\n");
    }
}

int findNumber(char *line,int start) {
    int i = 0;
    char *str = &line[2];

    while(!isspace(str[i])) i++;

    if (start == 1){
        str[i] = '\0';
        return atoi(str);
    }else{
        str += i;
        return atoi(str);
    }
}

int main(int argc, char *argv[]) {

    int i=0;
    int size = 2;
    int edge1 = 0;
    int edge2 = 0;
    FILE *stream;
    Node* Tree = (Node*) malloc(sizeof(Node));
    char line[10240];

    stream = stdin;
    if (stream == NULL) {
        printf("ERRO!!\n");
        exit(EXIT_FAILURE);
    }
    while ((fgets(line, sizeof line, stream)) != NULL) {

        char* comutador_link = "L ";
        char* comutador_cut = "C ";
        char* comutador_connect = "Q "; 
        char* comutador_exit = "X";
        
        if (0 == strncmp(&(line[i]),comutador_link,size)) {
            edge2 = findNumber(line,2) - 1;
            edge1 = findNumber(line,1) - 1;
            algoSelector(Tree,comutador_link, edge1, edge2);
        } else if (0 == strncmp(&(line[i]),comutador_cut,size)) {
            edge2 = findNumber(line,2) - 1;
            edge1 = findNumber(line,1) - 1;
            algoSelector(Tree, comutador_cut, edge1, edge2);
        } else if (0 == strncmp(&(line[i]),comutador_connect,size)) {
            edge2 = findNumber(line,2) - 1;
            edge1 = findNumber(line,1) - 1;
            algoSelector(Tree, comutador_connect,edge1,edge2);
        } else if (0 == strncmp(&(line[i]),comutador_exit,size-1)) {
            algoSelector(Tree, comutador_exit, edge1,edge2);
        } else {
            int k, n = atoi(line);
            Tree = (Node*) calloc(n,sizeof(Node));

            for(k = 0; k < n; k++ ) {
                Tree[k] = (Node) malloc(sizeof(struct node));
                Tree[k]->label = k+1;
                Tree[k]->sz = 0;
                Tree[k]->p = NULL;
                Tree[k]->pp = NULL;
                Tree[k]->l = NULL;
                Tree[k]->r = NULL;
                update(Tree[k]);
            }
        }
    }
    fclose(stream);
    free(Tree);
    exit(EXIT_SUCCESS);
    return 0;
}
