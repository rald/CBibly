#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
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

  if(argc<3 || argc>5) {
    printf("syntax: %s search <text>\n",argv[0]);
    printf("syntax: %s <book> <chapter> [start verse] [end verse]\n",argv[0]);
    return -1;
  }

  FILE *fp=fopen("kjv.csv","r");

  if(strcasecmp(argv[1],"search")==0) {

    char *search=argv[2];
        
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

      char str[4096];

      sprintf(str,"%s %d:%d %s",bname,cnumber,vnumber,text);

      if(strcasestr(str,search)) { 
        printf("%s %d:%d -> %s\n\n",bname,cnumber,vnumber,text);
      }

      free(tokens);
      tokens=NULL;
      ntokens=0;

      free(line);
      line=NULL;
      llen=0;
    }

    
  } else {

    char *book=argv[1];
    unsigned int chap=0;
    unsigned int svers=0;
    unsigned int evers=0;

    if(argc>=3) chap=atoi(argv[2]);
    if(argc>=4) svers=atoi(argv[3]);
    if(argc==5) evers=atoi(argv[4]);

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

      if(strcasecmp(book,bname)==0 && cnumber==chap && ((argc==5 && svers!=0 && evers!=0 && (vnumber>=svers && vnumber<=evers)) || (argc==4 && vnumber==svers) || argc==3)) { 
        printf("%s %d:%d -> %s\n\n",bname,cnumber,vnumber,text);
      }

      free(tokens);
      tokens=NULL;
      ntokens=0;

      free(line);
      line=NULL;
      llen=0;
    }

  }
  
  return 0;
}
