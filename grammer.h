//
// Created by noble6emc2 on 2017/11/29.
//

#ifndef COMPILER_GRAMMER_H
#define COMPILER_GRAMMER_H
//#include "symbol0.h"
//#include "error.h"
//#include "genmid.h"
//constant number;
bool constant(int *res,bool *ischar){
    int sign=1;
    if(symbol==MINUSSY||symbol==PLUSSY){
        *ischar=0;
        if(symbol==MINUSSY) sign=-1;
        getsym();
        if(symbol==INTCON)
            if(sign==-1)
                *res=sign*num;
            else
                *res=num;
        else{
            bool exit1=error(28,w_count,temp);//constreadserr
            if(exit1) return 0;
        }
    }
    else if(symbol==INTCON){
        *ischar=0;
        *res=num;
    }
    else if(symbol==CHARCON){
        *ischar=1;
        *res=num;
    }
    else{
        bool exit1=error(28,w_count,temp);//constreaderr，错误类型应为变量
        if(exit1) return 0;
    }
    getsym();//next symbol!!!!
    return 1;
}
bool varlist(int paranum, int fundir);
bool expression(int lev, char *name);
//26.＜因子＞ ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
bool factor(int lev,char *name){
    //printf("enter factor\n");
    bool ischar;
    int res;
    if(symbol==PLUSSY||symbol==MINUSSY||symbol==INTCON){
        if(!constant(&res,&ischar)){
            bool exit1=error(28,w_count,temp);//constreaderr
            if(exit1) return 0;
        }
        //
        if(!entertemp(name,INT)){
            bool exit1=error(28,w_count,temp);//constreaderr
            if(exit1) return 0;
        }
        int loc=findtable(name);
        if(loc==-1){
            bool exit1=error(5,w_count,temp);//name not exist
            if(exit1) return 0;
        }
        char num[30];
        sprintf(num,"%d",res);
        emit3("=",maintab.ele[maintab.loc][loc].name,num,"1");//左边为常数则为1;
    }
    else if(symbol==CHARCON){
        ischar=1;
        res=num;
        getsym();//next symbol;
        //
        if(!entertemp(name,CHAR)){
            bool exit1=error(28,w_count,temp);//constreaderr
            if(exit1) return 0;
        }
        int loc=findtable(name);
        if(loc==-1){
            bool exit1=error(5,w_count,temp);//name not exist
            if(exit1) return 0;
        }
        char num[30];
        sprintf(num,"%d",res);
        emit3("=",maintab.ele[maintab.loc][loc].name,num,"1");//左边为常数则为1;
    }
    else if(symbol==IDSY){
        int idloc=findtable(token);
        int idloc1=maintab.loc;
        if(idloc==-1){
            bool exit1=error(5,w_count,temp);//name not exist
            if(exit1) return 0;
        }
        int kind=maintab.ele[idloc1][idloc].kind;
        int value=maintab.ele[idloc1][idloc].ret;
        int paranum=maintab.ele[idloc1][idloc].paranum;
        int fundir=maintab.ele[idloc1][idloc].fundir;
        char temp_n[200];
        //
        getsym();
        if(symbol==LBRASY){
            if(!(kind==INTARR||kind==CHRARR)){

                bool exit1=error(9,w_count,temp);
                if(exit1) return 0;
            }
            //
            getsym();
            if(!expression(1,temp_n)) return 0;
            if(symbol==RBRASY) getsym();
            else{
                bool exit1=error(14,w_count,temp);//lack rbrasy
                if(exit1) return 0;
            }
            //
            int ret;
            if(kind==INTARR)
                ret=entertemp(name,INT);
            else
                ret=entertemp(name,CHAR);
            if(ret==-1){
                bool exit1=error(21,w_count,temp);//expression bool exit1=error
                if(exit1) return 0;
            }
            int loc=findtable(name);
            if(loc==-1){
                bool exit1=error(5,w_count,temp);//name not exist
                if(exit1) return 0;
            }
            char num[30];
            sprintf(num,"%d",res);
            emit3("=[]",maintab.ele[maintab.loc][loc].name,maintab.ele[idloc1][idloc].name,temp_n);//左边为常数则为1;
        }//array;
        else if(symbol==LPARSY){
            if(kind!=FUNC||value==0){
                bool exit1=error(9,w_count,temp);
                if(exit1) return 0;
            }
            //
            getsym();
            //getchar();
            if(!varlist(paranum,fundir)){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }
            //getchar();
            if(symbol==RPARSY) getsym();
            else{
                bool exit1=error(16,w_count,temp);//lack rpar
                if(exit1) return 0;
            }
            //
            int ret;
            if(value==INT)
                ret=entertemp(name,INT);
            else
                ret=entertemp(name,CHAR);
            if(ret==-1){
                bool exit1=error(21,w_count,temp);//expression bool exit1=error
                if(exit1) return 0;
            }
            int loc=findtable(name);
            if(loc==-1){
                bool exit1=error(5,w_count,temp);//name not exist
                if(exit1) return 0;
            }
            char num[30];
            sprintf(num,"%d",res);
            emit1("call",maintab.ele[idloc1][idloc].name);//左边为常数则为1;
            emit1("v=",name);
        }//call function;
        else if(kind==FUNC||kind==INTARR||kind==CHRARR){
            bool exit1;
            if(kind==FUNC) exit1=error(15,w_count,temp);// lack '(';
            else if(kind==INTARR||kind==CHRARR) exit1=error(23,w_count,temp);//lack '[';
            if(exit1) return 0;
        }
        else strcpy(name,token);//标识符直接进行传递
    }
    else if(symbol==LPARSY){
        getsym();
        if(!expression(lev+1,name)) return 0;//读取中间变量
        if(symbol==RPARSY){
            getsym();
        }
        else{
            bool exit1=error(16,w_count,temp);//lack rpar
            if(exit1) return 0;
        }
    }//(expression)
    //getsym();
    printf("factor is read\n");
    return 1;
}
//25.＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
bool item(int lev, char *name){
    //printf("enter item\n");
    char id[200];
    if(!factor(lev,id)){
        bool exit1=error(21,w_count,temp);//expression bool exit1=error
        if(exit1) return 0;
    }
    //
    int faloc=findtable(id);
    int faloc1=maintab.loc;
    int kind=maintab.ele[faloc1][faloc].kind;
    if(kind==CONSTCHR) kind=CHAR;
    if(kind==CONSTINT) kind=INT;
    if(!entertemp(name,kind)){
        bool exit1=error(21,w_count,temp);//expression bool exit1=error
        if(exit1) return 0;
    }
    emit3("=",name,id,"0");//t=0;
    char temp_n[200];
    //
    while(symbol==DIVISY||symbol==STARSY){
        int oper=symbol;
        getsym();
        if(!factor(lev,temp_n)){
            bool exit1=error(21,w_count,temp);//expression bool exit1=error
            if(exit1) return 0;
        }
        //
        faloc=findtable(temp_n);
        faloc1=maintab.loc;
        if(maintab.ele[faloc1][faloc].kind!=CONSTCHR&&kind!=maintab.ele[faloc1][faloc].kind&&kind==CHAR){
            int loc=findtable(name);
            int loc1=maintab.loc;
            maintab.ele[loc1][loc].kind=INT;
            kind=INT;
        }
        if(oper==DIVISY) emit3("/",name,name,temp_n);
        else emit3("*",name,name,temp_n);
    }
    printf("item is read\n");
    return 1;
}
//24.＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
bool expression(int lev, char *name){
    //printf("expression level:%d\n",lev);
    if(lev>MAXEXPLEVEL){
        printf("expression level exceeded\n");
        bool exit1=error(21,w_count,temp);//expression bool exit1=error
        if(exit1) return 0;
    }
    int oper=-1;
    char temp_n[200];
    if(symbol==PLUSSY||symbol==MINUSSY){
        oper=symbol;
        getsym();
    }
    if(!item(lev,temp_n)){
        bool exit1=error(21,w_count,temp);//expression bool exit1=error
        if(exit1) return 0;
    }
    //
    int faloc=findtable(temp_n);
    int faloc1=maintab.loc;
    int kind=maintab.ele[faloc1][faloc].kind;
    if(!entertemp(name,kind)){
        bool exit1=error(21,w_count,temp);//expression bool exit1=error
        if(exit1) return 0;
    }
    emit3("=",name,"0","1");//t=0;
    if(oper==MINUSSY) emit3("-",name,name,temp_n);
    else emit3("+",name,name,temp_n);
    //
    while(symbol==PLUSSY||symbol==MINUSSY){
        oper=symbol;
        getsym();
        if(!item(lev,temp_n)){
            bool exit1=error(21,w_count,temp);//expression bool exit1=error
            if(exit1) return 0;
        }
        //
        faloc=findtable(temp_n);
        faloc1=maintab.loc;
        if(kind!=maintab.ele[faloc1][faloc].kind&&kind==CHAR){
            int loc=findtable(name);
            int loc1=maintab.loc;
            maintab.ele[loc1][loc].kind=INT;
            kind=INT;
        }
        if(oper==PLUSSY)
            emit3("+",name,name,temp_n);//+;
        else if(oper==MINUSSY)
            emit3("-",name,name,temp_n);//-;
    }
    printf("expression has been read\n");
    //getsym();
    return 1;
}
//10.＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
bool constdef();
bool conststatement(){
    //printf("enter const sta\n");
    if(symbol==CONSTSY){
        getsym();
        if(!constdef()){
            bool exit1=error(7,w_count,temp);//const def bool exit1=error
            if(exit1) return 0;
        }
        if(symbol!=SEMISY){
            bool exit1=error(10,w_count,temp);//lack semi
            if(exit1) return 0;
        }
        else getsym();
        //printf("const sta continue\n");
        return 1;
    }//const;
    printf("constant statement is read\n");
    return 0;
}

