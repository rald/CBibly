#include "ezxml.h"

int main(void) {

  ezxml_t bible = ezxml_parse_file("kjv.xml"), book,chap,verse;
  const char *bname;
  int cnumber,vnumber;
  char *text;
  int i=1;
   
  for (book = ezxml_child(bible, "BIBLEBOOK"); book; book = book->next) {

    for (chap = ezxml_child(book, "CHAPTER"); chap; chap = chap->next) {

      for (verse = ezxml_child(chap, "VERS"); verse; verse = verse->next) {

        bname = ezxml_attr(book, "bname");
        cnumber = atoi(ezxml_attr(chap, "cnumber"));
        vnumber = atoi(ezxml_attr(verse, "vnumber"));
        text = ezxml_txt(verse);

        printf("%s|%d|%d|%s\n",bname,cnumber,vnumber,text);
      }

    }


  }
  ezxml_free(book);

return 0;
}
