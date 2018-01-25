//
// Created by noble6emc2 on 2017/12/3.
//

#ifndef COMPILER_GENMID_H
#define COMPILER_GENMID_H
//#include "grammer.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <ctype.h>
#define MIDNUM 10000
#define MAXPARANUM 10//参数个数
#define MAXSYMTABLENUM 1000//每个符号表元素个数
#define MAXFUNCNUM 100//函数个数
#define MAXIDENLEN 40//最大标识符长度
#define MAXLEVEL 10//语句列最大嵌套深度（该层次函数调用内层次的不算）
#define MAXEXPLEVEL 10//表达式最大嵌套深度（该层次函数调用内层次的不算）
#define CONSTINT 0 //常
#define CONSTCHR 1
#define INT 2 //变量
#define CHAR 3 //参数
#define FUNC 4 //函数
#define INTARR 5
#define CHRARR 6
//
//int pc=0;//代码计数器
#define STACKLIM 0x7fffeffc
#define ARGLIM 0x10000000
//#define STACKLIM 0x100000
#define STRINGLIM 0x10002000
int sp=STACKLIM;//stack top
//
FILE *mipsout=fopen("mips.txt","w");
FILE *midout=fopen("mid.txt","w");
typedef struct{
    char name [ MAXIDENLEN ]; //名字，可限制长度
    int kind; //种类
    int ret; //对函数来说，1为返回值函数，0为void函数
    int fundir;
    int addr; //相对地址
    int size;//大小
    int paranum; //参数数目
}symb;

/*typedef struct {
    symb para[ MAXPARANUM ]; //可限制每个函数的参数个数，入表时应检测kind是否等于PARA。
    int index;
    int ptotal; //参数总数，
    int paranum;//
}paratable; //参数单独设表，方便处理*/

typedef struct {
    symb ele[MAXFUNCNUM][MAXSYMTABLENUM];
    int memnum;//全局大小
    //int funnum;//函数大小
    int index[MAXFUNCNUM];
    int funsize[MAXFUNCNUM];
    int ftotal; //函数总数;
    int loc;//查表层数；
    int pointer;
}symtable;

