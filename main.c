#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#define TEACHER_SEMAPHORE_NAME "/teacher_sem"
#define STUDENT_SEMAPHORE_NAME "/answering_ticket_sem"

sem_t *answering_ticket_sem = NULL, *teacher_sem = NULL;
int pid;
char memn_pid[] = "shared-memory-posix";

int *addr_pid;
int shm_pid;
int mem_pid_size = sizeof(int);

void sigfunc(int sig)
{
    if (sig != SIGINT)
    {
        return;
    }

    if (getpid() == pid)
    {

        if (sem_destroy(answering_ticket_sem) < 0)
        {
            printf("Can't close answering_ticket_sem\n");
        }
        else
        {
            printf("answering_ticket_sem has been closed\n");
        }

        if (sem_destroy(teacher_sem) < 0)
        {
            printf("Can't close teacher_sem\n");
        }
        else
        {
            printf("teacher_sem has been closed\n");
        }
        if (shm_unlink(memn_pid) == -1)
        {
            printf("Shared memory pid is absent\n");
        }

        printf("Teacher finished exam\n");
    }
    else
    {
        printf("Students go to bar\n");
    }

    exit(10);
}

void init()
{
    if ((shm_pid = shm_open(memn_pid, O_CREAT | O_RDWR, 0666)) == -1)
    {
        printf("Opening error\n");
        perror("shm_open");
        exit(-1);
    }
    ftruncate(shm_pid, mem_pid_size);
    addr_pid = (int *)mmap(0, mem_pid_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm_pid, 0);
    if (addr_pid == (int *)-1)
    {
        printf("Error getting pointer to shared memory\n");
        perror("mmap");
        exit(-1);
    }
    
    if ((answering_ticket_sem = sem_open(STUDENT_SEMAPHORE_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED )
    {
        printf("Can't open answering_ticket_sem\n");
        exit(-1);
    }
    else
    {
        printf("Open answering_ticket_sem\n");
    }

    if ((teacher_sem = sem_open(TEACHER_SEMAPHORE_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
    {
        printf("Can't open teacher_sem\n");
        exit(-1);
    }
    else
    {
        printf("Open teacher_sem\n");
    }
}

int main()
{
    signal(SIGINT, sigfunc);
    signal(SIGTERM, sigfunc);
    srand(time(NULL));
    init();
    pid = getpid();
    fork();

    if (pid == getpid())
    {
        printf("Teacher pid = %d, ppid = %d\n", getpid(), getppid());
        printf("Exam started\n");
        sem_post(answering_ticket_sem);

        while (1)
        {
            sem_wait(teacher_sem);
            printf("Teacher started listening to student\'s %d answer\n", addr_pid[0]);
            sleep(rand() % 10 + 1);
            int mark = rand() % 10 + 1;
            printf("Student %d got mark %d for exam\n", addr_pid[0], mark);
            printf("%d goes home\n\n", addr_pid[0]);
            kill(addr_pid[0], SIGINT);
            printf("Teacher waiting for next student\n");
            sem_post(answering_ticket_sem);
        }
    }
    else
    {
        printf("Creator pid = %d, ppid = %d\n\n", getpid(), getppid());
        while (1)
        {
            sleep(rand() % 10 + 1);
            if (fork() == 0)
            {
                break;
            }
        }
        int ticket_number = rand() % 100;
        printf("Student %d come to exam and start writing ticket number %d\n", getpid(), ticket_number);
        sleep(rand() % 10 + 1);
        sem_wait(answering_ticket_sem);
        printf("%d student start answering his ticket\n", getpid());
        addr_pid[0] = getpid();
        sem_post(teacher_sem);
    }

    return 0;
}