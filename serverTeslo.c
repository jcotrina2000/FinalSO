#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>


void salir(int signal);
void recoger_hijos_zombies(int sgn);


int main(int argc, char *argv[])
{
	int server_sockfd, client_sockfd;
	int server_len ; 
	int rc ; 	
	pid_t pid;
	sem_t *sem;
	unsigned client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htons(INADDR_ANY);
	server_address.sin_port = htons(7734) ; 
	server_len = sizeof(server_address);
	
	rc = bind(server_sockfd, (struct sockaddr *) &server_address, server_len);
	printf("RC from bind = %d\n", rc ) ; 
	

	rc = listen(server_sockfd, 5);
	printf("RC from listen = %d\n", rc ) ; 
	
	signal(SIGINT, salir);
	signal(SIGCHLD, recoger_hijos_zombies);
	
	while(1){
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
		
		if((pid = fork()) == 0){
			close(server_sockfd);
			while(1){	
				int sensor_reading;
				//printf("server waiting\n");
				rc = read(client_sockfd, &sensor_reading, 1);
				printf("Lectura valor sensor = %d\n", sensor_reading );	
				write(client_sockfd, &sensor_reading, 1);
	}
		}
	}
	
	printf("after accept()... client_sockfd = %d\n", client_sockfd) ; 
	
	/*while(1)
	{
		int sensor_reading;
		//printf("server waiting\n");
		rc = read(client_sockfd, &sensor_reading, 1);
		printf("Lectura valor sensor = %d\n", sensor_reading );	
		write(client_sockfd, &sensor_reading, 1);
	}*/
	//tupla = make_tuple(sensor_reading)
	//printf("");

	printf("server exiting\n");

	close(client_sockfd);
	return 0;
}

void salir(int signal){
	printf("Conexión finlizada. Adios!\n");
	exit(0);
}

void recoger_hijos_zombies(int sgn){
	while(waitpid(-1, 0, WNOHANG) > 0)
		;
	return;
}

/*void atender_cliente(int conn_fd){
	
}

void *thread_function(void *args){
	pthread_detach(pthread_self());
	while(1){
		atenderCliente();
		close();
	}
}*/