symtable maintab;
//paratable mainparatab;
void inittab(){
    memset(maintab.index,-1,sizeof(maintab.index));
    memset(maintab.funsize,-1,sizeof(maintab.funsize));
    maintab.ftotal=0;
    maintab.memnum=0;//全局数据区大小！！！;
    //maintab.funnum=-1;
    maintab.pointer=0;//指向全局或函数;
    maintab.loc=0;
}
void printtable(){
    printf("tab0:\n");
    for(int b=0;b<=maintab.index[0];b++)
        printf("name:%s kind%d ret:%d fundir:%d addr:%d size:%d paranum:%d\n",
               maintab.ele[0][b].name,maintab.ele[0][b].kind,
               maintab.ele[0][b].ret,maintab.ele[0][b].fundir,
               maintab.ele[0][b].addr,maintab.ele[0][b].size,
               maintab.ele[0][b].paranum);
    for(int a=1;a<=maintab.ftotal;a++){
        printf("\ntab%d:\n",a);
        for(int b=0;b<=maintab.index[a];b++)
            printf("name:%s kind%d ret:%d fundir:%d addr:%d size:%d paranum:%d\n",
                   maintab.ele[a][b].name,maintab.ele[a][b].kind,
                   maintab.ele[a][b].ret,maintab.ele[a][b].fundir,
                   maintab.ele[a][b].addr,maintab.ele[a][b].size,
                   maintab.ele[a][b].paranum);
    }
}
int findtable(char *name){
    int n=maintab.pointer;
    int main=0;
    for(int a=0;a<=maintab.index[n];a++)
        if(!strcmp(name,maintab.ele[n][a].name)){
            maintab.loc=n;
            return a;
        }
    for(int a=0;a<=maintab.index[main];a++)
        if(!strcmp(name,maintab.ele[main][a].name)){
            maintab.loc=main;
            return a;
        }
    return -1;
}
int findpartialtable(char *name){
    int n=maintab.pointer;
    for(int a=0;a<=maintab.index[n];a++)
        if(!strcmp(name,maintab.ele[n][a].name)) return a;
    return -1;
}
bool entertable(char name[],int kind,int size, int ret,int paranum){
    if(findpartialtable(name)!=-1){
        bool exit1=error(6,w_count,temp);//在本层重复命名;
        if(exit1) return 0;
        return 1;//ignore the entering request;
    }
    int top=++maintab.index[maintab.pointer];
    if(maintab.index[maintab.pointer]==MAXSYMTABLENUM){
        bool exit1=error(4,w_count,temp);
        if(exit1) return 0;
    }
    strcpy(maintab.ele[maintab.pointer][top].name,name);
    maintab.ele[maintab.pointer][top].kind=kind;
    maintab.ele[maintab.pointer][top].size=size;
    maintab.ele[maintab.pointer][top].ret=ret;
    if(maintab.pointer==0){
        maintab.ele[maintab.pointer][top].addr=maintab.memnum;
        printf("memnum:%d\n",maintab.memnum);
        maintab.memnum+=size;//在全局层填了变量等要算大小，函数不算
    }
    else{
        if(maintab.funsize[maintab.pointer]==-1) maintab.funsize[maintab.pointer]=0;
        maintab.ele[maintab.pointer][top].addr=maintab.funsize[maintab.pointer];//函数内记录相对地址
        maintab.funsize[maintab.pointer]+=size;
    }
    maintab.ele[maintab.pointer][top].paranum=paranum;
    if(kind==FUNC){
        maintab.ftotal+=1;
        maintab.ele[maintab.pointer][top].fundir=maintab.ftotal;
        if(maintab.ftotal==MAXFUNCNUM){
            bool exit1=error(4,w_count,temp);
            if(exit1) return 0;
        }
    }
    printf("%s enter at loc %d %d %d\n",name,maintab.pointer,top,kind);
    return 1;
}
int temp_num=0;//记录临时变量编号;
bool entertemp(char *name, int type){
    sprintf(name,"temp&&%d",temp_num++);
    //printf("temp_num%d\n",temp_num);
    int loc=findtable(name);
    int loc1=maintab.loc;
    if(loc==-1)
        return entertable(name,type,4,-1,-1);
    else{
        maintab.ele[loc1][loc].kind=type;
        printf("%s modified to %d\n",name,type);
        return 1;
    }
}
struct midcode{
    char name[20];
    char a[200];
    char b[200];
    char c[200];
    int num;
};
int midtop=-1;
midcode mcode[MIDNUM];
bool midtomips3(char *name,char *a,char *b,char *c);
bool midtomips2(char *name,char *a,char *b);
bool midtomips1(char *name,char *a);
bool midtomips0(char *name);
void printstr();
void outputmips(){
    printstr();
    //fprintf(mipsout,"li $sp %d\n",STACKLIM);//update sp
    for(int a=0;a<=midtop;a++){
        //printf("midcode: %s %s %s %s\n",mcode[a].name,mcode[a].a,mcode[a].b,mcode[a].c);
        if(mcode[a].num==0)   midtomips0(mcode[a].name);
        else if(mcode[a].num==1)   midtomips1(mcode[a].name,mcode[a].a);
        else if(mcode[a].num==2)   midtomips2(mcode[a].name,mcode[a].a,mcode[a].b);
        else if(mcode[a].num==3)   midtomips3(mcode[a].name,mcode[a].a,mcode[a].b,mcode[a].c);
        fprintf(mipsout,"\n");
    }
}
void emit3(char name[], char a[], char b[],char c[]){
    midtop+=1;
    if(midtop==MIDNUM){
        bool exit1=error(4,w_count,temp);
        if(exit1) return;
    }
    strcpy(mcode[midtop].name,name);
    strcpy(mcode[midtop].a,a);
    strcpy(mcode[midtop].b,b);
    strcpy(mcode[midtop].c,c);
    mcode[midtop].num=3;
    //if(!strcmp(name,"=")) printf("instruction@@@@@@@@@: %s=%s\n",a,b);
    fprintf(midout,"%s %s %s %s\n",name,a,b,c);
    printf("%s %s %s %s\n",name,a,b,c);
    /*if(!midtomips3(name,a,b,c)){
           bool exit1=error(-1,w_count,temp);
    }
    fprintf(mipsout,"\n");*/
}
void emit2(char name[], char a[], char b[]){
    midtop+=1;
    if(midtop==MIDNUM){
        bool exit1=error(4,w_count,temp);
        if(exit1) return;
    }
    strcpy(mcode[midtop].name,name);
    strcpy(mcode[midtop].a,a);
    strcpy(mcode[midtop].b,b);
    mcode[midtop].num=2;
    fprintf(midout,"%s %s %s\n",name,a,b);
    printf("%s %s %s\n",name,a,b);
    /*if(!midtomips2(name,a,b)){
        bool exit1=error(-1,w_count,temp);
    }
    fprintf(mipsout,"\n");*/
}

