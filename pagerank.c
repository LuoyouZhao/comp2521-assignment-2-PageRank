#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
//written by LUOYOU ZHAO
typedef struct Node{
    char *url;
    struct Node *next;
}Node;

typedef struct graphRep{
    Node **edge;
    Node **inedge;
    char **indexlist;
    int nV;
}graphRep;

typedef struct graphRep *Graph;

char **twoD_array(int length){
    char **new=malloc((length+1)*sizeof(char *));
    for(int i=0;i<length;i++){
        new[i]=malloc(100*sizeof(char));
    }
    new[length]=NULL;
    return new;    
}

struct Node *new_Node(char *url){
    Node *node=malloc(sizeof(struct Node));
    char *urls=malloc(strlen(url)*sizeof(char));
    strcpy(urls,url);
    node->url=urls;
    node->next=NULL;
    return node;
}

struct Node * add_node(struct Node *node,char *url){
    struct Node *new_node=new_Node(url);
    if(node==NULL){
        return new_node;
    }
    struct Node *backup=node;
    while(backup->next!=NULL){
        backup=backup->next;
    }
    backup->next=new_node;
    return node;
}

Graph new_Graph(int nV){
    Graph new=malloc(sizeof(struct graphRep));
    new->nV=nV;
    new->indexlist=twoD_array(nV);
    new->edge=malloc(nV*sizeof(struct Node *));
    new->inedge=malloc(nV*sizeof(struct Node *));
    return new;
}
// the method strlength() is copied from my first Assignment
int strlength(char **str){
    int length=0;
    while(str[length]!= NULL){
        length++;
    }
    return length;
}

int get_index(Graph g,char *url){
    for(int i=0;i<strlength(g->indexlist);i++){
        if(strcmp(g->indexlist[i],url)==0)  return i;
    }
    printf("did't find the index of %s\n",url);
    return -1;
}

int count_words(char *article){
    char read[100];
    int str_length=0;
    FILE *file = fopen("collection.txt","r");
    while(fscanf(file, "%s", read) != EOF) str_length++;
    fclose(file);
    return str_length;
}

void read_urls(Graph g, char **urls){
    for(int i=0;i<strlength(urls);i++){
        char read[100];
        char *backup=malloc(strlen(urls[i])*sizeof(char));
        strcpy(backup,urls[i]);
        strcat(backup,".txt");
        FILE *file = fopen(backup,"r");
        fscanf(file, "%s", read);
        while(strcmp(read,"Section-1")!=0) fscanf(file, "%s", read);
        fscanf(file, "%s", read);
        while(strcmp(read,"#end")!=0){
            if(strcmp(read,g->indexlist[i])!=0){
                g->edge[i]=add_node(g->edge[i],read);
                g->inedge[get_index(g,read)]=add_node(g->inedge[get_index(g,read)],g->indexlist[i]);
            }
            fscanf(file, "%s", read);
        }
        fclose(file);
    }
}
float getVout(Graph g, int v){
    int length=0;
    struct Node *node=g->edge[v];
    while(node!=NULL){
        length++;
        node=node->next;
    }
    if (length==0){
        return 0.5;
    }else{
        return length;
    }
}

float getVin(Graph g, int v){
    int length=0;
    struct Node *node=g->inedge[v];
    while(node!=NULL){
        if(strcmp(node->url,g->indexlist[v])!=0) length++;
        node=node->next;
    }
    return length;
}

float getWout(Graph g, int v, int u){
    float uOut=getVout(g,u);
    struct Node *node=g->edge[v];
    float sumOut=0;
    while(node!=NULL){
        sumOut+=getVout(g,get_index(g,node->url));
        node=node->next;
    }
    return(uOut/sumOut);
}

float getWin(Graph g, int v, int u){
    float uIn=getVin(g,u);
    struct Node *node=g->edge[v];
    float sumIn=0;
    while(node!=NULL){
        sumIn+=getVin(g,get_index(g,node->url));
        node=node->next;
    }
    return(uIn/sumIn);
}

float **pr(Graph g,float d,int maxInterations){
    float **matrix=malloc(g->nV*sizeof(float *));
    for(int i=0;i<g->nV;i++){
        matrix[i]=malloc(maxInterations*sizeof(float));
        matrix[i][0]=1.0/g->nV;
    }
    for(int j=1;j<maxInterations;j++){
        for(int i=0;i<g->nV;i++){
            struct Node *node=g->inedge[i];
            float sum=0;
            while(node!=NULL){
                int k=get_index(g,node->url);
                sum+=matrix[k][j-1]*getWin(g,k,i)*getWout(g,k,i);
                node=node->next;
            }
            matrix[i][j] = (float)((1-d)/(g->nV))+d*sum;
        }
    }  
    return matrix;
}

float *PageRank(Graph g,float d, float diffPR,int maxInterations){
    float *output=malloc(g->nV*sizeof(float));
    float **PR=pr(g,d,maxInterations);
    int interation=0;
    float diff=diffPR;
    while(interation<maxInterations&&diff>=diffPR){
        diff=0;
        for(int i=0;i<g->nV;i++){
            diff+=fabs(PR[i][interation+1]-PR[i][interation]);
        }
        interation++;
    }
    for(int i=0;i<g->nV;i++){
        output[i]=PR[i][interation];
    }
    return output;
}

void file_print(char *filename,Graph g,float *pr){
	FILE *file = fopen(filename, "w");
	if( file == NULL ) { 
		printf("Error: cannot open file : %s \n", filename);
		return;
	}
    for(int i=0;i<g->nV;i++){
        int max=i;
        for(int j=0;j<g->nV;j++){
            if(pr[j]>pr[max]){
                max=j;
            }
        }
        fprintf(file, "%s, %.0f, %.7f\n", g->indexlist[max],getVout(g,max),pr[max]);
        pr[max]=-100000.0;
    }
	fclose(file);
}
//--------------------the following methods is for testing only------------------------------
void printGraph(Graph g){
    printf("this graph has %d vertix\n----------------out---------------\n",g->nV);
    for(int i=0;i<strlength(g->indexlist);i++){
        printf("the vertix %d is %s:\n",i,g->indexlist[i]);
        struct Node *node=g->edge[i];
        if(node==NULL){
            printf("  -->nothing\n");
            continue;
        }
        while(node!=NULL){
            printf("  -->%s\n",node->url);
            node=node->next;
        }
    }
    printf("\n----------------in---------------\n");
    for(int i=0;i<strlength(g->indexlist);i++){
        printf("the vertix %d is %s:\n",i,g->indexlist[i]);
        struct Node *node=g->inedge[i];
        if(node==NULL){
            printf("  -->nothing\n");
            continue;
        }
        while(node!=NULL){
            printf("  -->%s\n",node->url);
            node=node->next;
        }
    }
}
//------------------------------------------------------------------------------------------
int main (int argc, char *argv[]) {
    char **urls=twoD_array(count_words("collection.txt"));
    Graph g=new_Graph(count_words("collection.txt"));
    int count=0;
    FILE *file = fopen("collection.txt","r");
    while(fscanf(file, "%s", urls[count]) != EOF){
        g->indexlist[count]=urls[count];
        count++;
    }
    fclose(file);
    read_urls(g, urls);
    float *pr=PageRank(g,atof(argv[1]), atof(argv[2]),atoi(argv[3]));
    file_print("pagerankList.txt",g,pr);
}