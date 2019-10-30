#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define MAX_NUM_FILE 100
#define MAX_DISTANCE 100
//written by LUOYOU ZHAO
int factorial(int n){
    int factorial=1;
    while(n!=1){
        factorial*=n;
        n--;
    }
    return factorial;
}

char **twoD_array(int length){
    char **new=malloc((length+1)*sizeof(char *));
    for(int i=0;i<length;i++){
        new[i]=malloc(100*sizeof(char));
    }
    new[length]=NULL;
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

void swap(int *num, int a, int b){
    int swap=num[b];
    num[b]=num[a];
    num[a]=swap;
}

void generate_position(int **position, int *num, int index, int length, int L[1]){
    if(index+1 == length){
        int *new=malloc(length*sizeof(int));
        for(int i=0;i<length;i++)
            new[i]=num[i];
        position[L[0]]=new;
        L[0]++;
        return;
    }

    for(int i=index;i<length;i++){
        swap(num,i,index);
        generate_position(position,num,index+1,length,L);
        swap(num,i,index);
    }
}

int check_words(char **wordslist,char *word){
    int length=strlength(wordslist);
    for(int i=0;i<length;i++){
        if(strcmp(wordslist[i],word)==0)    return 1;
    }
    return 0;
}

char **find_union(int argc, char *argv[]) {
    int str_length=0;
    char read[100];
    char **U=twoD_array(MAX_NUM_FILE);
    for(int i=1;i<argc;i++){
        FILE *file = fopen(argv[i],"r");
        while(fscanf(file, "%s", read) != EOF){
            if(!check_words(U,read)){
                char *new=malloc(strlen(read)*sizeof(char));
                strcpy(new,read);
                U[str_length]=new;
                str_length++;
            }
        }
        fclose(file);
    }
    U[str_length]=NULL;
    return U;
}

double find_index(char *word, char *filename){
    double index=0.0;
    char read[100];
    FILE *file = fopen(filename,"r");
    while(fscanf(file, "%s", read) != EOF){
        if(strcmp(read,word)==0){
            fclose(file);
            return index+1;
        }
        index++;
    }
    fclose(file);
    return -1;
}

float count_words(char *filename){
    char read[100];
    float str_length=0.0;
    FILE *file = fopen(filename,"r");
    while(fscanf(file, "%s", read) != EOF) str_length++;
    fclose(file);
    return str_length;
}

int find_int_index(int num, int *array, int length){
    for(int i=0;i<length;i++){
        if (array[i]==num)
            return i;
    }
    return -1;
}
//--------------------the following methods is for testing only--------------------
void print(int **position,int num){
    for(int i=0;i<factorial(num);i++){
        for(int j=0;j<num;j++)
            printf("%d ",position[i][j]);
        printf("\n");
    }
}
//--------------------------------------------------------------------------------------
int main (int argc, char *argv[]) {
    char **Union=find_union(argc, argv);
    float num=strlength(Union);
    int int_num=strlength(Union);
    int **position=malloc(factorial(num)*sizeof(int *));
    for(int i=0;i<factorial(num);i++){
        position[i]=malloc(num*sizeof(int));
    }
    int anum[int_num];
    for(int i=0;i<num;i++)  anum[i]=i+1;
    int L[1]={0};
    generate_position(position,anum,0,num,L);
    float smallest=MAX_DISTANCE;
    int smallest_index=0;
    for(int i=0;i<factorial(num);i++){
        float distance=0.0;
        for(int j=1;j<argc;j++){
            int index=0;
            for(int k=0;k<num;k++){
                if(find_index(Union[k],argv[j])==-1){
                    index++;//QAQ
                    continue;
                }
                float first=find_index(Union[k],argv[j])/count_words(argv[j]);
                float second=position[i][index]/num;
                distance+=fabs(first-second);
                index++;
            }
        }
        if(distance<smallest){
            smallest=distance;
            smallest_index=i;
        }
    }
    fprintf(stdout,"%.6f\n",smallest);
    for(int i=1;i<=num;i++){
        int index=find_int_index(i,position[smallest_index],int_num);
        fprintf(stdout,"%s\n",Union[index]);
    }
}