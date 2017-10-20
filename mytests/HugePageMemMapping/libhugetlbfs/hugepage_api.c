#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hugetlbfs.h>

#define SIZE 1024*1024*5

int main(void)
{
	char *ptr;
	ptr = get_hugepage_region(SIZE, GHR_STRICT);
	if(!ptr){
		printf("get hugepage err: %s\n", strerror(errno));
		return -1;
	}

	memset(ptr, '\0', SIZE);

	getchar();
	free_hugepage_region(ptr);
	return 0;
}
