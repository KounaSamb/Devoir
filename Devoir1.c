#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char* optarg;
extern int optopt;

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Nombre d'argument insuffisant\n");
        return 0;
    }

    if(argc > 4)
    {
        printf("Exces d'argument\n");
        return 0;
    }
	int opt;
  int n=0,m=0;
  int f=0,g=0;
	while((opt = getopt(argc,argv,":f:g:")) != -1)
	{
		switch(opt)
		{
			case 'f':
                f=1;
                if(optarg)
                    n=atoi(optarg);
                break;
			case 'g': 
                g=1;
                if(optarg)
                    m=atoi(optarg);
                break;
			case ':':
                if(optopt=='f')
                    f=1;
                if(optopt=='g')
                    g=1;
                break;
			case '?': 
                printf("Erreur : Option %c inconnue\n", optopt);
                break;
		}
	}
    if(f && g)
    {
        if(n)
        {
            printf("fog(%d) = %d\n",n,1<<(2*n));
        }
        else
            if(m)
            {
                printf("gof(%d) = %d\n",m,1<<(m+1));
            }
            
    }
    else
    {
        if(f)
            printf("f(%d) = %d\n",n,1<<n);
        else
            printf("g(%d) = %d\n",m,2*m);
    }

	return 0;
}
