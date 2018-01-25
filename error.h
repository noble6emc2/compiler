//
// Created by noble6emc2 on 2017/11/26.
//

#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

//#include <tic.h>
//int symbol;
//int getsym();
//int isDigit(char n);
//int isLetter(char n);
//char temp=' ';
void skip();
void syskip();
#define ERMAX 100
bool iserror=0;
int errline=1;
char errormsg[ERMAX][100];
int skiptar[100];
int skiptop=-1;
int sytar[100];
int sytop=-1;
bool error(int ecode,int w_count,char n){
    strcpy(errormsg[0],"MAINLACK   ");  strcpy(errormsg[1],"DOUBLE_QUOTE   ");
    strcpy(errormsg[2],"UNKNOWNCHAR   ");  strcpy(errormsg[3],"SINGLEQUOTE   ");
    strcpy(errormsg[4],"TABLEEXP   ");  strcpy(errormsg[5],"NAMENOTEXIST   ");
    strcpy(errormsg[6],"NAMEREDEF   ");  strcpy(errormsg[7],"CONSTDEFERR   ");
    strcpy(errormsg[8],"FUNCTIONERR   ");  strcpy(errormsg[9],"IDENTYPERROR   ");
    strcpy(errormsg[10],"SEMICOLONLACK   ");  strcpy(errormsg[11],"DEFIDENNMELACK   ");
    strcpy(errormsg[12],"REDUNDANTARG   ");  strcpy(errormsg[13],"LACKPARA   ");
    strcpy(errormsg[14],"RIGHTBRACKLACK   ");  strcpy(errormsg[15],"LEFTPARLACK   ");
    strcpy(errormsg[16],"RIGHTPARLACK   ");  strcpy(errormsg[17],"INTCONLACK   ");
    strcpy(errormsg[18],"STRINGCONLACK   ");  strcpy(errormsg[19],"STRINGCONERR   ");
    strcpy(errormsg[20],"RIGHTBRACELACK   ");  strcpy(errormsg[21],"EXPRESSIONERROR   ");
    strcpy(errormsg[22],"CONSTASSIGN   ");  strcpy(errormsg[23],"LEFTBRACELACK   ");
    strcpy(errormsg[24],"NORET   ");  strcpy(errormsg[25],"OPERATERLACK   ");
    strcpy(errormsg[26],"LOGICOPLACK   ");  strcpy(errormsg[27],"EXTRACODE   ");
    strcpy(errormsg[28],"CONSTREADERR   ");  strcpy(errormsg[29],"CONLACK   ");
    strcpy(errormsg[30],"CALLMAIN   "); strcpy(errormsg[31],"DEFMAIN   ");
    if(n!=EOF){
        if(ecode>=0){
            printf("%serror at line %d, %c\n",errormsg[ecode],errline,n);
        }
        else{
            printf("error at line %d, %c\n",errline,n);
        }
    }
    else{
        if(ecode>=0){
            printf("%serror at line %d, EOF\n",errormsg[ecode],errline);
        }
        else{
            printf("error at line %d, EOF\n",errline);
        }
    }
    iserror=1;//indicate there is error in the program. The program's quality cannot be ganranteed;
    if(ecode==0) {printf("oper:exit the program\n");return 1;}
    else if(ecode==1) {printf("oper:discard the '\"'\n");return 0;}
    else if(ecode==2) {syskip();skip();return 0;}
    else if(ecode==3) {printf("oper:discard the '\''\n");return 0;}
    else if(ecode==4) {printf("oper:exit the program\n");return 1;}
    else if(ecode==5) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==6) {printf("oper:not enter redefined names\n");return 0;}
    else if(ecode==7) {printf("oper:const def error,exit the program\n");return 1;}
    else if(ecode==8) {printf("oper:func def error,exit the program\n");return 1;}
    else if(ecode==9) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==10) {printf("oper:ignore the lack\n");return 0;}
    else if(ecode==11) {printf("oper:ignore func name not exist\n");return 0;}
    else if(ecode==12) {printf("oper:ignore later parameters\n");return 0;}
    else if(ecode==13) {printf("assign the later paramenters into the value of the last one\n");return 0;}
    else if(ecode==14) {printf("ignore the lacking ']'\n");return 0;}
    else if(ecode==15) {printf("cannot call func or read array,oper:exit the program\n");return 1;}
    else if(ecode==16) {printf("ignore the lacking ')'\n");return 0;}
    else if(ecode==17) {printf("ignore the erroneous assignment\n");return 0;}
    else if(ecode==18) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==19) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==20) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==21) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==22) {syskip();skip();return 0;}
    else if(ecode==23) {printf("cannot call func or read array,oper:exit the program\n");return 1;}
    else if(ecode==24) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==25) {syskip();skip();return 0;}
    else if(ecode==26) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==27) {printf("oper:there are extra codes, exit the program\n");return 1;}
    else if(ecode==28) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==29) {syskip();skip();return 0;}
    else if(ecode==30) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==31) {printf("oper:exit the program\n");return 1;}//!!
    else if(ecode==-1) {printf("oper:exit the program\n");return 1;}//!!
}
#endif //COMPILER_ERROR_H
