#include <stdio.h>
#include "dict"

int main()
{
  dict_t  *dict = dict_init()
  
  dict_push(dict, "Name", "Zhangfei");
  dict_push(dict, "Title", "Hero");
  
  char *str = NULL;
  dict_search(dict, "Name", &str);
  printf("str: %s\n", str);
  dict_search(dict, "Title", &str);
  printf("str: %s\n", str);
  
  dict_free(dict);
  
  return 0;
}
