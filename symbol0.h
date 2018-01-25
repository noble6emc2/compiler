//
// Created by noble6emc2 on 2017/11/24.
//

#ifndef COMPILER_SYMBOL0_H
#define COMPILER_SYMBOL0_H
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>
//#include "error.h"
#define DOSY 1//do
#define WHILSY 2//while
#define SWTSY 3//switch
#define CASESY 4//case
#define SCANF 5//scanf
#define PRINTF 6//printf
#define RETSY 7//return
#define IFSY 8//if
#define IDSY 9//标识符
#define INTSY 10//int
#define CHARSY 11//char
#define INTCON 12//a=1;
#define VOIDSY 13//void
#define CHARCON 14//e.g.:'a'
#define STRINGCON 15//e.g.: "a"
#define CONSTSY 16//const
#define PLUSSY 17//+
#define MINUSSY 18//-
#define STARSY 19//*
#define DIVISY 20///
#define EQLSY 21//==
#define NEQSY 22//!=
#define GTRSY 23//>
#define GEQSY 24//>=
#define LSSSY 25//<
#define LEQSY 26//<=
#define LPARSY 27//(
#define RPARSY 28//)
#define LBRASY 29//[
#define RBRASY 30//]
#define LCBRASY 31//{
#define RCBRASY 32//}
#define COMMASY 33//,
#define SEMISY 34//;
#define COLONSY 35//:
#define ASSIGNSY 36//=
int w_count=1;
char token[10001];
char temp_id[10001];
char stringcon [100][100];
int strcontop=-1;
int temp_sym;
int token_p=0;
int num;
int symbol;
char temp=' ';
FILE *fp;
void clearToken(){
    token_p=0;
    num=0;
    symbol=-1;
}
char Getchar(){
    char temp=fgetc(fp);
    if(temp=='\n') errline++;
    return temp;
}
int isLetter(char n){
    if((n<='z'&&n>='a')||(n<='Z'&&n>='A')||n=='_') //'_' is one of the letters;
        return 1;
    return 0;
}
int isEND(char n){
    if(n==EOF) return 1;
    return 0;
}
int isDigit(char n){
    if(n<='9'&&n>='0')
        return 1;
    return 0;
}
int isDigitnz(char n){
    if(n<='9'&&n>='1')
        return 1;
    return 0;
}//contain no zero;
int isSpace(char n){
    if(n==' ')
        return 1;
    return 0;
}
int isTab(char n){
    if(n=='\t')
        return 1;
    return 0;
}
int isStrlet(char n){
    if(n==32||n==33||(n>=35&&n<=126))
        return 1;
    return 0;
}
int isNewline(char n){
    if(n=='\n'||n=='\r')
        return 1;
    return 0;
}
void catToken(char n){
    token[token_p]=n;
    token_p++;
    //token[token_p]='\0';
}
void retract(){
    //w_count--;
    if(temp=='\n') errline--;
    fseek(fp,-1,SEEK_CUR);
}
int transNum(){
    token[token_p]='\0';
    return atoi(token);
}
int reserver(){
    token[token_p]='\0';//@@@@@!!!
    for(int a=0;a<=token_p;a++)
        token[a]=tolower(token[a]);//转大写,标识符不分大小写;
    if(!strcmp(token,"if")) return IFSY;
    else if(!strcmp(token,"char")) return CHARSY;
    else if(!strcmp(token,"int")) return INTSY;
    else if(!strcmp(token,"const")) return CONSTSY;
    else if(!strcmp(token,"void")) return VOIDSY;
    else if(!strcmp(token,"do")) return DOSY;
    else if(!strcmp(token,"while")) return WHILSY;
    else if(!strcmp(token,"void")) return VOIDSY;
    else if(!strcmp(token,"switch")) return SWTSY;
    else if(!strcmp(token,"case")) return CASESY;
    else if(!strcmp(token,"return")) return RETSY;
    else if(!strcmp(token,"printf")) return PRINTF;
    else if(!strcmp(token,"scanf")) return SCANF;
    else return 0;
}
void print_w(){
    if(symbol==-1) return;
    else if(symbol==DOSY) printf("%d DOSY do\n",w_count);
    else if(symbol==WHILSY) printf("%d WHILSY while\n",w_count);
    else if(symbol==SWTSY) printf("%d SWTSY switch\n",w_count);
    else if(symbol==CASESY) printf("%d CASESY case\n",w_count);
    else if(symbol==SCANF) printf("%d SCANF scanf\n",w_count);
    else if(symbol==PRINTF) printf("%d PRINTF printf\n",w_count);
    else if(symbol==RETSY) printf("%d RETSY return\n",w_count);
    else if(symbol==IFSY) printf("%d IFSY if\n",w_count);
    else if(symbol==IDSY) {printf("%d IDSY %s\n",w_count,token);}
    else if(symbol==INTSY) {printf("%d INTSY int\n",w_count);}
    else if(symbol==CHARSY) printf("%d CHARSY char\n",w_count);
    else if(symbol==INTCON) printf("%d INTCON %d\n",w_count,num);
    else if(symbol==VOIDSY) printf("%d VOIDSY void\n",w_count);
    else if(symbol==CHARCON) printf("%d CHARCON %c\n",w_count,num);
    else if(symbol==STRINGCON) printf("%d STRINGCON %s\n",w_count,token);
    else if(symbol==CONSTSY) printf("%d CONSTSY const\n",w_count);
    else if(symbol==PLUSSY) {printf("%d PLUSSY +\n",w_count);}
    else if(symbol==MINUSSY) {printf("%d MINUSSY -\n",w_count);}
    else if(symbol==STARSY) {printf("%d STARSY *\n",w_count);}
    else if(symbol==DIVISY) {printf("%d DIVISY /\n",w_count);}
    else if(symbol==EQLSY) {printf("%d EQLSY ==\n",w_count);}
    else if(symbol==NEQSY) {printf("%d NEQSY !=\n",w_count);}
    else if(symbol==GTRSY) {printf("%d GTRSY >\n",w_count);}
    else if(symbol==GEQSY) {printf("%d GEQSY >=\n",w_count);}
    else if(symbol==LSSSY) {printf("%d LSSSY <\n",w_count);}
    else if(symbol==LEQSY) {printf("%d LEQSY <=\n",w_count);}
    else if(symbol==LPARSY) {printf("%d LPARSY (\n",w_count);}
    else if(symbol==RPARSY) {printf("%d RPARSY )\n",w_count);}
    else if(symbol==LBRASY) {printf("%d LBRASY [\n",w_count);}
    else if(symbol==RBRASY) {printf("%d RBRASY ]\n",w_count);}
    else if(symbol==LCBRASY) {printf("%d LCBRASY {\n",w_count);}
    else if(symbol==RCBRASY) {printf("%d RCBRASY }\n",w_count);}
    else if(symbol==COMMASY) {printf("%d COMMASY ,\n",w_count);}
    else if(symbol==SEMISY) {printf("%d SEMISY ;\n",w_count);}
    else if(symbol==COLONSY) {printf("%d COLONSY :\n",w_count);}
    else if(symbol==ASSIGNSY) {printf("%d ASSIGNSY =\n",w_count);}
}
int getsym(){
    //printf("temp:%c\n",temp);
    clearToken();
    //printf("temp:%c\n",temp);
    do{
        temp=Getchar();
    }
    while((isSpace(temp)||isNewline(temp)||isTab(temp))&&!isEND(temp));
    if(isEND(temp)) return 0;
    if(isLetter(temp)){//先为字母
        while((isLetter(temp)||isDigit(temp))&&!isEND(temp)){
            catToken(temp);
            temp=Getchar();

        }
        retract();
        int ret=reserver();//转小写;
        if(ret==0) symbol=IDSY;
        else symbol=ret;
    }//标识符
    else if(isDigit(temp)){//unsigned number;
        if(temp!='0'){
            while(isDigit(temp)&&!isEND(temp)){
                catToken(temp);
                temp=Getchar();
            }
            retract();
            num=transNum();
            symbol=INTCON;
        }
        else{
            num=0;
            symbol=INTCON;
        }
    }//字面整数()
    else if(temp==':'){
        temp=Getchar();
        symbol=COLONSY;
        retract();
    }
    else if(temp=='"'){
        temp=Getchar();
        while(isStrlet(temp)&&!isEND(temp)&&!isNewline(temp)){
            catToken(temp);
            temp=Getchar();
        }//不允许字符串跨行;
        if(temp=='"'){
            token[token_p]='\0';
            symbol=STRINGCON;
            int pos=0;
            while(pos<=strcontop){
                if(!strcmp(stringcon[pos],token)) break;
                else pos++;
            }
            if(pos>strcontop) strcontop=pos;
            strcpy(stringcon[pos],token);
        }
        else {
            error(1,w_count,temp);//缺双引号
            token[token_p]='\0';
            symbol=STRINGCON;
            int pos=0;
            while(pos<=strcontop){
                if(!strcmp(stringcon[pos],token)) break;
                else pos++;
            }
            if(pos>strcontop) strcontop=pos;
            strcpy(stringcon[pos],token);
        }
    }//string can be empty!!!;
    else if(temp=='\''){
        temp=Getchar();
        if(temp=='-'||temp=='+'||temp=='*'||temp=='/'||isDigit(temp)||isLetter(temp)) num=temp;
        else {
            sytop=-1;
            skiptop=-1;
            skiptar[++skiptop]='-';
            skiptar[++skiptop]='+';
            skiptar[++skiptop]='*';
            skiptar[++skiptop]='/';
            skiptar[++skiptop]=DIG;
            skiptar[++skiptop]=LET;
            error(2,w_count,temp);
            num=temp;//!!
        }//字符未识别
        temp=Getchar();
        if(temp!='\''){
            error(3,w_count,temp);//缺单引号
            printf("charcon error\n");//容错
            retract();
            symbol=CHARCON;
        }
        else symbol=CHARCON;
    }//char
    else if(temp=='+') symbol=PLUSSY;//+
    else if(temp=='-') symbol=MINUSSY;//-
    else if(temp=='*') symbol=STARSY;//*
    else if(temp=='(') symbol=LPARSY;//(
    else if(temp==')') symbol=RPARSY;//)
    else if(temp=='[') symbol=LBRASY;//[
    else if(temp==']') symbol=RBRASY;//]
    else if(temp=='{') symbol=LCBRASY;//{
    else if(temp=='}') symbol=RCBRASY;//}
    else if(temp==',') symbol=COMMASY;//,
    else if(temp==';') symbol=SEMISY;//;
    else if(temp=='/') symbol=DIVISY;// /
    else if(temp=='>'){
        temp=Getchar();
        if(temp=='='){
            symbol=GEQSY;
        }
        else{
            symbol=GTRSY;
            retract();
        }
    }//>\>=
    else if(temp=='<'){
        temp=Getchar();
        if(temp=='='){
            symbol=LEQSY;
        }
        else{
            symbol=LSSSY;
            retract();
        }
    }//<\<=
    else if(temp=='='){
        temp=Getchar();
        if(temp=='=') symbol=EQLSY;
        else{
            symbol=ASSIGNSY;
            retract();
        }
    }//=\==
    else if(temp=='!'){
        temp=Getchar();
        if(temp=='=') symbol=NEQSY;
        else {
            sytop=-1;
            skiptop=-1;
            skiptar[++skiptop]='=';
            error(2,w_count,temp);
            symbol=NEQSY;
        }//无法识别的字符
    }//!=
    else {
        sytop=-1;
        skiptop=-1;
        skiptar[++skiptop]=DIG;skiptar[++skiptop]=LET;
        skiptar[++skiptop]=':';skiptar[++skiptop]='"';
        skiptar[++skiptop]='\'';skiptar[++skiptop]='+';
        skiptar[++skiptop]='-';skiptar[++skiptop]='*';
        skiptar[++skiptop]='(';skiptar[++skiptop]=')';
        skiptar[++skiptop]='[';skiptar[++skiptop]=']';
        skiptar[++skiptop]='{';skiptar[++skiptop]='}';
        skiptar[++skiptop]=',';skiptar[++skiptop]=';';
        skiptar[++skiptop]='/';skiptar[++skiptop]='>';
        skiptar[++skiptop]='<';skiptar[++skiptop]='=';
        skiptar[++skiptop]='!';
        error(2,w_count,temp);
        printf("skip completed\n");
        retract();
        getsym();
        return 0;
    }//无法识别的字符
    print_w();
    w_count++;
    return 0;
}
void skip(){
    bool ok=0;
    if(skiptop==-1) return;
    printf("oper:skip charactor");
    for(int a=0;a<=skiptop-1;a++)
        if(skiptar[a]<0) printf("no.%d,",skiptar[a]);
        else printf("%c,",skiptar[a]);
    if(skiptar[skiptop]<0) printf("no.%d\n",skiptar[skiptop]);
    else printf("%c\n",skiptar[skiptop]);
    while(!ok){
        if(isEND(temp)) return;
        temp=Getchar();
        for(int a=0;a<=skiptop;a++) {
            if (skiptar[a] == DIG && isDigit(temp)) ok = 1;
            else if (skiptar[a] == LET && isLetter(temp)) ok = 1;
            else if (skiptar[a] == SPA && isSpace(temp)) ok = 1;
            else if (skiptar[a] == TAB && isTab(temp)) ok = 1;
            else if (skiptar[a] == SLE && isStrlet(temp)) ok = 1;
            else if (skiptar[a] == NLI && isNewline(temp)) ok = 1;
            else if (skiptar[a] == temp) ok = 1;
            if(ok) break;
        }
    }
}
void syskip(){
    bool ok=0;
    if(sytop==-1) return;
    printf("oper:skip symbol");
    for(int a=0;a<=sytop-1;a++)
        if(sytar[a]<0) printf("no.%d,",sytar[a]);
        else printf("%c,",sytar[a]);
    if(skiptar[sytop]<0) printf("no.%d\n",sytar[skiptop]);
    else printf("%c\n",sytar[sytop]);
    while(!ok){
        if(isEND(temp)) return;
        getsym();
        for(int a=0;a<=sytop;a++) {
            if(symbol==sytar[a]) ok=1;
            if(ok) break;
        }
    }
}
#endif //COMPILER_SYMBOL0_H
