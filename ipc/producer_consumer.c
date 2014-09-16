#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

const size_t CACHE_LENGTH = 1020;

//采用信号灯机制实现p、v操作的数据结构和函数
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

// acquire
int P(int semid, int semnum)
{
    struct sembuf sops = { semnum, -1, SEM_UNDO };
    return (semop(semid, &sops, 1));
}

// release
int V(int semid, int semnum)
{
    struct sembuf sops = { semnum, +1, SEM_UNDO };
    return (semop(semid, &sops, 1));
}

int main(int argc, char *argv[])
{

    //定义共享缓冲区及其信号灯
    int readbuf_id = shmget(IPC_PRIVATE, CACHE_LENGTH + sizeof(unsigned),
                            IPC_CREAT | IPC_EXCL | 0666);
    int rbuf_empty = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);
    int rbuf_max = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);
    int writebuf_id = shmget(IPC_PRIVATE, CACHE_LENGTH + sizeof(unsigned),
                             IPC_CREAT | IPC_EXCL | 0666);
    int wbuf_empty = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);
    int wbuf_max = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);

    int finish_id = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);

    int get_id = 0;
    int put_id = 0;
    int copy_id = 0;

    union semun semopts;
    FILE *out;
    FILE *in;
    int n;

    //打开源和目标文件
    if (argc != 3)
    {
        puts("arguments error");
        return 0;
    }
    if ((in = fopen(argv[1], "rb")) == NULL)
    {
        puts("can't open input file");
        return 0;
    }
    if ((out = fopen(argv[2], "wb")) == NULL)
    {
        puts("can't open output file");
        fclose(in);
        return 0;
    }

    //信号灯赋值
    semopts.val = 1;
    semctl(rbuf_empty, 0, SETVAL, semopts);
    semctl(wbuf_empty, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(rbuf_max, 0, SETVAL, semopts);
    semctl(wbuf_max, 0, SETVAL, semopts);
    semctl(finish_id, 0, SETVAL, semopts);
    /*semctl(part_over, 0, SETVAL, semopts);*/
    /*semctl(over, 0, SETVAL, semopts);*/

    // get
    if ((get_id = fork()) == 0)
    {
        unsigned char *s = shmat(readbuf_id, 0, 0);
        puts("get:start to get");
        do
        {
            P(rbuf_empty, 0);

            //加入copy
            n = fread(s + sizeof(unsigned), 1, CACHE_LENGTH, in);
            /*n = fread(s + sizeof(unsigned),  1, 2, in);*/
            printf("read: %d\n", n);
            /*putchar('\n');*/

            *(unsigned *)s = n;
            /*fwrite(s + sizeof(unsigned),  1, *(unsigned *)s, stdout);*/
            V(rbuf_max, 0);

            if (*(unsigned *)s == 0)
                break;
        } while (1);
        fclose(in);
        /*sleep(5);*/
        P(finish_id, 0);
        shmdt(s);

        //请加入保证三个进程同步结束的机制
        puts("get: get ended");
        return 0;
    }
    // copy
    if ((copy_id = fork()) == 0)
    {
        unsigned char *s1 = shmat(readbuf_id, 0, 0);
        unsigned char *s2 = shmat(writebuf_id, 0, 0);

        char buf[CACHE_LENGTH + sizeof(unsigned)];
        puts("copy: start to copy");
        do
        {
            P(rbuf_max, 0);

            //加入内容
            n = *(unsigned *)s1;
            printf("copy: %d\n", n);
            memcpy(buf, s1, CACHE_LENGTH + sizeof(unsigned));
            /*fwrite(s1 + sizeof(unsigned),  1, *(unsigned*)s1, stdout);*/

            V(rbuf_empty, 0);

            P(wbuf_empty, 0);

            //加入内容
            /**(unsigned *)s2 = n;*/
            memcpy(s2, buf, CACHE_LENGTH + sizeof(unsigned));
            printf("\ncopy: \n");
            /*fwrite(s2 + sizeof(unsigned),  1, *(unsigned*)s2, stdout);*/
            printf("\n\n");

            V(wbuf_max, 0);

            if (*(unsigned *)s2 == 0)
                break;
        } while (1);

        P(finish_id, 0);
        shmdt(s1);
        shmdt(s2);

        //请加入保证三个进程同步结束的机制
        puts("copy: copy ended");
        /*sleep(5);*/
        return 0;
    }
    // put
    if ((put_id = fork()) == 0)
    {
        unsigned char *s = shmat(writebuf_id, 0, 0);
        puts("put: start to put");
        char buf_put[CACHE_LENGTH + sizeof(unsigned)];
        do
        {
            P(wbuf_max, 0);

            //加入内容
            /*memcpy(buf_put, s, CACHE_LENGTH + sizeof(unsigned));*/
            printf("put: %d\n", *(unsigned *)s);
            /*if (*(unsigned*)s > 0)*/
            /*{*/
            fwrite(s + sizeof(unsigned), 1, *(unsigned *)s, out);
            fwrite(s + sizeof(unsigned), 1, *(unsigned *)s, stdout);
            /*}*/
            fflush(out);

            V(wbuf_empty, 0);

            if (*(unsigned *)s == 0)
            {
                V(finish_id, 0);
                break;
            }
        } while (1);
        shmdt(s);
        fclose(out);

        //请加入保证三个进程同步结束的机制

        puts("put: put ended");
        return 0;
    }

    //请加入保证三个进程同步结束的机制

    //请加入释放缓冲区和信号灯的机制
    /*int status;*/
    P(finish_id, 0);
    shmctl(readbuf_id, IPC_RMID, 0);
    shmctl(writebuf_id, IPC_RMID, 0);

    return 0;
}
