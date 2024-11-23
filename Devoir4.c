#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int n,compteur=0,j=0;
sem_t sem1;
sem_t sem2;

void *Thread1(void* arg)
{
    while(1)
    {
        sem_wait(&sem1);
        printf("%d   %d\n",j++,compteur++);
        if(compteur==n)
           for(int i=0;i<2*n;i++)
              sem_post(&sem2);
    }
    pthread_exit(NULL);
}

void *Thread2(void* arg)
{
    while(1)
    {
        sem_wait(&sem2);
        printf("%d   %d\n",j++,compteur--);
        if(compteur==(-n))
           for(int i=0;i<2*n;i++)
              sem_post(&sem1);
    }
    pthread_exit(NULL);
}

int main(int argc,char* argv[])
{
    if(argc<2)
    {
        perror("Nombre d'argument insuffisant\n");
        return 0;
    }
    n=atoi(argv[1]);
    int resultat;

    resultat=sem_init(&sem1,0,n);
    if(resultat==-1)
    {
        perror("Erreur de creation du semaphore 1\n");
        return 0;
    }
    resultat=sem_init(&sem2,0,0);
    if(resultat==-1)
    {
        perror("Erreur de creation du semaphore 2\n");
        return 0;
    }

    pthread_t th1,th2;
        resultat=pthread_create(&th1,NULL,Thread1,(void*) NULL);
        if(resultat==-1)
        {
            perror("Erreur de creation du Thread 1\n");
            return 0;
        }
    resultat=pthread_create(&th2,NULL,Thread2,(void*) NULL);
        if(resultat==-1)
        {
            perror("Erreur de creation du Thread 2\n");
            return 0;
        }

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);

    return 0;
}
