#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

void split(char* file);
void join(char* file[],int nb_file);
int n;
int main(int argc,char* argv[])
{
    if(argc<3)
    {
        perror("Nombre d'argument insuffisant\n");
        return 0;
    }
    n=atoi(argv[2]);
    if(argc==3)
       split(argv[1]);
    else
    {
        int nb_file=argc-1;
        char* arg[nb_file];
        for(int i=0;i<nb_file;i++)
        {
            arg[i]=argv[i+1];
        }
        join(arg,nb_file);
    }
    
    return 0;
}

void split(char* file)
{
   int fs=open(file,O_RDONLY);
   if(fs==-1)
   {
      perror("Erreur d'ouverture du fichier source\n");
      return;
   }

   struct stat Fichier;
   int resultat=fstat(fs,&Fichier);
   if(resultat==-1)
   {
       printf("Erreur de recuperation des informations de %s\n",file);
       return;
   }
   int taille=Fichier.st_size;
   int permission=Fichier.st_mode;

   int nb;
   if(taille/n)
       nb=floor(taille/n)+1;
    else
       nb=taille/n;
    
    char parti[nb][15];
    for(int i=0;i<nb;i++)
    {
        sprintf(parti[i],"parti%d",i+1);
    }

    char buff;
    int i=0,k=0;
    while(i<taille)
    {
        int fd=open(parti[k],O_WRONLY | O_CREAT,permission);
        if(fd==-1)
        {
            printf("Erreur d'ouverture de %s\n",parti[(i)%n]);
            return;
        }
        int j=0;
        while(j<n)
        {
            if((read(fs,&buff,sizeof(char)))>0)
            {
                if(write(fd,&buff,sizeof(char)) <=0)
                {
                    printf("Erreur d'ecriture dans %s\n",parti[i]);
                    return;
                }
            }
            j++;
        }
        close(fd);
        i=i+n;
        k++;
    }
    close(fs);
}

void join(char* file[],int nb_file)
{
   int fd=open(file[0],O_WRONLY | O_CREAT|O_EXCL,644);

   if(fd==-1)
   {
      printf("Erreur d'ouverture de %s\n",file[0]);
      return;
   }

    char buff;
    for(int i=1;i<nb_file;i++)
    {
        int fs=open(file[i],O_RDONLY);
        while(read(fs,&buff,sizeof(char))>0)
        {
            if(write(fd,&buff,sizeof(char)) <=0)
            {
                printf("Erreur d'ecriture dans %s\n",file[i]);
                return;
            }
        }
        close(fs);
    }
    close(fd);
}
