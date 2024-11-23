#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

void creation(char* file,int n,int bin)
{
    int m[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = rand() % 10;
        }
    }
    srand(time(NULL));

    int fd = open(file,O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (fd == -1) 
    {
        printf("Erreur d'ouverture de %s\n",file);
        return;
    }
    if (bin) {
        for (int i = 0; i < n; i++)
        {
            if (write(fd, m[i], sizeof(m[i])) != sizeof(m[i])) 
            {
                printf("Erreur lors de l'écriture dans %s",file);
                return;
            }
        }
        
        close(fd);
    } 
    else
    {
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++)
            {
                if(write(fd,&m[i][j],sizeof(int))<=0)
                {
                    printf("Erreur lors de l'écriture dans %s",file);
                    return;
                }
            }
        }
        close(fd);
    }
}

void affichage(int n, const char *file, int bin) {
    int m[n][n];
    int fd = open(file, O_RDONLY);
    if (fd == -1) 
    {
        printf("Erreur lors de l'ouverture de %s",file);
        return;
    }

    if (bin) 
    {
        read(fd, m, sizeof(int)*n*n);

        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                printf("%02x 00 00 00 ", m[i][j]);
            }
            printf("\n");
        }
    } 
    else 
    {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) 
            {
                if(read(fd,&m[i][j],sizeof(int))>0)
                   printf("%d ", m[i][j]);
            }
            printf("\n");
        }
    }
    close(fd);
}

extern int optopt;
extern char *optarg;
int main(int argc,char* argv[])
{
    int create=0,affiche=0,bin=1,dim=0;
    char* file;
    int opt;
    while((opt=getopt(argc,argv,":cad:btf:"))!=-1)
    {
        switch (opt)
        {
            case 'c':create=1;break;
            case 'a':affiche=1;break;
            case 'b':bin=1;break;
            case 't':bin=0;break;
            case 'd':dim=atoi(optarg);break;
            case 'f':file=optarg;break;
            case ':': printf("Erreur : l'option %c attend un argument\n",optopt);break;
	    case '?': printf("Erreur : Option %c inconnue\n", optopt);break;
        }
    }
    if(!create && !affiche)
    {
        printf("Erreur : Option obligatoire non fournie\n");
    }
    if(dim && file)
    {
	if(create)
   	{
           creation(file,dim,bin);
   	}
    	if(affiche)
    	{
       	    affichage(dim,file,bin);
    	}
     }
     else
        printf("Erreur : Veillez fournir tous les information necessaire\n");

    return 0;
}
