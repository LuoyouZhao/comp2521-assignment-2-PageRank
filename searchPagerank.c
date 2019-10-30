#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define MAX_NUM_FILE 100
//written by LUOYOU ZHAO
//the method strlength() is copied from my first Assignment
int strlength(char **str){
    int length=0;
    while(str[length]!= NULL){
        length++;
    }
    return length;
}

char **twoD_array(int length){
    char **new=malloc((length+1)*sizeof(char *));
    for(int i=0;i<length;i++){
        new[i]=malloc(100*sizeof(char));
    }
    new[length]=NULL;
    return new;    
}

int check_words(char **wordslist,char *word){
    int length=strlength(wordslist);
    for(int i=0;i<length;i++){
        if(strcmp(wordslist[i],word)==0)    return 1;
    }
    return 0;
}

void find_words(char **wordslist,char **finded_words){
    int length=0;
    char read[100];
    FILE *file = fopen("invertedIndex.txt","r");
    while(fscanf(file, "%s", read) != EOF){
        if(check_words(wordslist,read)){
            char line[1000];
            fgets(line, 1000, file);
            char *temp=strtok(line, " ");
            while(temp){
                if(!check_words(finded_words,temp)&&strcmp(temp,"\n")!=0){
                    char *new=malloc(strlen(temp)*sizeof(char));
                    strcpy(new,temp);
                    finded_words[length]=new;
                    length++;
                }
                temp=strtok(NULL," ");
            }                 
        }else{
            fscanf(file,"%*[^\n]");
        }
    }
    fclose(file);
    finded_words[length]=NULL;
}

int main (int argc, char *argv[]) {
    char **wordslist=twoD_array(argc-1);
    for(int i=0;i<argc-1;i++){
        strcpy(wordslist[i],argv[i+1]);
    }
    char **finded_words=twoD_array(MAX_NUM_FILE);
    find_words(wordslist,finded_words);
    FILE *file = fopen("pagerankList.txt","r");
    char read[100];
    while(fscanf(file, "%s", read) != EOF){
        read[strlen(read)-1]='\0';
        if(check_words(finded_words,read)){
            fprintf(stdout,"%s\n",read);
        }
        fscanf(file,"%*[^\n]");
    }
    fclose(file);
}