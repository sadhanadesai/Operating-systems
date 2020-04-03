//
//  assignment1.c
//  Operating_systems
//
//  Created by Sadhana Desai on 3/1/20.
//  Copyright © 2020 Sadhana Desai. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

pid_t pid;
int count = 0;
int andcount = 0;
int status;
int killval = 0;
 
int myshell();

void handler(int signum){
wait(NULL);
}

int main(int argc , char *argv[]){
int r = myshell();
    return 0;
}


int  myshell(){
    int i = 0;
        
    while(1){
             
        char *ps[255],*ps1[255];
        char * tokens, *tokens1;
        char buff [255], buff1[255];
        int counter = 0;
        int pipecounter = 0;
        char *tok1 [255],*tok2 [255], *tok3[255];
        int a = 0;
        int k = 0;
              
        int parray[500];   //process array
               
        int pcounter = 0;   //counter
        int pipefound = 0;   //pipe tracker
           
                int fd1 = 0;
               
                int fd[2];

        printf("\nmyshell>");
              
        fgets(buff,255,stdin);
                sprintf(buff1,"%s",buff);
        tokens = strtok (buff," ");
        while (tokens != NULL){
                    if(a == 1){
            tok1[counter] = malloc((counter+1)*sizeof(tokens));
            sprintf(tok1[counter],"%s",tokens);
            counter++;
                    }else{
                      ps[0] = malloc((0+1)*sizeof(tokens));
                      sprintf(ps[0],"%s",tokens);
                      tok1[counter] = malloc((counter+1)*sizeof(tokens));
              sprintf(tok1[counter],"%s",tokens);
              counter++;
                      a = 1;
                    }
            tokens = strtok (NULL, " \n");
        }
                
                if(strcmp(tok1[0],"kill") == 0 ){
                       kill(atoi(tok1[1]),SIGKILL);
                       continue;
               }
               
                for(i=0;i<counter;i++){
                   if(strcmp(tok1[i],"|") == 0){
                        pipefound = 1;
                   }
                }
               
                

                if(pipefound == 1){
                   tokens1 = strtok (buff1,"|");
                   
                    while (tokens1 != NULL){
                tok3[pipecounter] = malloc((pipecounter+1)*sizeof(tokens1));
                sprintf(tok3[pipecounter],"%s",tokens1);
                pipecounter++;
                tokens1 = strtok (NULL, "|\n");
            }
                           
                                           
                        i = 0;
            while(i < pipecounter) {
                              char *str4[255], *str6[255];
                              char *tokens2;
                              tokens2 = strtok (tok3[i]," ");
                              int str4counter = 0;
                              int fileflag = 0;
                  while (tokens2 != NULL){
                     str4[str4counter] = malloc((str4counter+1)*sizeof(tokens2));
                     sprintf(str4[str4counter],"%s",tokens2);
                     str4counter++;
                     tokens2 = strtok (NULL, " \n");
                 }
                              
                                if(str4counter <2){
                     
                     str4[str4counter] = malloc((str4counter+ 1)*sizeof(tokens2));
                    str4[str4counter] = NULL;
                     
                }else{
                     str4[str4counter] = malloc((str4counter+1)*sizeof(tokens2));
                     str4[str4counter] = NULL;
                }
                                
                                
                                if(str4counter >1 &&  strcmp(str4[str4counter-2],"<") == 0 && strcmp(str4[0],"cat") != 0){
                         char c,*a,*l;
                                             fileflag = 1;
                         FILE *fp = fopen(str4[str4counter-1],"r");
                                             
                         if(fp == NULL){
                        printf("\nFile not found");
                        exit(1);
                         }
                         
                         c = fgetc(fp);
                         a = (char*)malloc(1000*sizeof(char));
                         while (c != EOF){
                        l = (char*)malloc(10*sizeof(char));
                        *l = c;
                        strcat(a,l);
                        c = fgetc(fp);
                         }
             
                         char * tokens1;
                         char buff1 [255];
                         int  counter1 = 1;
                         int i =0;
                     
                
                         for(i = 0; i < strlen(a); i++){
                        buff1[i] = a[i];
                         }
                        str6[0] = malloc((counter1+1)*sizeof(tokens1));
                            str6[0] = str4[0];
                        if(i-1<3){
                        tokens1 = strtok (buff1,"\n");
                        str6[counter1] = malloc((counter1+1)*sizeof(tokens1));
                        sprintf(str6[counter1],"%s",tokens1);
                        counter1++;
                        }else{
                             tokens1 = strtok (buff1," \n");
                             while (tokens1 != NULL){
                                  str6[counter1] = malloc((counter1+1)*sizeof(tokens1));
                                  sprintf(str6[counter1],"%s",tokens1);
                                  counter1++;
                                  tokens1 = strtok(NULL, " \n");
                             }
                                            }
                         if(counter1 <2){
                                 str6[counter1] = malloc((counter1+1)*sizeof(tokens1));
                                 str6[counter1] = NULL;
                                 str6[counter1+1] = malloc(((counter1+1)+1)*sizeof(tokens1));
                                 str6[counter1+1] = NULL;
                         }else{
                             str6[counter1] = malloc((counter1+1)*sizeof(tokens1));
                             str6[counter1] = NULL;
                         }
                                                                               
                                    }
                     
                pipe(fd);
                                               
                 if ((pid = fork()) < 0) {
                         fprintf(stderr, "fork failed\n");
                         exit(1);
                       }
                               
                else if (pid == 0) {
                                        
                        dup2(fd1, 0);
                                             
                    if (i+1 != pipecounter) {
                      dup2(fd[1], 1);
                    }
                    close(fd[0]);
                        
                                        if(fileflag == 1){
                                          if(strcmp(str4[0],"cat") == 0){
                                                str4[str4counter-2] = str4[str4counter-1];
                                                str4[str4counter-1] = NULL;
                                                if(execvp(str4[0],str4) == -1)
                                         fprintf(stderr, "%s: command not found\n",str4[0]);
                                      }else{
                                                         
                                             if(execvp(str4[0],str6) == -1)
                                 fprintf(stderr, "%s: command not found\n",str4[0]);

                                      }
                                      }else{
                                        if(str4counter>1 && strcmp(str4[str4counter-2],">") == 0){
                                        int fp1 = open(str4[str4counter-1], O_WRONLY|O_CREAT,0666);
                                        str4[str4counter-1] = NULL;
                                        str4[str4counter-2] = NULL;
                                        dup2(fp1,1);
                                        if(execvp(str4[0],str4) == -1)
                                   fprintf(stderr, "%s: command not found\n",str4[0]);
                                close(fp1);
                                        }else{
                                      
                                              if(execvp(str4[0],str4) == -1)
                                fprintf(stderr, "%s: command not found\n",str4[0]);
                                        }
                                        }
                }
                                
                                
                else {
                    if(waitpid(pid,&status,WUNTRACED) == -1){
                                 fprintf(stderr, "wait failed\n");
                         }
                                        
                    close(fd[1]);
                    fd1 = fd[0];
                                        i++;
                                       
                }
                }
                    continue;
                }

                if(counter <2){
                     if(counter<2){
                     tok1[counter] = malloc((counter+ 1)*sizeof(tokens));
                     tok1[counter] = NULL;
                     tok1[counter+1] = malloc(((counter+1)+1)*sizeof(tokens));
                     tok1[counter+1] = NULL;
                     }
                }else{
                     tok1[counter] = malloc((counter+1)*sizeof(tokens));
                     tok1[counter] = NULL;
                }
              
         char  *process = strtok(tok1[0],"\n");
                   if(strcmp(tok1[0],"exit") == 0){
                    parray[pcounter] = getpid();
                    int i;
                    for(i=0; i<pcounter;i++)
                          kill(parray[i],SIGKILL);
                    exit(0);
                }
        
 
                if(counter > 1 && strcmp(tok1[counter-1],"&") == 0){
                     andcount++;
                }

                if(counter >1 &&  strcmp(tok1[counter-2],"<") == 0 && strcmp(tok1[0],"cat") != 0){
                     char c,*a,*l;
                     FILE *fp = fopen(tok1[counter-1],"r");
                     if(fp == NULL){
                printf("\nFile not found");
                exit(1);
                 }
                     c = fgetc(fp);
                     a = (char*)malloc(1000*sizeof(char));
             while (c != EOF){
                        l = (char*)malloc(10*sizeof(char));
                        *l = c;
                        strcat(a,l);
            c = fgetc(fp);
             }
                     char * tokens1;
             char buff1 [255];
             int  counter1 = 1;
                     int i =0;
                     
                
                     for(i = 0; i < strlen(a); i++){
                buff1[i] = a[i];
             }
                    
                    tok2[0] = malloc((counter1+1)*sizeof(tokens1));
                    tok2[0] = tok1[0];
                    if(i-1<3){
                        tokens1 = strtok (buff1,"\n");
                        tok2[counter1] = malloc((counter1+1)*sizeof(tokens1));
            sprintf(tok2[counter1],"%s",tokens1);
            counter1++;
                    }else{
                            tokens1 = strtok (buff1," ");
                 while (tokens1 != NULL){
                    if(k == 1){
                        tok2[counter1] = malloc((counter1+1)*sizeof(tokens1));
                        sprintf(tok2[counter1],"%s",tokens1);
                        counter1++;
                    }else{
                      ps1[0] = malloc((0+1)*sizeof(tokens1));
                      sprintf(ps1[0],"%s",tokens1);
                      tok2[counter1] = malloc((counter1+1)*sizeof(tokens1));
                      sprintf(tok2[counter1],"%s",tokens1);
                      counter1++;
                      k = 1;
                    }
                    tokens1 = strtok (NULL, " \n");
                 }
                     }

                     
             if(counter1 <2){
                     if(counter1<2){
                         tok2[counter1] = malloc((counter1+1)*sizeof(tokens1));
                         tok2[counter1] = NULL;
                         tok2[counter1+1] = malloc(((counter1+1)+1)*sizeof(tokens1));
                         tok2[counter1+1] = NULL;
                     }
                           
             }else{
                     tok2[counter1] = malloc((counter1+1)*sizeof(tokens1));
                     tok2[counter1] = NULL;
             }
               }
                
               if ((pid = fork()) < 0) {
             fprintf(stderr, "fork failed\n");
             exit(1);
           }
                             
           if(pid == 0) {
                     setpgid(0, 0);
                     parray[pcounter] = getpid();
             pcounter++;
             if(counter > 1 && strcmp(tok1[counter-1],"&") == 0){
                          
                          tok1[counter-1] = NULL;
                  if(execvp(process,tok1) == -1)
                      fprintf(stderr, "%s: command not found\n",tok1[0]);
                     }else{
                          if(counter >1 && strcmp(tok1[counter-2],"<") == 0){
                                  if(strcmp(tok1[0],"cat") == 0){
                                        tok1[counter-2] = tok1[counter-1];
                                        tok1[counter-1] = NULL;
                                        if(execvp(process,tok1) == -1)
                             fprintf(stderr, "%s: command not found\n",tok1[0]);
                                  }else{
                                        tok1[counter-1] = NULL;
                                        tok1[counter-2] = NULL;
                                         if(execvp(process,tok2) == -1)
                             fprintf(stderr, "%s: command not found\n",tok2[0]);
                                  }
                          }else{
                                  if(counter>1 && strcmp(tok1[counter-2],">") == 0){
                               
                                        int fp1 = open(tok1[counter-1], O_WRONLY|O_CREAT,0666);
                                        tok1[counter-1] = NULL;
                                        tok1[counter-2] = NULL;
                                        dup2(fp1,1);
                                        if(execvp(process,tok1) == -1)
                               fprintf(stderr, "%s: command not found\n",tok1[0]);
                                close(fp1);
                                   }else{
                                             
                                      if(execvp(process,tok1) == -1)
                                fprintf(stderr, "%s: command not found\n",tok1[0]);
                                        }
                                 }
               }
                        }
                        
                        if(!(counter > 1 && strcmp(tok1[counter-1],"&") == 0 )){
                                if(waitpid(pid,&status,WUNTRACED) == -1){
                                 fprintf(stderr, "wait failed\n");
                         }
                        }
              
    }
 return 0;
}
