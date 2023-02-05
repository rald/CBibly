#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOK_MAX 256

void tokenize(char ***tokens,size_t *ntokens,char *s,char *d) {
  char *token=strtok(s,d);
  while(token) {
    *tokens=realloc(*tokens,sizeof(**tokens)*(*ntokens+1));
    (*tokens)[(*ntokens)++]=strdup(token);
    token=strtok(NULL,d);
  }
}

char *rmnl(char *s) {
  char *p=strchr(s,'\n');
  if(p) *p='\0';
  return s;
}

int main(int argc,char **argv) {

  if(argc!=3 && argc!=4) {
    printf("syntax: %s book chapter [verse]\n",argv[0]);
    return -1;
  }

  char *book=argv[1];
  int chap=atoi(argv[2]);
  int vers=0;

  if(argc==4) vers=atoi(argv[3]);

  FILE *fp=fopen("kjv.csv","r");

  char *line=NULL;
  size_t llen=0;
  ssize_t rlen=0;

  while((rlen=getline(&line,&llen,fp)) && rlen>0) {

    char **tokens=NULL;
    size_t ntokens=0;

    tokenize(&tokens,&ntokens,rmnl(line),"|");

    char *bname=tokens[0];
    int cnumber=atoi(tokens[1]);
    int vnumber=atoi(tokens[2]);
    char *text=tokens[3];

    if(strcasecmp(book,bname)==0 && ((chap==cnumber && vers!=0 && vers==vnumber) || (chap==cnumber && vers==0))) { 
      printf("%s %d:%d -> %s\n\n",bname,cnumber,vnumber,text);
    }

    free(line);
    line=NULL;
    llen=0;
  }
  
  return 0;
}