void emit1(char name[], char a[]){
    midtop+=1;
    if(midtop==MIDNUM){
        bool exit1=error(4,w_count,temp);
        if(exit1) return;
    }
    strcpy(mcode[midtop].name,name);
    strcpy(mcode[midtop].a,a);
    mcode[midtop].num=1;
    //if(!strcmp(name,"ret")) printf("instruction@@@@@@@@@: ret %s\n",name);
    //if(!strcmp(name,"scf")) printf("instruction@@@@@@@@@: scf %s\n",name);
    //if(!strcmp(name,"lab")) printf("instruction@@@@@@@@@: %s:\n",name);
    //if(!strcmp(name,"jmp")) printf("instruction@@@@@@@@@: ret %s\n",name);
    fprintf(midout,"%s %s\n",name,a);
    printf("%s %s\n",name,a);
    /*if(!midtomips1(name,a)){
        bool exit1=error(-1,w_count,temp);
    }
    fprintf(mipsout,"\n");*/
}

void emit0(char name[]){
    midtop+=1;
    if(midtop==MIDNUM){
        bool exit1=error(4,w_count,temp);
        if(exit1) return;
    }
    strcpy(mcode[midtop].name,name);
    mcode[midtop].num=0;
    fprintf(midout,"%s\n",name);
    printf("%s\n",name);
    /*if(!midtomips0(name)){
        bool exit1=error(-1,w_count,temp);
    }
    fprintf(mipsout,"\n");*/
}
int pushtop=0;
//int baseaddr[100];
//int blevel=-1;
bool midtomips3(char *name, char *a, char *b, char *c){
    if(!strcmp(name,"=")&&!strcmp(c,"1")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        int num=atoi(b);
        fprintf(mipsout,"li $t0 %d\n",num);
        //fprintf(mipsout,"sw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"sw $t0 -%d($sp)\n",addr+8);//local;
    }
    else if(!strcmp(name,"=")&&!strcmp(c,"0")){
        int loc=findtable(b);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            printf("name: %s pointer: %d\n",a,maintab.pointer);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t0 %d\n",addr);
        //fprintf(mipsout,"lw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8);//local;
        loc=findtable(a);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            printf("name: %s pointer: %d\n",b,maintab.pointer);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"sw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"sw $t0 -%d($sp)\n",addr+8);//local;
    }
    else if(!strcmp(name,"=[]")){
        int loc=findtable(b);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;

        //fprintf(mipsout,"li $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"li $t0 %d\n",STACKLIM-addr);//global;
        else {
                fprintf(mipsout,"li $t0 -%d\n",addr+8);
                fprintf(mipsout,"add $t0 $t0 $sp\n");
        }//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;

        fprintf(mipsout,"add $t1 $t1 $t1\n");
        fprintf(mipsout,"add $t1 $t1 $t1\n");//4*t1;
        fprintf(mipsout,"sub $t0 $t0 $t1\n");//array addr
        fprintf(mipsout,"lw $t1 0($t0)\n");//res;
        loc=findtable(a);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"sw $t1 %d\n",addr);//res;
        if(loc1==0) fprintf(mipsout,"sw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"sw $t1 -%d($sp)\n",addr+8);//local;
    }
    else if(!strcmp(name,"[]=")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;

        //fprintf(mipsout,"li $t0 %d\n",addr);
        if(loc1==0){
                fprintf(mipsout,"li $t0 %d\n",STACKLIM-addr);//global;
        }
        else{
                fprintf(mipsout,"li $t0 -%d\n",addr+8);
                fprintf(mipsout,"add $t0 $t0 $sp\n");
        }//local;
        loc=findtable(b);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;

        fprintf(mipsout,"add $t1 $t1 $t1\n");
        fprintf(mipsout,"add $t1 $t1 $t1\n");//4*t1;
        fprintf(mipsout,"sub $t0 $t0 $t1\n");//array addr
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"sw $t1 %d\n",addr);//res;
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"sw $t1 0($t0)\n");//res;
    }
    else if(!strcmp(name,"/")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        int taraddr,tarloc;
        taraddr=addr;//memo
        tarloc=loc1;//
        loc=findtable(b);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t2 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t2 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t2 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"div $t1 $t2\n");
        fprintf(mipsout,"mflo $t0\n");
        if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
        else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8);//local;
    }
    else if(!strcmp(name,"*")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        int taraddr,tarloc;
        taraddr=addr;//memo
        tarloc=loc1;//
        loc=findtable(b);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t2 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t2 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t2 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"mult $t1 $t2\n");
        fprintf(mipsout,"mflo $t0\n");
        if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
        else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8);//local;
    }
    else if(!strcmp(name,"-")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        int taraddr,tarloc;
        taraddr=addr;//memo
        tarloc=loc1;//
        loc=findtable(b);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t2 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t2 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t2 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"sub $t0 $t1 $t2\n");
        if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
        else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8);//local;
    }
    else if(!strcmp(name,"+")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        int taraddr,tarloc;
        taraddr=addr;//memo
        tarloc=loc1;//
        loc=findtable(b);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t2 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t2 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t2 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"add $t0 $t1 $t2\n");
        if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
        else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8);//local;
    }
    else if(!strcmp(name,"bne")){
        int loc=findtable(b);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"bne $t0 $t1 %s\n",a);
    }
    else if(!strcmp(name,"beq")){
        int loc=findtable(b);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"beq $t0 $t1 %s\n",a);
    }
    else if(!strcmp(name,"bgr")){
        int loc=findtable(b);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"sub $t0 $t0 $t1\n");
        fprintf(mipsout,"bgtz $t0 %s\n",a);
    }
    else if(!strcmp(name,"bge")){
        int loc=findtable(b);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"sub $t0 $t0 $t1\n");
        fprintf(mipsout,"bgez $t0 %s\n",a);
    }
    else if(!strcmp(name,"bls")){
        int loc=findtable(b);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"sub $t0 $t0 $t1\n");
        fprintf(mipsout,"bltz $t0 %s\n",a);
    }
    else if(!strcmp(name,"ble")){
        int loc=findtable(b);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8);//local;
        loc=findtable(c);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t1 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"sub $t0 $t0 $t1\n");
        fprintf(mipsout,"blez $t0 %s\n",a);
    }
    else if(!strcmp(name,"prt")&&!strcmp(b,"0")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        int kind=atoi(c);
        if(!(kind==INT||kind==CHAR||kind==CONSTCHR||kind==CONSTINT)){
            printf("type int/char error ");
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        //printf("prtkind %s %d %d %d\n",a,kind,loc1,loc);
        fprintf(mipsout,"li $a0 %d\n",STRINGLIM);
        if(kind==INT||kind==CONSTINT) fprintf(mipsout,"li $v0 %d\n",1);
        else fprintf(mipsout,"li $v0 %d\n",11);
        //fprintf(mipsout,"lw $a0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $a0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $a0 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"syscall\n");
    }
    return 1;
}
bool midtomips2(char *name, char *a, char *b){
    if(!strcmp(name,"bnez")){
        int loc=findtable(b);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        //fprintf(mipsout,"lw $t0 %d\n",addr);
        if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8);//local;
        fprintf(mipsout,"bne $t0 $0 %s\n",a);
    }
    else if(!strcmp(name,"prt")&&!strcmp(b,"1")){
        int pos=0;
        while(pos<=strcontop){
            if(!strcmp(stringcon[pos],a)) break;
            else pos++;
        }
        if(pos>strcontop){
            printf("print error");
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        fprintf(mipsout,"la $a0 String%d\n",pos);
        fprintf(mipsout,"li $v0 %d\n",4);
        fprintf(mipsout,"syscall\n");
    }
    return 1;
}
bool midtomips1(char *name, char *a){
    if(!strcmp(name,"call")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        int paranum=maintab.ele[loc1][loc].paranum;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        if(maintab.ele[loc1][loc].kind!=FUNC){
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
            //return 1;
        }
        if(maintab.pointer==0){
            printf("maintab.memnum:%d\n",maintab.memnum);
            fprintf(mipsout,"sw $sp -%d($sp)\n",maintab.memnum+4);// return stack num;
            fprintf(mipsout,"subi $sp $sp %d\n",maintab.memnum);
        }
        else{
            if(maintab.funsize[maintab.pointer]==-1)
                maintab.funsize[maintab.pointer]=0;
            fprintf(mipsout,"sw $sp -%d($sp)\n",12+maintab.funsize[maintab.pointer]);
            fprintf(mipsout,"subi $sp $sp %d\n",8+maintab.funsize[maintab.pointer]);
        }
        //printf("pushtop-paranum:%d\n",pushtop-paranum);
        for(int a=paranum-1;a>=0;a--){
            fprintf(mipsout,"subi $gp $gp 4\n");
            fprintf(mipsout,"lw $a0 0($gp)\n");
            fprintf(mipsout,"sw $a0 -%d($sp)\n",4*a+8);//reserv 1 byte;
        }
        /*for(int a=pushtop-paranum;a<pushtop;a++){
            fprintf(mipsout,"lw $a0 %d\n",4*a+ARGLIM);
            fprintf(mipsout,"sw $a0 -%d($sp)\n",4*(a-pushtop+paranum)+8);//reserv 1 byte;
        }*/
        //reserv the return address and data;
        //pushtop-=paranum;
        fprintf(mipsout,"jal %s\n",a);//ra=namelab;
    }
    else if(!strcmp(name,"ret")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;//global;
        if(loc1==0) fprintf(mipsout,"lw $v0 %d\n",STACKLIM-addr);
        else fprintf(mipsout,"lw $v0 -%d($sp)\n",addr+8);
        fprintf(mipsout,"lw $ra 0($sp)\n");//ra=namelab;
        fprintf(mipsout,"lw $sp -4($sp)\n");//ra=namelab;
        fprintf(mipsout,"jr $ra\n");
    }
    else if(!strcmp(name,"lab")){
        fprintf(mipsout,"%s:\n",a);
    }
    else if(!strcmp(name,"jmp")){
        fprintf(mipsout,"j %s\n",a);
    }
    else if(!strcmp(name,"push")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8);//local;
        //fprintf(mipsout,"sw $t0 %d\n",ARGLIM+4*pushtop);
        fprintf(mipsout,"sw $t0 0($gp)\n");
        fprintf(mipsout,"addi $gp $gp 4\n");
        //pushtop+=1;
    }
    else if(!strcmp(name,"scf")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        int kind=maintab.ele[loc1][loc].kind;
        if(kind==INT){
            fprintf(mipsout,"li $v0 5\n");
            fprintf(mipsout,"syscall\n");
            if(loc1==0) fprintf(mipsout,"sw $v0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"sw $v0 -%d($sp)\n",addr+8);//local;
        }
        else if(kind==CHAR){
            fprintf(mipsout,"li $v0 12\n");
            fprintf(mipsout,"syscall\n");
            //fprintf(mipsout,"sw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"sw $v0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"sw $v0 -%d($sp)\n",addr+8);//local;
        }
        else{
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
            //return 1;
        }
    }
    else if(!strcmp(name,"v=")){
        int loc=findtable(a);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int addr=maintab.ele[loc1][loc].addr;
        if(loc1==0) fprintf(mipsout,"sw $v0 %d\n",STACKLIM-addr);//global;
        else fprintf(mipsout,"sw $v0 -%d($sp)\n",addr+8);//local;
    }
    else if(!strcmp(name,"swtpoi")){
        int num=atoi(a);
        maintab.pointer=num;//switch maintab.pointer!!!
    }
    return 1;
}