//11.＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
//| char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
bool constdef(){
    int res;
    bool ischar;
    //
    char name[200];
    int kind;
    //
    if(symbol==INTSY){
        kind=CONSTINT;//
        getsym();
        if(symbol!=IDSY){
            bool exit1=error(7,w_count,temp);//const def bool exit1=error
            if(exit1) return 0;
        }
        strcpy(name,token);//name=token;
        if(!strcmp(name,"main")){
            bool exit1=error(31,w_count,temp);
            if(exit1) return 0;
        }//cannot be main
        if(!entertable(name,kind,4,-1,-1)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }//常量入表
        getsym();
        if(symbol!=ASSIGNSY){
            bool exit1=error(7,w_count,temp);//const def bool exit1=error
            if(exit1) return 0;
        }
        getsym();
        if(!constant(&res,&ischar)){
            sytop=-1;
            skiptop=-1;
            sytar[++sytop]=INTCON;
            bool exit1=error(29,w_count,temp);//lack const
            if(exit1) return 0;
        }
        char temp_num[20];
        if(ischar){
            bool exit1=error(17,w_count,temp);//lack intcon
            if(exit1) return 0;
        }
        else{
            sprintf(temp_num,"%d",res);
            emit3("=",name,temp_num,"1");//name=num;
        }
        while(symbol==COMMASY){
            getsym();
            if(symbol!=IDSY){
                bool exit1=error(7,w_count,temp);//const def bool exit1=error
                if(exit1) return 0;
            }
            strcpy(name,token);//name=token;
            if(!strcmp(name,"main")){
                bool exit1=error(31,w_count,temp);
                if(exit1) return 0;
            }//cannot be main
            if(!entertable(name,kind,4,-1,-1)){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }//常量入表
            getsym();
            if(symbol!=ASSIGNSY){
                bool exit1=error(7,w_count,temp);//conset def bool exit1=error
                if(exit1) return 0;
            }
            getsym();
            if(!constant(&res,&ischar)){
                sytop=-1;
                skiptop=-1;
                sytar[++sytop]=INTCON;
                bool exit1=error(29,w_count,temp);//to the next intcon
                if(exit1) return 0;
            }
            if(ischar){
                bool exit1=error(17,w_count,temp);
                if(exit1) return 0;
            }
            else{
                sprintf(temp_num,"%d",res);
                emit3("=",name,temp_num,"1");//name=num;
            }
        }
    }
    else if(symbol==CHARSY){
        kind=CONSTCHR;
        getsym();
        if(symbol!=IDSY){
            bool exit1=error(7,w_count,temp);
            if(exit1) return 0;
        }
        strcpy(name,token);//name=token;
        if(!strcmp(name,"main")){
            bool exit1=error(31,w_count,temp);
            if(exit1) return 0;
        }//cannot be main;
        if(!entertable(name,kind,4,-1,-1)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }//常量入表
        getsym();
        if(symbol!=ASSIGNSY){
            bool exit1=error(7,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        if(!constant(&res,&ischar)){
            sytop=-1;
            skiptop=-1;
            sytar[++sytop]=CHARCON;
            bool exit1=error(29,w_count,temp);
            if(exit1) return 0;
        }
        if(!ischar){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        char temp_num[20];
        sprintf(temp_num,"%d",res);
        emit3("=",name,temp_num,"1");//name=num;
        while(symbol==COMMASY){
            getsym();
            if(symbol!=IDSY){
                bool exit1=error(7,w_count,temp);
                if(exit1) return 0;
            }
            strcpy(name,token);//name=token;
            if(!strcmp(name,"main")){
                bool exit1=error(31,w_count,temp);
                if(exit1) return 0;
            }//cannot be main;
            if(!entertable(name,kind,4,-1,-1)){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }//常量入表
            getsym();
            if(symbol!=ASSIGNSY){
                bool exit1=error(7,w_count,temp);
                if(exit1) return 0;
            }
            getsym();
            if(!constant(&res,&ischar)){
                sytop=-1;
                skiptop=-1;
                sytar[++sytop]=CHARCON;
                bool exit1=error(29,w_count,temp);
                if(exit1) return 0;
            }
            if(!ischar){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }
            sprintf(temp_num,"%d",res);
            emit3("=",name,temp_num,"1");//name=num;
        }
    }
    else{
        bool exit1=error(7,w_count,temp);
        if(exit1) return 0;
    }
    printf("constant definition is read\n");
    //getsym();
    return 1;
}
bool vardef(int kind){//17.＜变量定义＞  ::=
    //＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’) }
    char name[200];
    while(symbol==COMMASY){
        getsym();
        if(symbol!=IDSY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        strcpy(name,token);//name=token;
        if(!strcmp(name,"main")){
            bool exit1=error(31,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        if(symbol==LBRASY){
            int res;
            bool ischar;
            getsym();
            if(!constant(&res,&ischar)){
                sytop=-1;
                skiptop=-1;
                sytar[++sytop]=INTCON;
                bool exit1=error(29,w_count,temp);
                if(exit1) return 0;
            }
            if(ischar||res<0||symbol!=RBRASY){
                bool exit1=error(-1,w_count, temp);
                if(exit1) return 0;
            }
            int ret;
            if(kind==INT) ret=entertable(name,INTARR,4*res,-1,-1);//数组入表
            else ret=entertable(name,CHRARR,4*res,-1,-1);
            if(ret==-1){
                bool exit1=error(-1,w_count,temp);
            }
            getsym();
        }
        else if(!entertable(name,kind,4,-1,-1)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }//变量入表
    }
    printf("multiple variable statement in a sentence is read\n");
    return 1;
}
bool sentences(int lev);
bool paralist(int loc1, int loc);
//16.＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
//19.＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数表＞‘)’ ‘{’＜复合语句＞‘}’
//20.＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数表＞‘)’‘{’＜复合语句＞‘}’
bool compoundsentences(int lev);
bool mainfunc(int lev);
bool endwithid=0;
bool varstatement(){
    //printf("enter variable statement\n");
    endwithid=0;
    char name[200];
    int kind;
    if(symbol==INTSY||symbol==CHARSY){
        kind=symbol;
        if(kind==INTSY) kind=INT;
        else kind=CHAR;
        getsym();
        if(symbol!=IDSY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        strcpy(name,token);
        if(!strcmp(name,"main")){
            bool exit1=error(31,w_count,temp);
            if(exit1) return 0;
        }//cannot be main;
        getsym();
        if(symbol==SEMISY){
            printf("variable statement is read\n");
            getsym();
            if(!entertable(name,kind,4,-1,-1)){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }//enter variable;
            return 1;
        }
        else if(symbol==COMMASY){
            if(!entertable(name,kind,4,-1,-1)){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }//enter variable;
            if(!vardef(kind)){//vardef has taken care of midgen.
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }
            if(symbol!=SEMISY){
                bool exit1=error(10,w_count,temp);//semi
                if(exit1) return 0;
            }//分号
            else getsym();
            printf("variable statement is read\n");
            return 1;
        }
        else if(symbol==LBRASY){
            int res;
            bool ischar;
            getsym();
            if(!constant(&res,&ischar)){
                sytop=-1;
                skiptop=-1;
                sytar[++sytop]=INTCON;
                bool exit1=error(29,w_count,temp);
                if(exit1) return 0;
            }
            if(ischar||res<0||symbol!=RBRASY){
                bool exit1=error(-1,w_count, temp);
                if(exit1) return 0;
            }
            int ret;
            if(kind==INT) ret=entertable(name,INTARR,4*res,-1,-1);
            else ret=entertable(name,CHRARR,4*res,-1,-1);// enter array;
            if(ret==-1){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }
            getsym();
            if(!vardef(kind)){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }
            if(symbol!=SEMISY){
                bool exit1=error(10,w_count,temp);
                if(exit1) return 0;
            }//分号
            else getsym();
            printf("variable statement is read\n");
            return 1;
        }//没有识别分号
        else{
            endwithid=1;//symbol前是id;
            strcpy(temp_id,token);//暂时存储token;
            temp_sym=kind;//暂存kind;
            //printf("variable statement out\n");
            return 0;
        }
    }
    //printf("variable statement out\n");
    return 0;
}

bool funcstatement(){
    int paranum;
    //printf("enter function statement\n");
    if(endwithid&&symbol!=LPARSY){
        endwithid=0;
        bool exit1=error(8,w_count,temp);//func state bool exit1=error
        if(exit1) return 0;
    }//识别变量后标识符只能跟（;
    if(symbol==LPARSY&&endwithid){
        endwithid=0;//只能用一次
        getsym();
        //
        if(!entertable(temp_id,FUNC,0,temp_sym,-1)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        int idloc=findtable(temp_id);
        int idloc1=maintab.loc;
        //
        if(!paralist(idloc1,idloc)||symbol!=RPARSY){//process arguments;
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        emit1("lab",temp_id);//set a label;
        getsym();
        if(symbol!=LCBRASY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        emit0("swra");//存取返回地址;
        if(!compoundsentences(1)||symbol!=RCBRASY){
            bool exit1=error(8,w_count,temp);
            //printf("bool exit1=error\n");
            if(exit1) return 0;
        }
        printf("variable function statement is read\n");
        emit0("ret0");//默认返回零
        maintab.pointer=0;//back to main layer;
        emit1("swtpoi","0");
        getsym();
        return 1;
    }//函数不用识别分号
    else if(symbol==INTSY||symbol==CHARSY){
        int kind=symbol;
        char name[200];
        if(kind==INTSY) kind=INT;
        else kind=CHAR;
        getsym();
        if(symbol!=IDSY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        strcpy(name,token);
        if(!strcmp(name,"main")){
            bool exit=error(31,w_count,temp);
            if(exit) return 0;
        }
        getsym();
        if(symbol!=LPARSY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        if(!entertable(name,FUNC,0,kind,-1)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }//enter function;
        int idloc=findtable(name);
        int idloc1=maintab.loc;
        getsym();
        if(!paralist(idloc1,idloc)||symbol!=RPARSY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        emit1("lab",name);//set a label;
        getsym();
        if(symbol!=LCBRASY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        emit0("swra");//存取返回地址;
        getsym();
        if(!compoundsentences(1)||symbol!=RCBRASY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        emit0("ret0");//默认返回0
        maintab.pointer=0;//back to main layer;
        emit1("swtpoi","0");
        printf("variable function statement is read\n");
        getsym();
        return 1;
    }
    else if(symbol==VOIDSY){
        char name[200];
        getsym();
        if(symbol!=IDSY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        strcpy(name,token);
        if(!entertable(name,FUNC,0,0,-1)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }//enter a function;(要严格处理？？？)
        int idloc=findtable(name);
        int idloc1=maintab.loc;
        if(!strcmp(token,"main")){
            maintab.ele[idloc1][idloc].paranum=0;//main has no argument;
            maintab.pointer=maintab.ftotal;//to the func layer;
            char poi[100];
            sprintf(poi,"%d",maintab.pointer);
            emit1("swtpoi",poi);
            emit1("lab",name);//label;
            emit0("swra");//存取返回地址;
            getsym();
            if(!mainfunc(1)){
                //printf("section 4\n");
                bool exit1=error(8,w_count,temp);
                if(exit1) return 0;
            }
            emit0("ret");
            printf("symbol at the end:%d\n",symbol);
            if(symbol!=-1){
                printf("there is something unwanted in the end.\n");
                bool exit1=error(8,w_count,temp);
                if(exit1) return 0;
            }
            maintab.pointer=0;//back to main layer;
            emit1("swtpoi","0");
            return 1;//main ended correctly;
        }
        getsym();
        if(symbol!=LPARSY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        if(!paralist(idloc1,idloc)||symbol!=RPARSY){
            //printf("RPARSY bool exit1=error\n");
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        emit1("lab",name);//set a label;
        getsym();
        if(symbol!=LCBRASY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        emit0("swra");//存取返回地址;
        getsym();
        if(!compoundsentences(1)||symbol!=RCBRASY){
            bool exit1=error(8,w_count,temp);
            if(exit1) return 0;
        }
        emit0("ret");//默认退出
        /*if(blevel==0){
            blevel--;
            sp=STACKLIM+maintab.memnum;
            maintab.pointer=0;
        }
        else if(blevel>0){
            blevel--;
            sp=baseaddr[blevel]-4;
            maintab.pointer=basepointer[blevel];
        }
        else{
            sp=STACKLIM+maintab.memnum;
            maintab.pointer=0;
        }*/
        maintab.pointer=0;//back to main layer;
        emit1("swtpoi","0");
        printf("void function statement is read\n");
        getsym();
        return 1;
    }
    return 0;
}
//21.＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞//
bool compoundsentences(int lev){
    while(symbol==CONSTSY){
        if(!conststatement()){//const statement already taken care.
            bool exit1=error(7,w_count,temp);
            if(exit1) return 0;
        }
    }
    printf("compound constant statement is read\n");
    while(symbol==INTSY||symbol==CHARSY){
        if(!varstatement()){//变量声明funcallow为0;//var state taken care
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
    }
    printf("compound variable statement is read\n");
    if(!sentences(lev)){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    printf("compound sentences are read\n");
    return 1;
}
bool paralist(int loc1,int loc){
    maintab.pointer=maintab.ftotal;//to the func layer!!!;
    char poi[100];
    sprintf(poi,"%d",maintab.pointer);
    emit1("swtpoi",poi);
    int paranum=0;
    if(loc==-1){
        bool exit1=error(11,w_count,temp);
        if(exit1) return 0;
        return 1;
    }
    if(symbol==RPARSY){
        printf("empty paralist is read\n");
        maintab.ele[loc1][loc].paranum=paranum;//para num=0;
        return 1;
    }
    if(symbol!=CHARSY&&symbol!=INTSY){
        sytop=-1;
        skiptop=-1;
        sytar[++sytop]=CHARSY;
        sytar[++sytop]=INTSY;
        bool exit1=error(2,w_count,temp);
        if(exit1) return 0;
    }
    int kind=symbol;
    if(kind==INTSY) kind=INT;
    else kind=CHAR;
    getsym();
    if(symbol!=IDSY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    if(!entertable(token,kind,4,-1,-1)){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }//enter var;
    paranum++;
    getsym();
    while(symbol==COMMASY){
        getsym();
        if(symbol!=CHARSY&&symbol!=INTSY){
            sytop=-1;
            skiptop=-1;
            sytar[++sytop]=CHARSY;
            sytar[++sytop]=INTSY;
            bool exit1=error(2,w_count,temp);
            if(exit1) return 0;
        }
        kind=symbol;
        if(kind==INTSY) kind=INT;
        else kind=CHAR;
        getsym();
        if(symbol!=IDSY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        if(!entertable(token,kind,4,-1,-1)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }//enter var;
        paranum++;
        getsym();
    }
    maintab.ele[loc1][loc].paranum=paranum;//record func para number;
    printf("paralist is read\n");
    //getsym();
    return 1;
}
bool readmain=0;
bool mainfunc(int lev){
    readmain=1;
    if(symbol!=LPARSY){
        bool exit1=error(15,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(symbol!=RPARSY){
        bool exit1=error(16,w_count,temp);
        if(exit1) return 0;
    }
    else getsym();
    if(symbol!=LCBRASY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(!compoundsentences(lev)||symbol!=RCBRASY) {
        bool exit1=error(8,w_count, temp);
        if(exit1) return 0;
    }
    printf("mainfunc is read\n");
    getsym();
    return 1;
}//
bool whilstatement(int lev);
bool ifstatement(int lev);
bool switchstatement(int lev);
bool callassignstatement(int lev);
bool printstatement();
bool scanfstatement();
bool retstatement();
bool sentence(int lev){
    temp_num=0;//中间变量清0;
    if(symbol==IFSY){
        //printf("enter if state\n");
        if(!ifstatement(lev)) {
            bool exit1=error(-1,w_count, temp);
            if(exit1) return 0;
        }
        printf("sentence:if is read\n");
        return 1;
    }
    else if(symbol==DOSY){
        if(!whilstatement(lev)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        printf("sentence:while is read\n");
        return 1;
    }
    else if(symbol==SWTSY){
        if(!switchstatement(lev)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        printf("sentence:switch is read\n");
        return 1;
    }
    else if(symbol==LCBRASY){
        getsym();
        if(!sentences(lev+1)||symbol!=RCBRASY){//考虑嵌套层次；
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        printf("sentence:{sentences} is read\n");
        return 1;
    }
    else if(symbol==IDSY){
        //printf("enter callassign\n");
        if(!callassignstatement(lev)||symbol!=SEMISY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        //printf("out of callassign\n");
        getsym();
        printf("sentence:assignment is read\n");
        return 1;
    }
    else if(symbol==PRINTF){
        if(!printstatement()||symbol!=SEMISY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        printf("sentence:print is read\n");
        return 1;
    }
    else if(symbol==SCANF){
        if(!scanfstatement()||symbol!=SEMISY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        printf("sentence:scanf is read\n");
        return 1;
    }
    else if(symbol==RETSY){
        if(!retstatement()||symbol!=SEMISY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        printf("sentence:ret is read\n");
        return 1;
    }
    else if(symbol==SEMISY){
        getsym();
        printf("sentence:; is read\n");
        return 1;
    }
    return 0;
}
int lab_num=0;//can not be 0 again;
int endlab_num=0;
bool casestatement(int lev, char *name, char *endlab){
    char lab[100];
    char midname[100];
    if(symbol==CASESY){
        int res;
        bool ischar;
        getsym();
        if(!constant(&res,&ischar)||symbol!=COLONSY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        if(ischar){
            if(!entertemp(midname,CHAR)){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }
        }
        else if(!entertemp(midname,INT)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        //
        char temp_num[20];
        sprintf(temp_num,"%d",res);
        sprintf(lab,"lab%d",lab_num++);
        emit3("=",midname,temp_num,"1");//t=num;
        emit3("bne",lab,midname,name);//
        //printf("case level is %d\n",lev);
        if(!sentence(lev)){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        //sprintf(lab,"endlab%d",endlab_num);
        emit1("jmp",endlab);
        //sprintf(lab,"lab%d",lab_num);
        emit1("lab",lab);
        //lab_num++;
        printf("a case is read\n");
        return 1;
    }
    return 0;
}
bool caselist(int lev, char *name){
    char lab[200];
    sprintf(lab,"endlab%d",endlab_num++);//end label number +=1;
    if(!casestatement(lev,name,lab)){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    while(casestatement(lev,name,lab));
    emit1("lab",lab);//
    //endlab_num++;//
    printf("caselist is read\n");
    return 1;
}
bool switchstatement(int lev){
    char expres[200];
    if(symbol!=SWTSY){
        sytop=-1;
        skiptop=-1;
        sytar[++sytop]=SWTSY;
        bool exit1=error(2,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(symbol!=LPARSY){
        bool exit1=error(15,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(!expression(1,expres)||symbol!=RPARSY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(symbol!=LCBRASY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(!caselist(lev+1,expres)||symbol!=RCBRASY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    printf("case statement is read\n");
    return 1;
}
bool varlist(int paranum, int fundir){
    char name[200];
    int num=0;
    if(symbol!=RPARSY){
        if(!expression(1,name)){
            bool exit1=error(21,w_count,temp);
            if(exit1) return 0;
        }
        if(num>=paranum){
            bool exit1=error(12,w_count,temp);
            if(exit1) return 0;
            return 1;
        }
        int loc=findtable(name);
        int loc1=maintab.loc;
        int kind=maintab.ele[loc1][loc].kind;
        if((kind==CONSTINT||kind==INT)&&maintab.ele[fundir][num].kind==CHAR)
            printf("warning at line %d: entering int into char arguments\n",errline);
        emit1("push",name);
        num++;
        //system("pause");
        while(symbol==COMMASY) {
            getsym();
            if (!expression(1,name)) {
                bool exit1=error(21,w_count, temp);
                if(exit1) return 0;
            }
            if(num>=paranum){
                bool exit1=error(12,w_count,temp);
                if(exit1) return 0;
                return 1;
            }
            loc=findtable(name);
            loc1=maintab.loc;
            kind=maintab.ele[loc1][loc].kind;
            if((kind==CONSTINT||kind==INT)&&maintab.ele[fundir][num].kind==CHAR)
                printf("warning at line %d: entering int into char arguments\n",errline);
            emit1("push",name);
            num++;
        }
    }
    if(num<paranum){
        bool exit1=error(13,w_count,temp);
        if(exit1) return 0;
        for(int a=num;a<=paranum;a++)
            emit1("push",name);
    }
    printf("a varlist is read\n");
    return 1;
}
bool callassignstatement(int lev){
    char name[200];
    char id[200];
    if(symbol!=IDSY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    strcpy(id,token);
    getsym();
    if(symbol==LBRASY){
        int loc=findtable(id);
        int loc1=maintab.loc;
        int kind=maintab.ele[loc1][loc].kind;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }//cannot find id;
        if(kind!=CHRARR&&kind!=INTARR){
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
        }
        char exp[200];
        getsym();
        if(!expression(1,name)||symbol!=RBRASY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        if(symbol!=ASSIGNSY){
            sytop=-1;
            skiptop=-1;
            skiptar[++skiptop]='=';//to the new '=';
            bool exit1=error(25,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        if(!expression(1,exp)){
            bool exit1=error(21,w_count,temp);
            if(exit1) return 0;
        }
        int eloc=findtable(exp);
        int eloc1=maintab.loc;
        int ekind=maintab.ele[eloc1][eloc].kind;
        if(kind==CHRARR&&(ekind==INT||ekind==CONSTINT))
            printf("warning at line %d: assign int to char\n",errline);
        emit3("[]=",id,name,exp);//assignment;
    }
    else if(symbol==ASSIGNSY){
        getsym();
        if(!expression(1,name)){
            bool exit1=error(21,w_count,temp);
            if(exit1) return 0;
        }
        int loc=findtable(id);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }//cannot find id;
        int kind=maintab.ele[loc1][loc].kind;
        if(kind==CONSTCHR||kind==CONSTINT){
            skiptop=-1;
            sytop=-1;
            skiptar[++skiptop]=';';//skip to next line;
            bool exit1=error(22,w_count,temp);
            if(exit1) return 0;
        }
        else if(kind!=CHAR&&kind!=INT){
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
        }
        int nloc=findtable(name);
        int nloc1=maintab.loc;
        int nkind=maintab.ele[nloc1][nloc].kind;
        if(nloc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        //printf("nkind:%d\n",nkind);
        if(kind==CHAR&&(nkind==INT||nkind==CONSTINT))
            printf("warning at line %d: assign int to char\n",errline);
        if(nkind!=INT&&nkind!=CHAR&&nkind!=CONSTCHR&&nkind!=CONSTINT){
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
        }//in this compiler an int can be assigned to a char variable;
        emit3("=",id,name,"0");
    }
    else if(symbol==LPARSY){
        getsym();
        if(!strcmp(id,"main")){
            bool exit1=error(30,w_count,temp);
            if(exit1) return 0;
        }
        int loc=findtable(id);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }
        int kind=maintab.ele[loc1][loc].kind;
        int ret=maintab.ele[loc1][loc].ret;
        int paranum=maintab.ele[loc1][loc].paranum;
        int fundir=maintab.ele[loc1][loc].fundir;
        if(kind!=FUNC){
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
        }
        if(!varlist(paranum,fundir)||symbol!=RPARSY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        getsym();
        emit1("call",id);
    }
    else return 0;//lack of symbol;
    printf("an assignment or function calling is read\n");
    return 1;
}
bool sentences(int lev){
    //printf("level is %d\n",lev);
    if(lev>MAXLEVEL){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    while(sentence(lev));
    printf("sentences is read\n");
    return 1;
}
bool condition(char *lab){
    char a[200];
    char b[200];
    if(!expression(0,a)){
        bool exit1=error(21,w_count,temp);
        if(exit1) return 0;
    }
    if(symbol>=EQLSY&&symbol<=LEQSY){
        int oper=symbol;
        getsym();
        if(!expression(0,b)){
            bool exit1=error(21,w_count,temp);
            if(exit1) return 0;
        }
        switch(oper){
            case EQLSY: emit3("beq",lab,a,b);break;
            case NEQSY: emit3("bne",lab,a,b);break;
            case GTRSY: emit3("bgr",lab,a,b);break;
            case GEQSY: emit3("bge",lab,a,b);break;
            case LSSSY: emit3("bls",lab,a,b);break;
            case LEQSY: emit3("ble",lab,a,b);break;
        }
    }//逻辑运算符
    else emit2("bnez",lab,a);
    printf("condition is read\n");
    return 1;
}
bool whilstatement(int lev){
    if(symbol!=DOSY){
        sytop=-1;
        skiptop=-1;
        sytar[++sytop]=DOSY;
        bool exit1=error(2,w_count,temp);
        if(exit1) return 0;
    }
    char lab[200];
    sprintf(lab,"lab%d",lab_num++);
    emit1("lab",lab);
    getsym();
    if(!sentence(lev)||symbol!=WHILSY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(symbol!=LPARSY){
        bool exit1=error(15,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(!condition(lab)||symbol!=RPARSY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    printf("while statement is read\n");
    getsym();
    return 1;
}
bool ifstatement(int lev){//function level;
    char lab[200];
    char end[200];
    if(symbol!=IFSY){
        sytop=-1;
        skiptop=-1;
        sytar[++sytop]=IFSY;
        bool exit1=error(2,w_count,temp);
        if(exit1) return 0;
    }
    sprintf(lab,"lab%d",lab_num++);
    sprintf(end,"endlab%d",endlab_num++);
    getsym();
    if(symbol!=LPARSY){
        bool exit1=error(15,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(!condition(lab)||symbol!=RPARSY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    emit1("jmp",end);
    emit1("lab",lab);
    getsym();
    //printf("out of condition\n");
    if(!sentence(lev)){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    emit1("lab",end);
    printf("if statement is read\n");
    return 1;
}
bool printstatement(){
    char name[200];
    if(symbol!=PRINTF){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(symbol!=LPARSY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(symbol==STRINGCON){
        emit2("prt",token,"1");//1输出字符串;
        getsym();
        if(symbol==RPARSY) getsym();
        else if(symbol==COMMASY){
            getsym();
            if(!expression(1,name)||symbol!=RPARSY){
                bool exit1=error(-1,w_count,temp);
                if(exit1) return 0;
            }
            int loc=findtable(name);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            char kind[100];
            sprintf(kind,"%d",maintab.ele[loc1][loc].kind);
            /*if(maintab.ele[loc1][loc].kind==FUNC&&maintab.ele[loc1][loc].kind==0){
                bool exit1=error(9,w_count,temp);
                if(exit1) return 0;
            }*/
            emit3("prt",name,"0",kind);//0输出变量
            getsym();
        }
    }
    else{
        if(!expression(1,name)||symbol!=RPARSY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        int loc=findtable(name);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }//cannot find name;
        char kind[100];
        sprintf(kind,"%d",maintab.ele[loc1][loc].kind);
        /*if(maintab.ele[loc1][loc].kind==FUNC&&maintab.ele[loc1][loc].kind==0){
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
        }*/
        emit3("prt",name,"0",kind);//0输出变量
        getsym();
    }
    printf("printf is read\n");
    return 1;
}
bool scanfstatement(){
    if(symbol!=SCANF){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(symbol!=LPARSY){
        bool exit1=error(15,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(symbol!=IDSY){
        sytop=-1;
        skiptop=-1;
        sytar[++sytop]=IDSY;
        bool exit1=error(2,w_count,temp);
        if(exit1) return 0;
    }
    int loc=findtable(token);
    int loc1=maintab.loc;
    if(loc==-1){
        bool exit1=error(5,w_count,temp);
        if(exit1) return 0;
    }//cannot find token;
    int kind=maintab.ele[loc1][loc].kind;
    if(kind!=INT&&kind!=CHAR){
        bool exit1=error(9,w_count,temp);
        if(exit1) return 0;
    }
    //if(loc==-1) printf("nooooooo %s\n",token);
    emit1("scf",token);
    getsym();
    while(symbol==COMMASY){
        getsym();
        if(symbol!=IDSY){
            sytop=-1;
            skiptop=-1;
            sytar[++sytop]=IDSY;
            bool exit1=error(2,w_count,temp);
            if(exit1) return 0;
        }
        loc=findtable(token);
        loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }//cannot find token;
        kind=maintab.ele[loc1][loc].kind;
        if(kind!=INT&&kind!=CHAR){
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
        }
        emit1("scf",token);
        getsym();
    }
    if(symbol!=RPARSY){
        bool exit1=error(14,w_count,temp);
        if(exit1) return 0;
    }
    else getsym();
    printf("scanf is read\n");
    return 1;
}
bool retstatement(){
    char name[200];
    if(symbol!=RETSY){
        bool exit1=error(-1,w_count,temp);
        if(exit1) return 0;
    }
    getsym();
    if(symbol==LPARSY){
        getsym();
        if(!expression(1,name)||symbol!=RPARSY){
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        int ret;
        for(int a=0;a<=maintab.index[0];a++)
            if(maintab.ele[0][a].fundir==maintab.pointer){
                ret=maintab.ele[0][a].ret;
                break;
            }//find the kind of this func;
        int loc=findtable(name);
        int loc1=maintab.loc;
        if(loc==-1){
            bool exit1=error(5,w_count,temp);
            if(exit1) return 0;
        }//cannot find token;
        int kind=maintab.ele[loc1][loc].kind;
        if(ret==CHAR&&(kind==INT||kind==CONSTINT))
            printf("warning at line %d: using int as a return value in char function\n",errline);
        if(ret==0){
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
        }// if it is void function;
        emit1("ret",name);
        //else emit1("jmp","mainend");
        getsym();
    }
    else{
        int ret;
        for(int a=0;a<=maintab.index[0];a++)
            if(maintab.ele[0][a].fundir==maintab.pointer){
                ret=maintab.ele[0][a].ret;
                break;
            }//find the kind of this func;
        if(ret!=0){
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
        }// if it is void function;
        emit0("ret");
    }
    //else emit1("jmp","mainend");
    printf("a return is read\n");
    return 1;
}
bool program(){
    getsym();
    while(conststatement());
    //printf("section 1\n");
    while(varstatement());
    //printf("section 2\n");
    emit1("call","main");
    emit1("jmp","mainend");
    while(funcstatement());
    //printf("section 3\n");
    if(!readmain){
        bool exit1=error(0,w_count,temp);
        if(exit1) return 0;
    }
    if(symbol!=-1){
        bool exit1=error(27,w_count,temp);
        if(exit1) return 0;
    }
    emit1("lab","mainend");
    return 1;
}
#endif //COMPILER_GRAMMER_H
