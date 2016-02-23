/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   first.c
 * Author: Kotadia
 *
 * Created on December 4, 2015, 11:03 PM
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#ifndef NULL
#define NULL   ((void *) 0)
#endif
struct trienode{
    char letter;
    int isword;
    int numoccur;
    struct trienode* children[26];
    int superwords;
    int prefixes;
};
struct trienode* root = NULL;
struct trienode* makenode(){
    
    struct trienode *pNode;
    pNode = (struct trienode*)malloc(sizeof(struct trienode));
    
    if( pNode ){
        int i;
        pNode->letter = '\0';
        pNode->numoccur = 0;
        pNode->superwords = -1;
        pNode->prefixes = -1;
        pNode->isword = 0; /*0 represents not a word as yet*/
        
        for(i = 0; i < 26; i++){
            pNode->children[i] = NULL;
        }
    }
    
    return pNode;
}
struct trienode* insert(struct trienode* root, char key[]){
    int length = strlen(key);
    struct trienode* ptr;
    struct trienode* ptr1;
    int index;
    int level;
    char alphabet;
    
    if(root == NULL){
        root = makenode();
        root->isword = -1; /*-1 represents root node*/
    }
    
    ptr = root;
    
    for(level = 0; level < length; level++){
        alphabet = (char)tolower(key[level]);
        index = (char)tolower(key[level]) - 'a';
        
        if(ptr->children[index] == NULL){
            ptr->children[index] = makenode();
        }
        
        ptr = ptr->children[index];
        ptr->letter = alphabet;
    }
   
    ptr->isword = 1; /*word inserted*/
    ptr->numoccur = 0;
    ptr->superwords = 0;
    ptr->prefixes = 0;
    ptr1 = root;
    return ptr1;
}

void traversedown(struct trienode* ptr){
    if(ptr == NULL){
        return;
    }
    
    if(ptr->isword == 1){
        ptr->prefixes++;
    }
    
    int all;
    for(all = 0; all < 26; all++){
        traversedown(ptr->children[all]);
    }
}

void searchandupdate(struct trienode* root, char key[]){
    int length = strlen(key);
    struct trienode* ptr;
    int index;
    int level;
    
    
    ptr = root;
    
    if(ptr == NULL){
        return;
    }
    
    for(level = 0; level < length; level++){
        index = (char)tolower(key[level]) - 'a';
        
		/*if(ptr->isword == 1 && level < length){ level gives length of word found which is a prefix
            ptr->superwords++;
            numprefixes = numprefixes + ptr->numoccur;
        }*/
		
        if(ptr->children[index] == NULL){
            return; /*value not present, failure*/
        }
        
        ptr = ptr->children[index];
    }
    
    /*code reaches here only if key is present*/
    
        ptr->numoccur++;
        /*if(ptr->prefixes > numprefixes){
            ptr->prefixes = (ptr->prefixes - numprefixes) + numprefixes;
        }
        else{
            ptr->prefixes = (numprefixes - ptr->prefixes) + numprefixes;
        }
    
    all the counts affected because of key PRIOR to this point are updated by now, now update counts BEYOND this point*/
    
    /*for(count = 0; count < 26; count++){
        traversedown(ptr->children[count]);
    }*/
    return; /*success*/
}

void readDict(FILE *dict_file){
    
    
    char letter; int init = 0;
    char word[100];
    
    for(init = 0; init < 100; init++){
        word[init] = '\0';
    }
    
    int F = fscanf(dict_file,"%c",&letter);
    int i = 0;
    
    while(1){
        
        if(F == EOF){
            break;
        }
        
        if(isalpha(letter) != 0){ /*letter is an alphabet*/
            word[i] = letter;
            i++;
        }
        else{
            if(i > 0){
                root = insert(root, word);
                
                for(init = 0; init < 100; init++){
                    word[init] = '\0';
                }
                i = 0;
            }
        }
        
        F = fscanf(dict_file,"%c",&letter);
        
    }
    
    
    
}
void matchStr(char str[]){
    searchandupdate(root, str);
}
void readData(FILE *data_file){
    
    /*FILE* file;*/
    /*file = fopen(data_file, "r");*/
    
    char letter; int init = 0;
    char word[100];
    
    for(init = 0; init < 100; init++){
        word[init] = '\0';
    }
    
    int F = fscanf(data_file,"%c",&letter);
    int i = 0;
    
    while(1){
        
        if(F == EOF){
            break;
        }
        
        if(isalpha(letter) != 0){
            word[i] = letter;
            i++;
        }
        else{
            if(i > 0){
                matchStr(word);
                
                for(init = 0; init < 100; init++){
                    word[init] = '\0';
                }
                i = 0;
            }
            
        }
        
        F = fscanf(data_file,"%c",&letter);
        
    }
    
}
void printResulthelp(int index, char word[100], FILE *outputfile, struct trienode* ptr, int numprefixes){
    
	
    
	if(ptr == NULL){
        return;
    }
    
    
		
        word[index] = ptr->letter;
		ptr->prefixes = numprefixes;
		numprefixes = numprefixes + ptr->numoccur;
    
    
    if(ptr->isword == 1){
        
        int i = 0;
        for(i = index+1; i< 100; i++){
            word[i] = '\0';
        }
        
        fprintf(outputfile, "%s %d %d\n", word, ptr->numoccur, ptr->prefixes);
    }
    
    int k;
    for(k = 0; k< 26; k++){
        printResulthelp((index+1), word, outputfile, ptr->children[k], numprefixes);
    }
    
}

/*void printResult(FILE *output){
    char wordarr[100];
    int numprefixes;
    
    printResulthelp(-1, wordarr, output, root, numprefixes);
}*/

struct trienode* freetrie(struct trienode* root){
	int i;
	if(root == NULL){
            return NULL;
	}
	
	for(i = 0; i< 26; i++){
		if(root->children[i] != NULL){
			root->children[i] = freetrie(root->children[i]);
		}
	}
	
	free(root);
	return NULL;
}

int main(int argc, char** argv){
    
    FILE* mapfile; FILE* ofp; FILE* ifp1; FILE* ifp2;
    char dictfile[100]; char datafile[100];
    char outputName[11];
	char wordarr[100];
    int numprefixes;
	int k;
    
    int filecount = 1; int i;
    
    if(argc != 2){ /*filename not given*/
        printf("invalid input\n");
        return 0;
    }
    
    mapfile = fopen(argv[1], "r");
    if(mapfile == NULL){ /*file does not exist*/
        printf("invalid input\n");
        return 0;
    }
    
    while(fscanf(mapfile, "%s %s", dictfile, datafile) == 2){
        sprintf(outputName,"out%d.txt", filecount);
        ifp1 = fopen(dictfile, "r");
        ifp2 = fopen(datafile, "r");
        readDict(ifp1);
        readData(ifp2);
        ofp = fopen(outputName, "w");
		
		
		for(k = 0; k< 26; k++){
			numprefixes = 0;
			printResulthelp(0, wordarr, ofp, root->children[k], numprefixes);
		}
		
        /*printResult(ofp);*/
        fclose(ofp);
        fclose(ifp1);
        fclose(ifp2);
        filecount++;
        
        for(i = 0; i< 11; i++){
            outputName[i] = '\0';
        }
		root = freetrie(root);
    }
	return 0;
}


