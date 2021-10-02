#include<stdio.h>
#include<sys/types.h>//fork
#include<unistd.h>//system call
#include<sys/wait.h>//wait
#include<stdlib.h>//exit
void error_handling(char * message);

int main(int argc, char *argv[])
{	
	pid_t pid;
	int statloc;
	switch(pid=fork()){
		case -1:/*fork failed*/
			error_handling("fork failed");
			exit(-1);
		case 0:/*child process*/	
				printf("I\'m a child : PID반환값 %d 자식PID %d  부모PID %d\n", (int)pid, (int)getpid(), (int)getppid());
				fflush(stdout);
				usleep(100000);
				//while(1){}
				exit(7);//exit(0);도 가능
		default:/*parent process*/
			printf("I\'m your father : first child PID %d\n", (int)pid);
			rewind(stdin);
			/*child process한개 더 생성*/
			pid = fork();
			if(pid==0){
				printf("I\'m a second child child PID %d\n", (int)getpid());
				exit(3);//바로 리턴하고 종료
			}
			else{//wait함수에 들어온 인자를 출력하여 좀비 프로세스 알아보기
				for(int i=0;i<2;i++){
					if(wait(&statloc)==-1){
						error_handling("wait함수 system call 인터럽트\n");
					}
					else if(WIFEXITED(statloc))
						printf("정상종료 Child pass num : %d\n", WEXITSTATUS(statloc));
					else if(WIFSIGNALED(statloc))
						printf("비정상종료 Child pass num : %d\n", WTERMSIG(statloc));
					else{}

					usleep(3000000);//좀비 프로세스 확인 위해 부모 프로세스 종료 늦춤
				}
			}
			break;


	}
	exit(0);
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
