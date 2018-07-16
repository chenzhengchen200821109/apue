#include "apue.h"

char *pathname = "/home/chenzheng/Documents/github-apue/open.c";

int main()
{
    pid_t pid;
    static int sockfd[2] = { -1, -1 };

    TELL_WAIT();

    if (sockfd[0] < 0) {
        if (fd_pipe(sockfd) < 0) {
            err_ret("fd_pipe error");
            return (-1);
        }
        if ((pid = fork()) < 0) {
            err_ret("fork error");
            return (-1);
        } else if (pid == 0) { /* child */
            int newfd;
            off_t offset;

            //close(sockfd[0]);
            if ((newfd = open(pathname, O_RDONLY)) < 0) {
                err_sys("open error");
                exit(0);
            }
            if ((offset = lseek(newfd, 50, SEEK_CUR)) < 0)
                err_sys("lseek error");
            printf("child newfd %d: new offset: %d\n", newfd, offset);
            if (send_fd(sockfd[1], newfd) < 0)
                err_sys("send_fd error");
            close(newfd);

            TELL_PARENT(getppid());
            WAIT_PARENT();
        } else { /* parent */
            int newfd;
            off_t offset;

            WAIT_CHILD();

            //close(sockfd[1]);
            if ((newfd = recv_fd(sockfd[0], write)) < 0)
                err_sys("recv_fd error");
            if ((offset = lseek(newfd, 0, SEEK_CUR)) < 0)
                err_sys("lseek error");
            printf("parent newfd %d: new offset: %d\n", newfd, offset);

            TELL_CHILD(pid);
        }
        exit(0);
    }
}

