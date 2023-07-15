#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <string.h>

#include <errno.h>


#define CNT_BYTE_READ 20
void define_errno(int my_errno);


// void print_str() {
//     printf("buf:%s", buf);

// }

void read_file(int file_descriptor, char *buf) {

    char buffer_for_read[255];



    int res = read(file_descriptor, buffer_for_read, CNT_BYTE_READ);
    // lseek(file_descriptor, 13, SEEK_CUR);
    // read(file_descriptor, buffer_for_read + 13, 13);

    // printf("file_descriptor: %d\n", file_descriptor);
    // printf("res: %d\n", res);
    // printf("%s", buffer_for_read);
    strncpy(buf, buffer_for_read, res);
    buf[res] = '\0';
        

}

int main() {
    int file_descriptor = open("file.txt", O_CREAT|O_RDWR, S_IRWXU);

    while(1) {
        char buf[255];
        read_file(file_descriptor, buf);
        printf("%s", buf);
        if (buf[0] == '\0') break;
    }

    // int file_descriptor = open("file.txt", O_CREAT|O_RDWR, S_IRWXU);

    // printf("file_descriptor: %d\n", file_descriptor);
    close(file_descriptor);



    // char buffer_for_read[1024];

    // int res = read(file_descriptor, buffer_for_read, 13);
    // // read(file_descriptor, buffer_for_read + 13, 13);

    // printf("res: %d\n", res);
    // printf("%s", buffer_for_read);













    const char buffer[] = "Hello world!\n"; 
    size_t size = sizeof(char) * strlen(buffer); 
    

    const char buffer2[] = "hEllo world!\n"; 





    ssize_t rv = write(file_descriptor, buffer, size);
    // write(file_descriptor, buffer2, size);

    // printf("error_: %d\n", errno);



    // printf("return_value: %ld\nsize: %ld\n", rv, size);
    // close(file_descriptor);

    return 0;
}




void define_errno(int my_errno) {
    switch (my_errno)
    {
    case EAGAIN:
        printf("EAGAIN\n");
        break;

    case EBADF:
        printf("EBADF\n");
        break;

    case EDESTADDRREQ:
        printf("EDESTADDRREQ\n");
        break;

    case EDQUOT:
        printf("EDQUOT\n");
        break;

    case EFAULT:
        printf("EFAULT\n");
        break;

    case EFBIG:
        printf("EFBIG\n");
        break;

    case EINTR:
        printf("EINTR\n");
        break;

    case EINVAL:
        printf("EINVAL\n");
        break;

    case EIO:
        printf("EIO\n");
        break;

    case ENOSPC:
        printf("ENOSPC\n");
        break;

    case EPERM:
        printf("EPERM\n");
        break;

    case EPIPE:
        printf("EPIPE\n");
        break;


    default:
        printf("Nothing...\n");

        break;
    }
}














