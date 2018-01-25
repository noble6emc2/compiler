//
// Created by noble6emc2 on 2017/12/14.
//

#ifndef COMPILER_OPTIM_H
#define COMPILER_OPTIM_H
//#include"genmid.h"
#define BBLOCK 1000
#define REGNUM 5
#define GLOREGTOP 7
#define LOCREGTOP 7
#define GLOREGSTR 19
#define LOCREGSTR 11
#define GLSIZE 64
typedef struct{
    int uplim=-1;//起始行数
    int downlim=-1;//结束行数
    int pre[100];
    int after[100];
    int pretop=-1;
    int afttop=-1;
}b_block;
typedef struct{
    char in[100][100];
    int intop=-1;
    char out[100][100];
    int outtop=-1;
    char use[100][100];
    int usetop=-1;
    char def[100][100];
    int deftop=-1;

}live_var_tab;
typedef struct{
    char name[30];
    int num;
    int gloreg=-1;
} confltnode;
confltnode node[100];
//char gloreg[32][100];
int glotop=-1;
char locreg[32][100];
int loctop=-1;
int btop=-1;
b_block bform[BBLOCK];
live_var_tab ltab[BBLOCK];
int confltmap[100][100];
int deflocreg[100];
//void optmips(int );
bool isjstru(int num){
    if(!strcmp(mcode[num].name,"jmp")) return 1;
    else if(!strcmp(mcode[num].name,"ret")) return 1;
    else if(!strcmp(mcode[num].name,"ret0")) return 1;
    return 0;
}
bool isprt(int num){
    if(!strcmp(mcode[num].name,"prt")) return 1;
    return 0;
}
bool isbranch(int num){
    if(!strcmp(mcode[num].name,"bne")) return 1;
    else if(!strcmp(mcode[num].name,"beq")) return 1;
    else if(!strcmp(mcode[num].name,"bgr")) return 1;
    else if(!strcmp(mcode[num].name,"bge")) return 1;
    else if(!strcmp(mcode[num].name,"bls")) return 1;
    else if(!strcmp(mcode[num].name,"ble")) return 1;
    else if(!strcmp(mcode[num].name,"bnez")) return 1;
    else if(!strcmp(mcode[num].name,"bez")) return 1;
    return 0;
}
bool islstru(int num){
    if(!strcmp(mcode[num].name,"lab")) return 1;
    return 0;
}
char *loc(int num){
    return mcode[num].a;
}
int finduse(int a,char *var);
int finddef(int a,char *var);
void insertuse(int a, char* var){
    if(finduse(a,var)!=-1)
        return;
    strcpy(ltab[a].use[++ltab[a].usetop],var);
}
void insertdef(int a, char* var){
    if(finddef(a,var)!=-1)
        return;
    strcpy(ltab[a].def[++ltab[a].deftop],var);
}
int finduse(int a,char *var){
    for(int b=0;b<=ltab[a].usetop;b++)
        if(!strcmp(ltab[a].use[b],var))
            return b;
    return -1;
}
int finddef(int a,char *var){
    for(int b=0;b<=ltab[a].deftop;b++)
        if(!strcmp(ltab[a].def[b],var))
            return b;
    return -1;
}
int findout(int a,char *var){
    for(int b=0;b<=ltab[a].outtop;b++)
        if(!strcmp(ltab[a].out[b],var))
            return b;
    return -1;
}
int findin(int a,char *var){
    for(int b=0;b<=ltab[a].intop;b++)
        if(!strcmp(ltab[a].in[b],var))
            return b;
    return -1;
}
void init_u_d(){
    for(int a=0;a<=btop;a++){
        ltab[a].usetop=-1;
        ltab[a].deftop=-1;
        for(int b=bform[a].uplim;b<=bform[a].downlim;b++)
            if(mcode[b].num==3&&strcmp(mcode[b].name,"SKIP")){
                if(isbranch(b)){
                    if(finddef(a,mcode[b].b)==-1)
                        insertuse(a,mcode[b].b);
                    if(finddef(a,mcode[b].c)==-1)
                        insertuse(a,mcode[b].c);
                }//use:b,c;def:a;
                else if(isprt(b)){
                    if(finddef(a,mcode[b].a)==-1)
                        insertuse(a,mcode[b].a);
                }//use:a;
                else if(!strcmp(mcode[b].name,"=")){
                    if(!strcmp(mcode[b].c,"0")){
                        if(finddef(a,mcode[b].b)==-1)
                            insertuse(a,mcode[b].b);
                    }//use first
                    if(finduse(a,mcode[b].a)==-1)
                        insertdef(a,mcode[b].a);
                }//def: a;use: b or not;
                else if(!strcmp(mcode[b].name,"=[]")){
                    if(finddef(a,mcode[b].c)==-1)
                        insertuse(a,mcode[b].c);
                    if(finduse(a,mcode[b].a)==-1)
                        insertdef(a,mcode[b].a);
                }//def: a;
                else if(!strcmp(mcode[b].name,"[]=")){
                    if(finddef(a,mcode[b].c)==-1)
                        insertuse(a,mcode[b].c);
                    if(finddef(a,mcode[b].b)==-1)
                        insertuse(a,mcode[b].b);
                }//use: c;
                else if(!strcmp(mcode[b].name,"/i")){
                    if(finddef(a,mcode[b].b)==-1)
                        insertuse(a,mcode[b].b);
                    if(finduse(a,mcode[b].a)==-1)
                        insertdef(a,mcode[b].a);
                }
                else if(!strcmp(mcode[b].name,"*i")){
                    if(finddef(a,mcode[b].b)==-1)
                        insertuse(a,mcode[b].b);
                    if(finduse(a,mcode[b].a)==-1)
                        insertdef(a,mcode[b].a);
                }
                else{
                    if(finddef(a,mcode[b].b)==-1)
                        insertuse(a,mcode[b].b);
                    if(finddef(a,mcode[b].c)==-1)
                        insertuse(a,mcode[b].c);
                    if(finduse(a,mcode[b].a)==-1)
                        insertdef(a,mcode[b].a);
                }//use: b,c;def: a;
            }
            else if(mcode[b].num==2&&strcmp(mcode[b].name,"SKIP")){
                if(!strcmp(mcode[b].name,"bnez")&&
                   finddef(a,mcode[b].b)==-1)
                    insertuse(a,mcode[b].b);
                if(!strcmp(mcode[b].name,"bez")&&
                   finddef(a,mcode[b].b)==-1)
                    insertuse(a,mcode[b].b);
            }//use:b;
            else if(mcode[b].num==1&&strcmp(mcode[b].name,"SKIP")){
                if(!strcmp(mcode[b].name,"ret")&&
                   finddef(a,mcode[b].a)==-1)
                    insertuse(a,mcode[b].a);//use:a;
                if(!strcmp(mcode[b].name,"push")&&
                   finddef(a,mcode[b].a)==-1)
                    insertuse(a,mcode[b].a);//use:a;
                if(!strcmp(mcode[b].name,"scf")&&
                   finduse(a,mcode[b].a)==-1)
                    insertdef(a,mcode[b].a);//scf:a;
                if(!strcmp(mcode[b].name,"v=")&&
                   finduse(a,mcode[b].a)==-1)
                    insertdef(a,mcode[b].a);//use:a;
                if(!strcmp(mcode[b].name,"minus")&&
                   finddef(a,mcode[b].a)==-1)
                    insertuse(a,mcode[b].a);//use:a;
            }
    }//init use def
    printf("use_def:\n");
    for(int a=0;a<=btop;a++){
        printf("use:");
        for(int b=0;b<=ltab[a].usetop;b++)
            printf("%s ",ltab[a].use[b]);
        printf("\ndef:");
        for(int b=0;b<=ltab[a].deftop;b++)
            printf("%s ",ltab[a].def[b]);
        printf("\n");
    }
}
void out(int a,bool *modi){
    for(int i=0;i<=bform[a].afttop;i++){
        int temp=bform[a].after[i];
        for(int j=0;j<=ltab[temp].intop;j++)
            if(findout(a,ltab[temp].in[j])==-1){
                strcpy(ltab[a].out[++ltab[a].outtop],ltab[temp].in[j]);
                *modi=1;
            }
    }
}//
void in(int a,bool *modi){
    for(int i=0;i<=ltab[a].outtop;i++)
        if(findin(a,ltab[a].out[i])==-1
           &&finddef(a,ltab[a].out[i])==-1){
            strcpy(ltab[a].in[++ltab[a].intop],ltab[a].out[i]);
            *modi=1;
        }
    for(int i=0;i<=ltab[a].usetop;i++)
        if(findin(a,ltab[a].use[i])==-1){
            strcpy(ltab[a].in[++ltab[a].intop],ltab[a].use[i]);
            *modi=1;
        }
}
/*int findgloreg(char *in){
    for(int a=0;a<=glotop;a++)
        if(!strcmp(in,gloreg[a]))
            return a;
    return -1;
}*/
int findlocreg(char *in){
    for(int a=0;a<=loctop;a++)
        if(!strcmp(in,locreg[a]))
            return a;
    return -1;
}
int mipspoi=-1;//mips instruction pointer!!!!
int blockend(){
    for(int a=0;a<=btop;a++)
        if(bform[a].uplim<=mipspoi&&mipspoi<=bform[a].downlim)
            return a;
    return -1;
}
int blockpre=-1;
int findreg(char *in);
int applyreg(char *in, bool use){
    int end=-1;
    int enda=blockend();
    if(enda==-1){
        printf("end is -1!!!!!!!\n");
        //bool exit1=error(5,w_count,temp);
        return -1;
    }
    else end=bform[enda].downlim;
    int reg=findreg(in);//load to the func at the begining of the func;
    if(reg!=-1) return(GLOREGSTR+reg);
    reg=findlocreg(in);
    if(reg==-1){
        int loc=findtable(in);
        int loc1=maintab.loc;
        if(loc==-1){
            printf("reg as:cannot find in:%s\n",in);
            bool exit1=error(5,w_count,temp);
            if(exit1) return -1;
        }
        int addr=maintab.ele[loc1][loc].addr;
        //if(loc1==0) return -1;//全局区var不分配；
        //分配全局变量
        if(loctop>=LOCREGTOP){
            printf("reach the alter!!!!!\n");
            bool used[32];
            for(int a=0;a<=31;a++)
                used[a]=0;
            for(int a=mipspoi;a<=end;a++){
                if(mcode[a].num>=3&&findlocreg(mcode[a].c)!=-1)
                    used[findlocreg(mcode[a].c)]=1;
                if(mcode[a].num>=2&&findlocreg(mcode[a].b)!=-1)
                    used[findlocreg(mcode[a].b)]=1;
                if(mcode[a].num>=1&&findlocreg(mcode[a].a)!=-1)
                    used[findlocreg(mcode[a].a)]=1;
            }
            int as=-1;
            for(int a=0;a<=LOCREGTOP;a++)
                if(!used[a]){
                    as=a;
                    break;
                }
            if(as==-1) return -1;
            int loc=findtable(locreg[as]);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            int addr=maintab.ele[loc1][loc].addr;
            if(loc1==0){
                //printf("alter-----a: %d name:%s\n",deflocreg[as],locreg[as]);
                if(deflocreg[as]==1)
                    fprintf(mipsout,"sw $%d %d\n",as+LOCREGSTR,STACKLIM-addr);//global;
                }
            else if(findout(enda,locreg[as])!=-1){
                if(deflocreg[as]==1){
                    fprintf(mipsout,"sw $%d -%d($sp)\n",as+LOCREGSTR,addr+8+GLSIZE);//local;
                    }
            }
            deflocreg[as]=0;
            strcpy(locreg[as],in);
            loc=findtable(locreg[as]);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            if(use){
                if(loc1==0) fprintf(mipsout,"lw $%d %d\n",as+LOCREGSTR,STACKLIM-addr);//global;
                else fprintf(mipsout,"lw $%d -%d($sp)\n",as+LOCREGSTR,addr+8+GLSIZE);//local;
                }
            else deflocreg[as]=1;
            //printf("loc_as:%d\b",as);
            return (LOCREGSTR+as);
        }
        else{
            strcpy(locreg[++loctop],in);
            int loc=findtable(in);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            int addr=maintab.ele[loc1][loc].addr;
            if(use){
                if(loc1==0) fprintf(mipsout,"lw $%d %d\n",loctop+LOCREGSTR,STACKLIM-addr);//global;
                else fprintf(mipsout,"lw $%d -%d($sp)\n",loctop+LOCREGSTR,addr+8+GLSIZE);//local;
            }
            else deflocreg[loctop]=1;
            //printf("loc_loc:%d\b",as);
            return (LOCREGSTR+loctop);
        }
    }
    if(!use) deflocreg[reg]=1;
    return(LOCREGSTR+reg);
}
/*void nosharing(){
    for(int a=0;a<=btop;a++){
        for(int b=0;b<=ltab[a].intop;b++)
            if(findgloreg(ltab[a].in[b])==-1){
                if(glotop>=GLOREGTOP) break;
                strcpy(gloreg[++glotop],ltab[a].in[b]);
            }
        if(glotop>=GLOREGTOP) break;
    }
}*/
int nodetop=-1;
void colorize();
int findnode(char *in);
void addnode(char *in);
void init_color();
void elim_redexp(int head,int end,int bnum);
void allocreg(){
    //func_num++;//init: func_num is 0;
    //nodetop[func_num]=-1;
    //init_map();
    init_u_d();//init use def
    bool modi=1;
    for(int a=btop;a>=0;a--){
        ltab[a].intop=-1;
        ltab[a].outtop=-1;
    }
    while(modi){
        modi=0;
        for(int a=btop;a>=0;a--){
            out(a,&modi);
            in(a,&modi);
            //printf("ltab%d:\n",a);
            //printf("intop:%d\n",ltab[a].intop);
            //for(int b=0;b<=ltab[a].intop;b++)
            //    printf("%s ",ltab[a].in[b]);
            //printf("\nouttop:%d\n",ltab[a].outtop);
            //for(int b=0;b<=ltab[a].outtop;b++)
            //    printf("%s ",ltab[a].out[b]);
            //printf("\n");
        }
        //printf("modi:%d\n",modi);
    }//init in out;
    //nosharing();
    for(int a=0;a<=btop;a++){
        int head,end;
        head=bform[a].uplim;
        end=bform[a].downlim;
        //redun_ass(head,end);
        elim_redexp(head,end,a);//delete redundant expression;
    }
    init_u_d();//init use def
    for(int a=btop;a>=0;a--){
        ltab[a].intop=-1;
        ltab[a].outtop=-1;
    }
    modi=1;
    while(modi){
        modi=0;
        for(int a=btop;a>=0;a--){
            out(a,&modi);
            in(a,&modi);
            //printf("ltab%d:\n",a);
            //printf("intop:%d\n",ltab[a].intop);
            //for(int b=0;b<=ltab[a].intop;b++)
            //    printf("%s ",ltab[a].in[b]);
            //printf("\nouttop:%d\n",ltab[a].outtop);
            //for(int b=0;b<=ltab[a].outtop;b++)
            //    printf("%s ",ltab[a].out[b]);
            //printf("\n");
        }
        //printf("modi:%d\n",modi);
    }//init in out;
    init_color();
    for(int a=0;a<=btop;a++){
        if(nodetop>=99) break;
        for(int b=0;b<=ltab[a].intop;b++) {
            if(nodetop>=99) break;
            if(findnode(ltab[a].in[b])==-1)
                addnode(ltab[a].in[b]);
            for (int c=b+1;c<=ltab[a].intop;c++) {
                if (findnode(ltab[a].in[c])==-1)
                    addnode(ltab[a].in[c]);
                int anode=findnode(ltab[a].in[b]);
                int bnode=findnode(ltab[a].in[c]);
                if(anode!=-1&&bnode!=-1){
                    confltmap[anode][bnode]=1;
                    confltmap[bnode][anode]=1;
                }
            }
        }
        for(int b=0;b<=ltab[a].outtop;b++) {
            if(nodetop>=99) break;
            if(findnode(ltab[a].out[b])==-1)
                addnode(ltab[a].out[b]);
            for (int c=b+1;c<=ltab[a].outtop;c++) {
                if (findnode(ltab[a].out[c])==-1)
                    addnode(ltab[a].out[c]);
                int anode=findnode(ltab[a].out[b]);
                int bnode=findnode(ltab[a].out[c]);
                if(anode!=-1&&bnode!=-1){
                    confltmap[anode][bnode]=1;
                    confltmap[bnode][anode]=1;
                }
            }
        }
        for(int b=0;b<=ltab[a].intop;b++) {
            if(nodetop>=99) break;
            if(findnode(ltab[a].in[b])==-1)
                addnode(ltab[a].in[b]);
            for (int c=0;c<=ltab[a].outtop;c++) {
                if (findnode(ltab[a].out[c])==-1)
                    addnode(ltab[a].out[c]);
                int anode=findnode(ltab[a].in[b]);
                int bnode=findnode(ltab[a].out[c]);
                if(strcmp(ltab[a].in[b],ltab[a].out[c])&&anode!=-1&&bnode!=-1){
                    confltmap[anode][bnode]=1;
                    confltmap[bnode][anode]=1;
                }
            }
        }
    }
    colorize();//alloc reg;
}
void elim_redexp(int head, int end,int bnum){
    for(int a=head;a<=end;a++)
        for(int b=a+1;b<=end;b++)
            if(mcode[a].num==3&&
               strcmp(mcode[a].name,"SKIP")&&
               strcmp(mcode[a].name,"[]=")&&
               !isbranch(a)&&!isprt(a)&&
               mcode[a].num==mcode[b].num&&
               !strcmp(mcode[a].name,mcode[b].name)&&
               !strcmp(mcode[a].b,mcode[b].b)&&
               !strcmp(mcode[a].c,mcode[b].c)&&
               findout(bnum,mcode[b].a)==-1&&
               strcmp(mcode[b].a,mcode[b].c)&&
               strcmp(mcode[b].a,mcode[b].b)){
                printf("line %d: m[b].a:%s\n",b,mcode[b].a);
                bool work=1;
                for(int c=a+1;c<=b-1;c++)
                    if(strcmp(mcode[c].name,"SKIP")&&
                       mcode[c].num==3&&
                       (!strcmp(mcode[c].a,mcode[a].a)||
                        !strcmp(mcode[c].a,mcode[a].b)||
                       !strcmp(mcode[c].a,mcode[a].c))){
                        work=0;
                        break;
                    }
                    else if(strcmp(mcode[c].name,"SKIP")&&
                            mcode[c].num==1&&
                            (!strcmp(mcode[c].a,mcode[a].a)||
                             !strcmp(mcode[c].a,mcode[a].b)||
                             !strcmp(mcode[c].a,mcode[a].c))&&
                            (!strcmp(mcode[c].name,"v=")||
                            !strcmp(mcode[c].name,"scf")||
                            !strcmp(mcode[c].name,"minus"))
                            ){
                        work=0;
                        break;
                    }
                if(work){
                    bool skip=1;
                    //strcpy(mcode[b].name,"SKIP");
                    for(int d=b+1;d<=end;d++)
                        if(mcode[d].num==3){
                            if(!strcmp(mcode[d].b,mcode[b].a))
                                strcpy(mcode[d].b,mcode[a].a);
                            if(!strcmp(mcode[d].c,mcode[b].a))
                                strcpy(mcode[d].c,mcode[a].a);
                            if(!strcmp(mcode[d].a,mcode[b].a)){
                                if(!strcmp(mcode[d].name,"prt"))
                                    strcpy(mcode[d].a,mcode[a].a);
                                else break;
                            }
                            if(!strcmp(mcode[d].a,mcode[a].a)){
                                if(strcmp(mcode[d].name,"prt")){
                                    skip=0;
                                    break;
                                }
                            }//optional;
                        }
                        else if(mcode[d].num==2){
                            if(!strcmp(mcode[d].name,"bnez")
                               &&!strcmp(mcode[d].b,mcode[b].a))
                                strcpy(mcode[d].b,mcode[a].a);
                            if(!strcmp(mcode[d].name,"bez")
                               &&!strcmp(mcode[d].b,mcode[b].a))
                                strcpy(mcode[d].b,mcode[a].a);
                        }
                        else if(mcode[d].num==1){
                            if(strcmp(mcode[d].name,"call")){
                                    skip=0;
                                    break;
                                }
                            if(!strcmp(mcode[d].name,"ret")
                               &&!strcmp(mcode[d].a,mcode[b].a))
                                strcpy(mcode[d].a,mcode[a].a);
                            if(!strcmp(mcode[d].name,"push")
                               &&!strcmp(mcode[d].a,mcode[b].a))
                                strcpy(mcode[d].a,mcode[a].a);
                            if(!strcmp(mcode[d].name,"scf")
                               &&!strcmp(mcode[d].a,mcode[b].a))
                                break;
                            if(!strcmp(mcode[d].name,"v=")
                               &&!strcmp(mcode[d].a,mcode[b].a))
                                break;
                            if(!strcmp(mcode[d].name,"scf")
                               &&!strcmp(mcode[d].a,mcode[a].a)){
                                skip=0;
                                break;
                            }//optional;
                            if(!strcmp(mcode[d].name,"v=")
                               &&!strcmp(mcode[d].a,mcode[a].a)){
                                skip=0;
                                break;
                            }//optional;
                        }
                    if(skip) strcpy(mcode[b].name,"SKIP");//optional;
                }
            }
}
void eraseredun(){
    for(int a=1;a<=btop;a++)
        if(bform[a].pretop==-1){
            printf("a:%d\n",a);
            for(int b=bform[a].uplim;b<=bform[a].downlim;b++){
                if(strcmp(mcode[b].name,"SKIP"))
                    printf("optim:%s %s %s %s\n",mcode[b].name,mcode[b].a,mcode[b].b,mcode[b].c);
                strcpy(mcode[b].name,"SKIP");
            }
        }
}
int anafunc(int m_top){
    btop++;//b_top=0;
    if(islstru(m_top)){
        bform[btop].uplim=m_top;
        bform[btop].downlim=m_top;
        m_top++;
    }
    else printf("not lab\n");
    while(strcmp(mcode[m_top].name,"swtpoi")){
        if(islstru(m_top)){
            bform[btop].downlim = m_top-1;
            btop++;//next block
            bform[btop].uplim = m_top;
            bform[btop].downlim = m_top;
        }
        else if(isjstru(m_top)||isbranch(m_top)){
            bform[btop].downlim = m_top;
            btop++;//next block
            bform[btop].uplim = m_top+1;
            bform[btop].downlim = m_top+1;
            if(islstru(m_top+1)) m_top++;
        }
        m_top++;//next block;
    }
    btop--;
    printf("btop:%d\n",btop);
    //analyse the block
    //
    for(int a=0;a<=btop;a++){
        bform[a].pretop=-1;
        bform[a].afttop=-1;//init;
        if(a==0){
            bform[a].pretop=0;
            bform[a].pre[0]=0;//init the start of func;
        }
        if(a!=0&&bform[a-1].pretop!=-1&&!isjstru(bform[a-1].downlim)){
            bform[a].pretop++;
            bform[a].pre[bform[a].pretop]=a-1;
        }
        if(islstru(bform[a].uplim))
            for(int b=0;b<=btop;b++)
                    if((isjstru(bform[b].downlim)||isbranch(bform[b].downlim))
                       &&bform[b].downlim!=bform[a].uplim
                       &&!strcmp(loc(bform[a].uplim),loc(bform[b].downlim))){//see if lab(a) matches
                        bform[a].pretop++;
                        bform[a].pre[bform[a].pretop] = b;
                    }
        if(islstru(bform[a].downlim)){
            bform[a].afttop++;
            bform[a].after[bform[a].afttop]=a+1;
        }
        else if(isjstru(bform[a].downlim)){
            for(int b=0;b<=btop;b++)
                if(islstru(bform[b].uplim)&&!strcmp(loc(bform[a].downlim),loc(bform[b].uplim))) {
                    bform[a].afttop++;
                    bform[a].after[bform[a].afttop] = b;
                }
        }
        else if(isbranch(bform[a].downlim)){
            bform[a].afttop++;
            bform[a].after[bform[a].afttop]=a+1;
            for(int b=0;b<=btop;b++)
                if(islstru(bform[b].uplim)&&!strcmp(loc(bform[a].downlim),loc(bform[b].uplim))) {
                    bform[a].afttop++;
                    bform[a].after[bform[a].afttop] = b;
                }
        }
        else{
            bform[a].afttop++;
            bform[a].after[bform[a].afttop]=a+1;
        }//if not j\b\l instru
    }
    for(int a=0;a<=btop;a++){
        printf("%d uplim:%d downlim:%d pretop:%d\n",a,bform[a].uplim,bform[a].downlim,bform[a].pretop);
        for(int b=0;b<=bform[a].pretop;b++)
            printf("%d ",bform[a].pre[b]);
        printf("\nafttop:%d\n",bform[a].afttop);
        for(int b=0;b<=bform[a].afttop;b++)
            printf("%d ",bform[a].after[b]);
        //int head,end;
        //head=bform[a].uplim;
        //end=bform[a].downlim;
        //elim_redexp(head,end,a);//delete redundant expression;
        printf("\n");
    }
    /*printf("nodetab%d:\n",func_num);
    for(int a=0;a<=nodetop[func_num];a++){
        printf("varname:%s, reg:%d\n",node[func_num][a].name,node[func_num][a].reg);
    }
    printf("\n");*/
    return m_top+1;//over swtpoi 0
}
void onemips(int a);
void optmips(int down,int up);
void redun_ass(int down,int up);
int enterfunc(int m_top){
    while(strcmp(mcode[m_top].name,"swtpoi")&&m_top<=midtop){
        onemips(m_top);
        m_top++;
    }
    printf("M:%d\n",m_top+1);
    if(m_top>midtop) return midtop;
    int temp=anafunc(m_top+1);//over 'swtpoi 0' or mainend:
    int down=m_top;//swtpoi
    int up=temp-1;//swtpoi
    eraseredun();//delete redundant code;
    printf("in_out-------------------\n");
    for(int a=0;a<=btop;a++){
        int head,end;
        head=bform[a].uplim;
        end=bform[a].downlim;
        redun_ass(head,end);
        //elim_redexp(head,end,a);//delete redundant expression;
    }
    allocreg();//allocate registers;
    for(int a=0;a<=btop;a++){
        printf("block%d------\nin:",a);
        for(int b=0;b<=ltab[a].intop;b++)
            printf("%s ",ltab[a].in[b]);
        printf("\nout:");
        for(int b=0;b<=ltab[a].outtop;b++)
            printf("%s ",ltab[a].out[b]);
        printf("\n");
    }
    printf("-------------------node:\n");
    for(int a=0;a<=nodetop;a++){
        printf("name:%s num:%d reg:%d\n",node[a].name,node[a].num,node[a].gloreg+GLOREGSTR);
    }
    //onemips(m_top);
    optmips(down,up);
    printf("M:%d\n",temp);
    printf("down:%d,up:%d\n",down,up);
    return temp;
}
void onemips(int a){
    if(!strcmp(mcode[a].name,"SKIP")) return;//skip when reach this string;
    mipspoi=a;//!!!
    if(mcode[a].num==0)   midtomips0(mcode[a].name);
    else if(mcode[a].num==1)   midtomips1(mcode[a].name,mcode[a].a);
    else if(mcode[a].num==2)   midtomips2(mcode[a].name,mcode[a].a,mcode[a].b);
    else if(mcode[a].num==3)   midtomips3(mcode[a].name,mcode[a].a,mcode[a].b,mcode[a].c);
    //fprintf(mipsout,"addi $fp $fp 1\n");
    fprintf(mipsout,"\n");
}
void optmips(int down,int up){
    onemips(down);
    onemips(down+1);
    int regin[32];
    memset(regin,0,32*sizeof(int));
    for(int a=0;a<=nodetop;a++)
        if(!regin[node[a].gloreg]&&findin(0,node[a].name)!=-1){
            regin[node[a].gloreg]=1;
            int loc=findtable(node[a].name);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                //printf("name: %s pointer: %d\n",a,maintab.pointer);
                if(exit1) return;
            }
            int addr=maintab.ele[loc1][loc].addr;
            if(loc1==0){
                printf("loc1==0!!!!\n");
                error(-1,w_count,temp);
                return;
                //fprintf(mipsout,"lw $%d %d\n",a+GLOREGSTR,STACKLIM-addr);
            }
            else fprintf(mipsout,"lw $%d -%d($sp)\n",node[a].gloreg+GLOREGSTR,addr+8+GLSIZE);//local;
        }
    int end=-1;
    int enda=-1;
    for(int a=down+2;a<=up;a++){
        mipspoi=a;
        enda=blockend();
        if(enda==-1){
            printf("a:%d\n",a);
            printf("end is -1!!!!!!!\n");
            //bool exit1=error(5,w_count,temp);
            return;
        }
        else end=bform[enda].downlim;
        //printf("midcode: %s %s %s %s\n",mcode[a].name,mcode[a].a,mcode[a].b,mcode[a].c);
        bool work=0;
        if(a==end) work=1;//reach the block end;
        if(work&&(isbranch(a)||isjstru(a))){
            for(int a=0;a<=loctop;a++){
                int loc=findtable(locreg[a]);
                int loc1=maintab.loc;
                if(loc==-1){
                    bool exit1=error(5,w_count,temp);
                    if(exit1) return;
                }
                int addr=maintab.ele[loc1][loc].addr;
                printf("-----a: %d name:%s\n",deflocreg[a],locreg[a]);
                if(loc1==0){
                    if(deflocreg[a]==1)
                        fprintf(mipsout,"sw $%d %d\n",a+LOCREGSTR,STACKLIM-addr);//global;
                    }
                else if(findout(enda,locreg[a])!=-1){
                    if(deflocreg[a]==1)
                        fprintf(mipsout,"sw $%d -%d($sp)\n",a+LOCREGSTR,addr+8+GLSIZE);//local;
                    }
            }
            printf("\n");
            memset(deflocreg,0,100*sizeof(int));//clean!!!!;
        }//clean the reg pool;
        if(strcmp(mcode[a].name,"SKIP"))//skip when reach this string;
            if(mcode[a].num==0)   midtomips0(mcode[a].name);
            else if(mcode[a].num==1)   midtomips1(mcode[a].name,mcode[a].a);
            else if(mcode[a].num==2)   midtomips2(mcode[a].name,mcode[a].a,mcode[a].b);
            else if(mcode[a].num==3)   midtomips3(mcode[a].name,mcode[a].a,mcode[a].b,mcode[a].c);
        if(work&&!(isbranch(a)||isjstru(a))){
            for(int a=0;a<=loctop;a++){
                int loc=findtable(locreg[a]);
                int loc1=maintab.loc;
                if(loc==-1){
                    bool exit1=error(5,w_count,temp);
                    if(exit1) return;
                }
                int addr=maintab.ele[loc1][loc].addr;
                printf("-----a: %d name:%s\n",deflocreg[a],locreg[a]);
                if(loc1==0){
                    if(deflocreg[a]==1)
                        fprintf(mipsout,"sw $%d %d\n",a+LOCREGSTR,STACKLIM-addr);//global;
                    }
                else if(findout(enda,locreg[a])!=-1){
                    if(deflocreg[a]==1)
                        fprintf(mipsout,"sw $%d -%d($sp)\n",a+LOCREGSTR,addr+8+GLSIZE);//local;
                    }
            }
            printf("\n");
            memset(deflocreg,0,100*sizeof(int));//clean!!!!;
        }//clean the reg pool;
        fprintf(mipsout,"\n");
        if(work){
            /*for(int a=0;a<=loctop;a++){
                printf("defloc:%d %s\n",deflocreg[a],locreg[a]);
            }
            printf("\n");*/
            loctop=-1;//clean!!!!;
        }
    }
}
void anaprogram(){
    printstr();//string
    int m_top=0;
    while(m_top<midtop){
        btop=-1;
        loctop=-1;//local reg;
        nodetop=-1;//global reg;
        glotop=-1;
        m_top=enterfunc(m_top);//enter the function;
        //printf("loctop:%d\n",loctop);
    }
    onemips(midtop);//mainend:!!
}