void printstr(){
    fprintf(mipsout,".data\n");
    for(int a=0;a<=strcontop;a++){
        fprintf(mipsout,"\tString%d: .asciiz \"",a);
        for(int b=0;b<strlen(stringcon[a]);b++)
            if(stringcon[a][b]!='\\') fprintf(mipsout,"%c",stringcon[a][b]);
            else fprintf(mipsout,"\\\\");
        fprintf(mipsout,"\" \n");
    }
    fprintf(mipsout,".text\n");
    fprintf(mipsout,".globl main\n");
}

bool midtomips0(char *name){
    if(!strcmp(name,"swra")){
        fprintf(mipsout,"sw $ra 0($sp)\n");
    }
    else if(!strcmp(name,"ret0")){
        fprintf(mipsout,"li $v0 0\n");
        fprintf(mipsout,"lw $ra 0($sp)\n");//ra=namelab;
        fprintf(mipsout,"lw $sp -4($sp)\n");//stack retreat;
        fprintf(mipsout,"jr $ra\n");
        //blevel--;
        /*if(blevel<0){
            bool exit1=error(4,w_count,temp);
            if(exit1) return 0;
        }*/
        //sp=baseaddr[blevel]-4;
        //maintab.pointer=basepointer[blevel];
    }
    else if(!strcmp(name,"ret")){
        fprintf(mipsout,"lw $ra 0($sp)\n");//ra=namelab;
        fprintf(mipsout,"lw $sp -4($sp)\n");//stack retreat;
        fprintf(mipsout,"jr $ra\n");
        /*blevel--;
        if(blevel<0){
            bool exit1=error(4,w_count,temp);
            if(exit1) return 0;
        }*/
        //sp=baseaddr[blevel]-4;
        //maintab.pointer=basepointer[blevel];
    }
    return 1;
}
#endif //COMPILER_GENMID_H
