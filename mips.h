//
// Created by noble6emc2 on 2017/12/17.
//

#ifndef COMPILER_MIPS_H
#define COMPILER_MIPS_H

#include "optim.h"
int pushtop=0;
//int baseaddr[100];
//int blevel=-1;
bool midtomips3(char *name, char *a, char *b, char *c){
    if(!strcmp(name,"=")&&!strcmp(c,"1")) {
        int reg = applyreg(a,0);
        if (reg == -1) {
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
            else fprintf(mipsout,"sw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else{
            int num=atoi(b);
            fprintf(mipsout,"li $%d %d\n",reg,num);
        }
    }
    else if(!strcmp(name,"=")&&!strcmp(c,"0")){
        int regb=applyreg(b,1);//before the assigment!!!
        char tarb[100];
        sprintf(tarb,"$t0");
        int loc,loc1,addr;
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                printf("name: %s pointer: %d\n",a,maintab.pointer);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        int rega=applyreg(a,0);
        if(rega==-1){
            loc=findtable(a);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                printf("name: %s pointer: %d\n",b,maintab.pointer);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"sw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"sw %s %d\n",tarb,STACKLIM-addr);//global;
            else fprintf(mipsout,"sw %s -%d($sp)\n",tarb,addr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"ori $%d %s 0\n",rega,tarb);
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
            fprintf(mipsout,"li $t0 -%d\n",addr+8+GLSIZE);
            fprintf(mipsout,"add $t0 $t0 $sp\n");
        }//local;
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"ori $t1 $%d 0\n",regc);
        //fprintf(mipsout,"add $t1 $t1 $t1\n");
        //fprintf(mipsout,"add $t1 $t1 $t1\n");//4*t1;
        fprintf(mipsout,"sll $t1 $t1 2\n");
        fprintf(mipsout,"sub $t0 $t0 $t1\n");//array addr
        fprintf(mipsout,"lw $t1 0($t0)\n");//res;
        int rega=applyreg(a,0);
        if(rega==-1){
            loc=findtable(a);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"sw $t1 %d\n",addr);//res;
            if(loc1==0) fprintf(mipsout,"sw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"sw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"ori $%d $t1 0\n",rega);
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
            fprintf(mipsout,"li $t0 -%d\n",addr+8+GLSIZE);
            fprintf(mipsout,"add $t0 $t0 $sp\n");
        }//local;
        int regb=applyreg(b,1);
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"ori $t1 $%d 0\n",regb);
        //fprintf(mipsout,"add $t1 $t1 $t1\n");
        //fprintf(mipsout,"add $t1 $t1 $t1\n");//4*t1;
        fprintf(mipsout,"sll $t1 $t1 2\n");
        fprintf(mipsout,"sub $t0 $t0 $t1\n");//array addr
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"sw $t1 %d\n",addr);//res;
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
            fprintf(mipsout,"sw $t1 0($t0)\n");//res;
        }
        else fprintf(mipsout,"sw $%d 0($t0)\n",regc);
    }
    else if(!strcmp(name,"/")){
        int rega;
        int loc,loc1,addr,taraddr,tarloc;
        char tarb[100];
        sprintf(tarb,"$t1");
        int regb=applyreg(b,1);
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t2");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t2 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t2 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t2 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        fprintf(mipsout,"div %s %s\n",tarb,tarc);
        rega=applyreg(a,0);
        if(rega==-1){
            loc=findtable(a);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            taraddr=addr;//memo
            tarloc=loc1;//
        }
        if(rega==-1){
            fprintf(mipsout,"mflo $t0\n");
            if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
            else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"mflo $%d\n",rega);
    }
    else if(!strcmp(name,"/i")){
        int rega;
        int loc,loc1,addr,taraddr,tarloc;
        char tarb[100];
        sprintf(tarb,"$t1");
        int regb=applyreg(b,1);
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        //char tarc[100];
        //sprintf(tarc,"%d",c);
        rega=applyreg(a,0);
        if(rega==-1){
            loc=findtable(a);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            taraddr=addr;//memo
            tarloc=loc1;//
        }
        if(rega==-1){
            //fprintf(mipsout,"mflo $t0\n");
            fprintf(mipsout,"div $t0 %s %s\n",tarb,c);
            if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
            else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"div $%d %s %s\n",rega,tarb,c);
    }
    else if(!strcmp(name,"*")){
        int rega;
        int loc,loc1,addr,taraddr,tarloc;
        char tarb[100];
        sprintf(tarb,"$t1");
        int regb=applyreg(b,1);
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t2");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t2 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t2 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t2 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        fprintf(mipsout,"mult %s %s\n",tarb,tarc);
        rega=applyreg(a,0);
        if(rega==-1){
            loc=findtable(a);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            taraddr=addr;//memo
            tarloc=loc1;//
        }
        if(rega==-1){
            fprintf(mipsout,"mflo $t0\n");
            if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
            else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"mflo $%d\n",rega);
    }
    else if(!strcmp(name,"*i")){
        int rega;
        int loc,loc1,addr,taraddr,tarloc;
        char tarb[100];
        sprintf(tarb,"$t1");
        int regb=applyreg(b,1);
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        //char tarc[100];
        //sprintf(tarc,"%d",c);
        rega=applyreg(a,0);
        if(rega==-1){
            loc=findtable(a);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            taraddr=addr;//memo
            tarloc=loc1;//
        }
        if(rega==-1){
            //fprintf(mipsout,"mflo $t0\n");
            fprintf(mipsout,"mul $t0 %s %s\n",tarb,c);
            if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
            else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"mul $%d %s %s\n",rega,tarb,c);
    }
    else if(!strcmp(name,"-")){
        int rega;
        int loc,loc1,addr,taraddr,tarloc;
        char tarb[100];
        sprintf(tarb,"$t1");
        int regb=applyreg(b,1);
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t2");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t2 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t2 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t2 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        rega=applyreg(a,0);
        if(rega==-1){
            loc=findtable(a);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            taraddr=addr;//memo
            tarloc=loc1;//
        }
        if(rega==-1){
            fprintf(mipsout,"sub $t0 %s %s\n",tarb,tarc);
            if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
            else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"sub $%d %s %s\n",rega,tarb,tarc);
    }
    else if(!strcmp(name,"+")){
        int rega;
        int loc,loc1,addr,taraddr,tarloc;
        char tarb[100];
        sprintf(tarb,"$t1");
        int regb=applyreg(b,1);
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t2");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t2 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t2 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t2 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        rega=applyreg(a,0);
        if(rega==-1){
            loc=findtable(a);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            taraddr=addr;//memo
            tarloc=loc1;//
        }
        if(rega==-1){
            fprintf(mipsout,"add $t0 %s %s\n",tarb,tarc);
            if(tarloc==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-taraddr);//global;
            else fprintf(mipsout,"sw $t0 -%d($sp)\n",taraddr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"add $%d %s %s\n",rega,tarb,tarc);
    }
    else if(!strcmp(name,"bne")){
        int regb=applyreg(b,1);
        int loc,loc1,addr;
        char tarb[100];
        sprintf(tarb,"$t0");
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t1");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        fprintf(mipsout,"bne %s %s %s\n",tarb,tarc,a);
    }
    else if(!strcmp(name,"beq")){
        int regb=applyreg(b,1);
        int loc,loc1,addr;
        char tarb[100];
        sprintf(tarb,"$t0");
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t1");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        fprintf(mipsout,"beq %s %s %s\n",tarb,tarc,a);
    }
    else if(!strcmp(name,"bgr")){
        char tarb[100];
        sprintf(tarb,"$t0");
        int regb=applyreg(b,1);
        int loc,loc1,addr;
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t1");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        fprintf(mipsout,"sub $t0 %s %s\n",tarb,tarc);
        fprintf(mipsout,"bgtz $t0 %s\n",a);
    }
    else if(!strcmp(name,"bge")){
        char tarb[100];
        sprintf(tarb,"$t0");
        int regb=applyreg(b,1);
        int loc,loc1,addr;
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t1");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        fprintf(mipsout,"sub $t0 %s %s\n",tarb,tarc);
        fprintf(mipsout,"bgez $t0 %s\n",a);
    }
    else if(!strcmp(name,"bls")){
        char tarb[100];
        sprintf(tarb,"$t0");
        int regb=applyreg(b,1);
        int loc,loc1,addr;
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t1");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        fprintf(mipsout,"sub $t0 %s %s\n",tarb,tarc);
        fprintf(mipsout,"bltz $t0 %s\n",a);
    }
    else if(!strcmp(name,"ble")){
        char tarb[100];
        sprintf(tarb,"$t0");
        int regb=applyreg(b,1);
        int loc,loc1,addr;
        if(regb==-1){
            loc=findtable(b);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        char tarc[100];
        sprintf(tarc,"$t1");
        int regc=applyreg(c,1);
        if(regc==-1){
            loc=findtable(c);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t1 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t1 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t1 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarc,"$%d",regc);
        fprintf(mipsout,"sub $t0 %s %s\n",tarb,tarc);
        fprintf(mipsout,"blez $t0 %s\n",a);
    }
    else if(!strcmp(name,"prt")&&!strcmp(b,"0")){
        int rega=applyreg(a,1);
        int loc,loc1,addr;
        if(rega==-1){
            loc=findtable(a);
            loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            addr=maintab.ele[loc1][loc].addr;
        }
        int kind=atoi(c);
        if(!(kind==INT||kind==CHAR||kind==CONSTCHR||kind==CONSTINT)){
            printf("type int/char error\n");
            bool exit1=error(-1,w_count,temp);
            if(exit1) return 0;
        }
        //printf("prtkind %s %d %d %d\n",a,kind,loc1,loc);
        //fprintf(mipsout,"li $a0 %d\n",STRINGLIM);
        if(kind==INT||kind==CONSTINT) fprintf(mipsout,"li $v0 %d\n",1);
        else fprintf(mipsout,"li $v0 %d\n",11);
        //fprintf(mipsout,"lw $a0 %d\n",addr);
        if(rega==-1){
            if(loc1==0) fprintf(mipsout,"lw $a0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $a0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"ori $a0 $%d 0\n",rega);
        fprintf(mipsout,"syscall\n");
    }
    return 1;
}
bool midtomips2(char *name, char *a, char *b){
    if(!strcmp(name,"bnez")){
        char tarb[100];
        sprintf(tarb,"$t0");
        int regb=applyreg(b,1);
        if(regb==-1){
            int loc=findtable(b);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            int addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        fprintf(mipsout,"bne %s $0 %s\n",tarb,a);
    }
    else if(!strcmp(name,"bez")){
        char tarb[100];
        sprintf(tarb,"$t0");
        int regb=applyreg(b,1);
        if(regb==-1){
            int loc=findtable(b);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            int addr=maintab.ele[loc1][loc].addr;
            //fprintf(mipsout,"lw $t0 %d\n",addr);
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tarb,"$%d",regb);
        fprintf(mipsout,"beq %s $0 %s\n",tarb,a);
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
        int end=-1;
        int enda=blockend();
        if(enda==-1){
            //printf("end is -1!!!!!!!\n");
            //bool exit1=error(5,w_count,temp);
            //return 0;
        }
        else end=bform[enda].downlim;
        bool used[32];
        for(int a=0;a<=31;a++)
            used[a]=0;
        if(end!=-1)
            for(int a=mipspoi;a<=end;a++){
                if(mcode[a].num>=3&&findlocreg(mcode[a].c)!=-1)
                    used[findlocreg(mcode[a].c)]=1;
                if(mcode[a].num>=2&&findlocreg(mcode[a].b)!=-1)
                    used[findlocreg(mcode[a].b)]=1;
                if(mcode[a].num>=1&&findlocreg(mcode[a].a)!=-1)
                    used[findlocreg(mcode[a].a)]=1;
            }
        if(maintab.pointer==0){
            printf("maintab.memnum:%d\n",maintab.memnum);
            fprintf(mipsout,"sw $sp -%d($sp)\n",maintab.memnum+4+GLSIZE);// return stack num;
            fprintf(mipsout,"subi $sp $sp %d\n",maintab.memnum);
        }
        else{
            if(maintab.funsize[maintab.pointer]==-1)
                maintab.funsize[maintab.pointer]=0;

            for(int a=LOCREGSTR;a<=LOCREGSTR+loctop;a++){//local reg;
                int loc=findtable(locreg[a-LOCREGSTR]);
                int loc1=maintab.loc;
                int addr=maintab.ele[loc1][loc].addr;
                if(loc!=-1&&loc1==0){
                    if(deflocreg[a-LOCREGSTR]==1)
                        fprintf(mipsout,"sw $%d %d\n",a,STACKLIM-addr);
                }
                else if(used[a-LOCREGSTR]||findout(enda,locreg[a-LOCREGSTR])!=-1){
                        fprintf(mipsout,"sw $%d -%d($sp)\n",a,4*(a-LOCREGSTR)+8+maintab.funsize[maintab.pointer]+GLSIZE);
                }
            }
            for(int a=GLOREGSTR;a<=GLOREGSTR+glotop;a++){//global reg;
                fprintf(mipsout,"sw $%d -%d($sp)\n",a,4*(a-LOCREGSTR)+8+maintab.funsize[maintab.pointer]+GLSIZE);
            }
            fprintf(mipsout,"sw $sp -%d($sp)\n",4+GLSIZE+8+maintab.funsize[maintab.pointer]+GLSIZE);
            fprintf(mipsout,"subi $sp $sp %d\n",8+GLSIZE+maintab.funsize[maintab.pointer]);
        }
        //printf("pushtop-paranum:%d\n",pushtop-paranum);
        /*for(int a=pushtop-paranum;a<pushtop;a++){
            fprintf(mipsout,"lw $a0 %d\n",4*a+ARGLIM);
            fprintf(mipsout,"sw $a0 -%d($sp)\n",4*(a-pushtop+paranum)+8+GLSIZE);//reserv 1 byte;
        }*/
        for(int a=paranum-1;a>=0;a--){//
            fprintf(mipsout,"subi $gp $gp 4\n");
            fprintf(mipsout,"lw $a0 0($gp)\n");
            fprintf(mipsout,"sw $a0 -%d($sp)\n",4*a+8+GLSIZE);//reserv 1 byte;
        }//
        //reserv the return address and data;
        //pushtop-=paranum;
        fprintf(mipsout,"jal %s\n",a);//ra=namelab;
        /*for(int a=0;a<=glotop;a++){
            int loc=findtable(gloreg[a]);
            int loc1=maintab.loc;
            int addr=maintab.ele[loc1][loc].addr;
            if(loc!=-1&&loc1==0){
                fprintf(mipsout,"lw $%d %d\n",a+GLOREGSTR,STACKLIM-addr);
            }
        }*/
        for(int a=GLOREGSTR;a<=GLOREGSTR+glotop;a++){//global reg;
            fprintf(mipsout,"lw $%d -%d($sp)\n",a,4*(a-LOCREGSTR)+8+maintab.funsize[maintab.pointer]+GLSIZE);
        }
        for(int a=LOCREGSTR;a<=LOCREGSTR+loctop;a++){//local reg;
            if(used[a-LOCREGSTR]||findout(enda,locreg[a-LOCREGSTR])!=-1){
                int loc=findtable(locreg[a-LOCREGSTR]);
                int loc1=maintab.loc;
                int addr=maintab.ele[loc1][loc].addr;
                if(loc!=-1&&loc1==0){
                    //if(deflocreg[a-LOCREGSTR]==1)
                        fprintf(mipsout,"lw $%d %d\n",a,STACKLIM-addr);
                }
                else fprintf(mipsout,"lw $%d -%d($sp)\n",a,4*(a-LOCREGSTR)+8+maintab.funsize[maintab.pointer]+GLSIZE);
            }
        }
    }
    else if(!strcmp(name,"ret")){
        int rega=applyreg(a,1);
        if(rega==-1){
            int loc=findtable(a);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            int addr=maintab.ele[loc1][loc].addr;//global;
            if(loc1==0) fprintf(mipsout,"lw $v0 %d\n",STACKLIM-addr);
            else fprintf(mipsout,"lw $v0 -%d($sp)\n",addr+8+GLSIZE);
        }
        else fprintf(mipsout,"ori $v0 $%d 0\n",rega);
        /*for(int a=LOCREGSTR;a<=LOCREGSTR+LOCREGTOP;a++)//local reg;
            fprintf(mipsout,"lw $%d -%d($sp)\n",a,4*(a-LOCREGSTR));
        for(int a=GLOREGSTR;a<=GLOREGSTR+GLOREGTOP;a++){
            //global reg;
            int loc=findtable(gloreg[a-GLOREGSTR]);
            int loc1=maintab.loc;
            int addr=maintab.ele[loc1][loc].addr;
            if(loc!=-1&&a<=GLOREGSTR+glotop){
                //continue;
                if(loc1==0){
                    printf("loc1==0!!!!save to memory\n");
                    //error(-1,w_count,temp);
                    //return;
                    fprintf(mipsout,"sw $%d %d\n",a,STACKLIM-addr);
                }
            }
            fprintf(mipsout,"lw $%d -%d($sp)\n",a,4*(a-LOCREGSTR));
        }*/
        for(int a=LOCREGSTR;a<=LOCREGSTR+loctop;a++){
            //global reg;
            int loc=findtable(locreg[a-LOCREGSTR]);
            int loc1=maintab.loc;
            int addr=maintab.ele[loc1][loc].addr;
            if(loc!=-1&&loc1==0){
                printf("loc1==0!!!!save to memory\n");
                //error(-1,w_count,temp);
                //return;
                if(deflocreg[a-LOCREGSTR]==1)
                    fprintf(mipsout,"sw $%d %d\n",a,STACKLIM-addr);
            }
        }
        fprintf(mipsout,"lw $ra -%d($sp)\n",GLSIZE);//ra=namelab;
        fprintf(mipsout,"lw $sp -%d($sp)\n",GLSIZE+4);//ra=namelab;
        fprintf(mipsout,"jr $ra\n");
    }
    else if(!strcmp(name,"lab")){
        fprintf(mipsout,"%s:\n",a);
    }
    else if(!strcmp(name,"jmp")){
        fprintf(mipsout,"j %s\n",a);
    }
    else if(!strcmp(name,"push")){
        char tara[100];
        sprintf(tara,"$t0");
        int rega=applyreg(a,1);
        if(rega==-1){
            int loc=findtable(a);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            int addr=maintab.ele[loc1][loc].addr;
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else sprintf(tara,"$%d",rega);
        //fprintf(mipsout,"sw %s %d\n",tara,ARGLIM+4*pushtop);
        fprintf(mipsout,"sw %s 0($gp)\n",tara);//
        fprintf(mipsout,"addi $gp $gp 4\n");//
        //pushtop+=1;
    }
    else if(!strcmp(name,"scf")){
        int rega=applyreg(a,0);
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
            if(rega==-1){
                if(loc1==0) fprintf(mipsout,"sw $v0 %d\n",STACKLIM-addr);//global;
                else fprintf(mipsout,"sw $v0 -%d($sp)\n",addr+8+GLSIZE);//local;
            }
            else fprintf(mipsout,"ori $%d $v0 0\n",rega);
        }
        else if(kind==CHAR){
            fprintf(mipsout,"li $v0 12\n");
            fprintf(mipsout,"syscall\n");
            //fprintf(mipsout,"sw $t0 %d\n",addr);
            if(rega==-1){
                if(loc1==0) fprintf(mipsout,"sw $v0 %d\n",STACKLIM-addr);//global;
                else fprintf(mipsout,"sw $v0 -%d($sp)\n",addr+8+GLSIZE);//local;
            }
            else fprintf(mipsout,"ori $%d $v0 0\n",rega);
        }
        else{
            bool exit1=error(9,w_count,temp);
            if(exit1) return 0;
            //return 1;
        }
    }
    else if(!strcmp(name,"v=")){
        int rega=applyreg(a,0);
        if(rega==-1){
            int loc=findtable(a);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            int addr=maintab.ele[loc1][loc].addr;
            if(loc1==0) fprintf(mipsout,"sw $v0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"sw $v0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"ori $%d $v0 0\n",rega);
    }
    else if(!strcmp(name,"minus")){
        int rega=applyreg(a,1);//minus itself!!!!
        if(rega==-1){
            int loc=findtable(a);
            int loc1=maintab.loc;
            if(loc==-1){
                bool exit1=error(5,w_count,temp);
                if(exit1) return 0;
            }
            int addr=maintab.ele[loc1][loc].addr;
            if(loc1==0) fprintf(mipsout,"lw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"lw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
            fprintf(mipsout,"sub $t0 $0 $t0\n");
            if(loc1==0) fprintf(mipsout,"sw $t0 %d\n",STACKLIM-addr);//global;
            else fprintf(mipsout,"sw $t0 -%d($sp)\n",addr+8+GLSIZE);//local;
        }
        else fprintf(mipsout,"sub $%d $0 $%d\n",rega,rega);
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
        fprintf(mipsout,"sw $ra -%d($sp)\n",GLSIZE);
    }
    else if(!strcmp(name,"ret0")){
        fprintf(mipsout,"li $v0 0\n");
        /*for(int a=LOCREGSTR;a<=LOCREGSTR+LOCREGTOP;a++)//local reg;
            fprintf(mipsout,"lw $%d -%d($sp)\n",a,4*(a-LOCREGSTR));
        for(int a=GLOREGSTR;a<=GLOREGSTR+GLOREGTOP;a++){
            int loc=findtable(gloreg[a-GLOREGSTR]);
            int loc1=maintab.loc;
            int addr=maintab.ele[loc1][loc].addr;
            if(loc!=-1&&a<=GLOREGSTR+glotop){
                //continue;
                if(loc1==0){
                    printf("loc1==0!!!!save to memory\n");
                    //error(-1,w_count,temp);
                    //return;
                    fprintf(mipsout,"sw $%d %d\n",a,STACKLIM-addr);
                }
            }
            fprintf(mipsout,"lw $%d -%d($sp)\n",a,4*(a-LOCREGSTR));//!!!
        }//global reg;*/
        for(int a=LOCREGSTR;a<=LOCREGSTR+loctop;a++){
            //global reg;
            int loc=findtable(locreg[a-LOCREGSTR]);
            int loc1=maintab.loc;
            int addr=maintab.ele[loc1][loc].addr;
            if(loc!=-1&&loc1==0){
                printf("loc1==0!!!!save to memory\n");
                //error(-1,w_count,temp);
                //return;
                if(deflocreg[a-LOCREGSTR]==1)
                    fprintf(mipsout,"sw $%d %d\n",a,STACKLIM-addr);
            }
        }
        fprintf(mipsout,"lw $ra -%d($sp)\n",GLSIZE);//ra=namelab;
        fprintf(mipsout,"lw $sp -%d($sp)\n",GLSIZE+4);//stack retreat;
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
        /*for(int a=LOCREGSTR;a<=LOCREGSTR+LOCREGTOP;a++)//local reg;
            fprintf(mipsout,"lw $%d -%d($sp)\n",a,4*(a-LOCREGSTR));*/
        /*for(int a=GLOREGSTR;a<=GLOREGSTR+GLOREGTOP;a++){
            int loc=findtable(gloreg[a-GLOREGSTR]);
            int loc1=maintab.loc;
            int addr=maintab.ele[loc1][loc].addr;
            if(loc!=-1&&a<=GLOREGSTR+glotop){
                //continue;
                if(loc1==0){
                    printf("loc1==0!!!!save to memory\n");
                    //error(-1,w_count,temp);
                    //return;
                    fprintf(mipsout,"sw $%d %d\n",a,STACKLIM-addr);
                }
            }
            fprintf(mipsout,"lw $%d -%d($sp)\n",a,4*(a-LOCREGSTR));
        }//global reg;*/
        for(int a=LOCREGSTR;a<=LOCREGSTR+loctop;a++){
            //global reg;
            int loc=findtable(locreg[a-LOCREGSTR]);
            int loc1=maintab.loc;
            int addr=maintab.ele[loc1][loc].addr;
            if(loc!=-1&&loc1==0){
                printf("loc1==0!!!!save to memory\n");
                //error(-1,w_count,temp);
                //return;
                if(deflocreg[a-LOCREGSTR]==1)
                    fprintf(mipsout,"sw $%d %d\n",a,STACKLIM-addr);
            }
        }
        fprintf(mipsout,"lw $ra -%d($sp)\n",GLSIZE);//ra=namelab;
        fprintf(mipsout,"lw $sp -%d($sp)\n",GLSIZE+4);//stack retreat;
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
#endif //COMPILER_MIPS_H