void addnode(char *in){
    if(nodetop>=99) return;
    int loc=findtable(in);
    int loc1=maintab.loc;
    int addr=maintab.ele[loc1][loc].addr;
    if(loc!=-1&&loc1==0){
        return;
    }
    nodetop++;
    node[nodetop].num=nodetop;
    node[nodetop].gloreg=-1;//clear!!!!
    strcpy(node[nodetop].name,in);
}
int findnode(char *in){
    for(int a=0;a<=nodetop;a++)
        if(!strcmp(node[a].name,in))
            return node[a].num;
    return -1;
}
void init_color(){
    for(int a=0;a<100;a++)
        for(int b=0;b<100;b++)
            confltmap[a][b]=0;
}
int findreg(char *in){
    int temp=findnode(in);
    if(temp==-1)
        return -1;
    return node[temp].gloreg;
}//called by genmid.h;
void colorize() {
    int stack[100];
    int erase[100];
    memset(erase, 0, 100*sizeof(int));
    int enter[100];
    memset(enter, 0, 100*sizeof(int));
    int stacktop = -1;
    bool work = 1;
    for(int a=0;a<=nodetop;a++){
        for(int b=0;b<=nodetop;b++){
            printf("%d ",confltmap[a][b]);
        }
        printf("\n");
    }
    while (work) {
        work = 0;
        for (int a = 0; a <= nodetop; a++)
            if (!enter[a] && !erase[a]) {
                work = 1;
                int dig = 0;
                for (int b = 0; b <= nodetop; b++){
                    printf("-------enter[b]:%d erase[b]:%d a:%d b:%d confltmap:%d\n",enter[b],erase[b],a,confltmap[a][b]);
                    if (!enter[b]&&!erase[b] && b != a && confltmap[a][b]==1)
                        dig++;
                }
                if (dig < GLOREGTOP+1) {
                    stack[++stacktop] = a;
                    enter[a] = 1;
                }
            }
        for (int a = 0; a <= nodetop; a++)
            if (!enter[a] && !erase[a]) {
                work = 1;
                int dig = 0;
                for (int b = 0; b <= nodetop; b++)
                    if (!enter[b]&&!erase[b] && b != a && confltmap[a][b]==1)
                        dig++;
                if (dig >= GLOREGTOP+1) {
                    erase[a] = 1;
                    break;
                }
            }
    }
    //alloc reg;
    int alloc_reg;
    int used[100];
    printf("----------------stack:%d\n",stacktop);
    for (int a = stacktop; a >= 0; a--) {
        alloc_reg = -1;
        memset(used, 0, 100*sizeof(int));
        for (int b = a + 1; b <= stacktop; b++)
            if (confltmap[stack[a]][stack[b]])
                used[node[stack[b]].gloreg] = 1;
        for (int b = 0; b <= GLOREGTOP; b++)
            if (!used[b]) {
                alloc_reg = b;
                break;
            }
        node[stack[a]].gloreg = alloc_reg;
    }
    int maxtop=-1;
    for(int a=0;a<=nodetop;a++){
        if(maxtop==-1||maxtop<node[a].gloreg) maxtop=node[a].gloreg;
    }
    glotop=maxtop;
}
void redun_ass(int start,int end){//optional;
    int pre=-1;
    for(int a=start;a<=end;a++)
        if(pre!=-1&&!strcmp(mcode[a].name,"=")&&!strcmp(mcode[a].c,"0")){
            if(mcode[pre].a[0]=='&'&&!strcmp(mcode[a].b,mcode[pre].a)){
                for(int b=a+1;b<=end;b++) {
                    if (!strcmp(mcode[b].name, "=")){
                        if(!strcmp(mcode[b].a,mcode[pre].a))
                            break;//reassign then it cannot be changed!!!
                        if(!strcmp(mcode[b].b,mcode[pre].a))
                            strcpy(mcode[b].b,mcode[a].a);
                    }
                    else break;
                }
                strcpy(mcode[a].b,mcode[pre].b);
                if(!strcmp(mcode[pre].c,"1"))
                    strcpy(mcode[a].c,"1");
                strcpy(mcode[pre].name,"SKIP");
            }
            pre=a;
        }
        else if(!strcmp(mcode[a].name,"="))
            pre=a;
        else pre=-1;
}
/*for(int a=0;a<=btop;a++){
        printf("ltab%d:\n",a);
        printf("intop:%d\n",ltab[a].intop);
        for(int b=0;b<=ltab[a].intop;b++)
            printf("%s ",ltab[a].in);
        printf("\nouttop:%d\n",ltab[a].intop);
        for(int b=0;b<=ltab[a].outtop;b++)
            printf("%s ",ltab[a].out);
        printf("\n");
}*/
#endif //COMPILER_OPTIM_H