#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<error.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>

int write_data_to_file(char *ptr, int len)
{
    int i;
    for(i=0; i<len; i++) {
        ptr[i] = 'c';
    }

    return 0;
}

int main(int argc,char *argv[]){
    int fd, len;
    char *ptr;
    if(argc < 2){
        printf("please enter a file\n");    
        return 0;
    }   
    if((fd = open(argv[1], O_CREAT|O_RDWR))<0){
        perror("open file error");
        return -1; 
    }   

    //len=lseek(fd, 0, SEEK_END);   
    len = 1024*5;
    ptr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);//读写得和open函数的标志相一致，否则会报错
    if(ptr == MAP_FAILED){
        perror("mmap error");
        close(fd);
        return -1; 
    }   
    close(fd);//关闭文件也ok
    printf("length is %d\n", (int)strlen(ptr));
    printf("the %s content is:\n%s\n", argv[1], ptr);

    //write_data_to_file(ptr, len);
    getchar();

    printf("the %s content is:\n%s\n",argv[1],ptr);
    munmap(ptr, len);//将改变的文件写入内存
    
    return 0;
}
