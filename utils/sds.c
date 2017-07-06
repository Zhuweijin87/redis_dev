#include <stdio.h>
#include <string.h>

typedef struct sds_s
{
	int		len;		//使用字节
	int		free;		//未用字节
	char	buf[];
}sds_t;

sds_t *sds_new(const char *str)
{
	size_t	size = (str == NULL) ? 0 : strlen(size);
	
}

