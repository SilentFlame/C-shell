#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <error.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
int mystrcmp(char const *, char const *);
char hostname[1024];

char *replace_str(char *str, char *orig, char *rep){
	static char buffer[4096];
	char *p;

	if(!(p = strstr(str, orig)))
		return str;

	strncpy(buffer, str, p-str);
	buffer[p-str] = '\0';

	sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

	return buffer;
}

#define BUFFERSIZE 200
int i;	
char* uniq_spc(char* str){
    char *from, *to;
    int spc=0;
    to=from=str;
    while(1){
        if(spc && *from == ' ' && to[-1] == ' ')
            ++from;
        else {
            spc = (*from==' ')? 1 : 0;
            *to++ = *from++;
            if(!to[-1])break;
        }
    }
    return str;
}

void ParseBuffer(char *buffer,char** args){
 i=0;
	for(i=0;i<BUFFERSIZE;i++){
		args[i]=strsep(&buffer," ");
		//printf("%s\n",args[i]);
		if(args[i]==NULL){
			break;
		}
	}
}

int Execute(char** args);

int main(){
	int background;
	char cwd[1024];
	cwd[1023]='\0';
	hostname[1023] = '\0';
	char *original=getcwd(cwd, sizeof(cwd));
	gethostname(hostname, 1023);
	char *prompt = hostname;
	char **args; 
	char buffer[BUFFERSIZE];
	char *current;
	char *a = ">";
	char *tok; 
	tok = strtok (buffer," ");
	char *vin,ch;
	char string[1024];
	strcpy(string,original);
	int status;
	int j;
	int in=0,out=0;
	while(1){ 
		bzero(buffer, BUFFERSIZE);
		current=getcwd(cwd, sizeof(cwd));
		
		vin=replace_str(current,string,"~");
		
		printf("%s@%s:%s%s",getenv("USER"),prompt,vin,a); 


		fgets(buffer, BUFFERSIZE, stdin);
		if(buffer[strlen(buffer)-1]=='\n'){
			buffer[strlen(buffer)-1]='\0';
		}
		FILE *fp;
		//printf("%s\n",buffer[strlen(buffer)-2]);
		//fp=fopen(buffer[strlen(buffer)-1],'w+');
		//	if(strstr(buffer,">")!=NULL){
		//////		out=1;
	//			strstr(buffer,">");
	//		}
//
//			if(strstr(buffer,"<")!=NULL){
//				in=1;
//				strstr(buffer,"<");
//			}
		//puts(buffer);
	
//	if(out){
//		for(j=0;j<sizeof(buffer);j++){
//
//		}

//		}	
			
		//int fd[2];
		//pipe(fd);

		
		uniq_spc(buffer);

		ParseBuffer(buffer,args);
		
		background=(mystrcmp(args[i-1],"&")==0);
			if(background){
				args[i-1]=NULL;
			}
				pid_t pid;
				pid=fork();
			
				
				if(pid==0){
					if(background){
						pid=fork();
							if(pid)
								exit(0);
						execvp(args[0],args);
					}

					if(mystrcmp(args[0],"cd") == 0){
						if(args[1]==NULL) {
							chdir(string);
						}
						else{
							cd(args[1]);
						}
					}

					else if(mystrcmp(args[0],"pwd")==0){
						printf("%s\n",getcwd(cwd, sizeof(cwd)));
					}	

					else if(mystrcmp(args[0],"echo")==0){
						if(mystrcmp(args[1],"$USER")==0){
							printf("%s\n",getenv("USER"));
						}

						else
							printf("%s\n",args[1]);
					}
					else if(mystrcmp(args[0],"exit")==0){
						break;
					}
					else if(Execute(args)==0){
							break;
				}

				}
				
				else if (!background) {
                         pid = waitpid(pid, &status, 0);
                     }

				else if(pid<0){
					fprintf(stderr, "Fork failed");
				}

				else{
					wait(NULL);
				}
	}
	return 0;
}

int Execute(char **args){
	execvp(args[0],args);
	chdir(args[1]);

	char *error=strerror(errno);
	printf("shell: %s :%s\n",args[0],error);
	return 0;
}

int mystrcmp(char const *p, char const *q)
{
	int i = 0;
	for(i = 0;q[i];i++)
	{
		if(p[i] != q[i])
			return -1;
	}
	return 0;
}

int cd(char *pth){
	char path[BUFFERSIZE];
	strcpy(path,pth);

	char *token;

	char cwd[BUFFERSIZE]; 
	if(pth[0] != '/'){
		getcwd(cwd,sizeof(cwd));
		strcat(cwd,"/"); 
		strcat(cwd,path);

		if(chdir(cwd)==-1)
		{
			perror("Cannot cd");
		}

	}
	else{
		if(chdir(pth)==-1){
			perror("Cannot cd");
		}
	}

	return 0;
}
