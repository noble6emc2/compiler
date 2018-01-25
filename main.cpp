#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define DIG -1
#define LET -2
#define SPA -3//space
#define TAB -4
#define SLE -5//string letter
#define NLI -6//newline
#include "error.h"
#include "symbol0.h"
#include "genmid.h"
#include "optim.h"
#include "mips.h"
#include "grammer.h"
int main(){
    char path[200];
    printf("enter source code path:");
    scanf("%s",path);
    fp=fopen(path,"r");
    if(fp==NULL){
        printf("cannot open the file\n");
        return 0;
    }
    //printf("itworks\n");
    //printf("%c\n",(temp=fgetc(fp)));
    //printf("%d",aflop);
    inittab();
    if(!program()){
        error(-1,w_count,temp);
        return 0;
    }
    printtable();
    anaprogram();
    outputmid();
    //outputmips();
    if(!iserror) printf("compile completed ....\n");
    return 0;
}
