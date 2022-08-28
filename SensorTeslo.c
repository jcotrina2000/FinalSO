#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <semaphore.h>
#include "ziggurat.h"

#define DEBUG 1
//FUNCIONES
//void desconectarProceso();

int pid_server, fd_cli;

int main(int argc, char *argv[])
{
/*Declaración de variables*/
printf("Programando de lado del cliente\n");
float fn[128],Tfreq;
int ID_sensor,Stype,Davg,Dsd,Tsd,sensor_reading;
uint32_t kn[128];
unsigned int delay;
uint32_t seed;
float value;
float wn[128];
int rc, len;

if (argc != 7) {
	fprintf(stderr,"usage: tiktoker <Tmin:integer> <Tmax:integer value> <Prob:1|0> \n");	
	return -1;
}

ID_sensor=atoi(argv[1]);
Stype=atoi(argv[2]);
Davg=atoi(argv[3]);
Dsd=atoi(argv[4]);
Tfreq=atof(argv[5]);
Tsd=atoi(argv[6]);


if ( ID_sensor< 0 || Stype < 0 || Davg < 0 || Dsd < 0|| Tfreq < 0|| Tsd < 0) {
	fprintf(stderr,"Arguments must be non-negative\n");	
	return -1;
}

  r4_nor_setup ( kn, fn, wn );
  srand(time(NULL));   // Initialization, should only be called once.
  seed = rand(); 
  /*Creación del socket para el cliente.*/
  fd_cli = socket(PF_INET, SOCK_STREAM, 0);
  if (fd_cli == -1) { 
  	perror("Fallo al crear el socket\n") ; 
  	return -1 ; 
  }
   
  //señales de terminación del proceso
  //signal(SIGINT, desconectarProceso);
  
  /*Inicializar el número de puerto y direcciones*/
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = htons(7734);
  len = sizeof(address);
  
  /*Iniciar la conexión*/
  int result = connect(fd_cli, (struct sockaddr *)&address, len);
  if(result < 0){
	perror("Error de conexión");
	exit(-1);
  }
 
 //Inicializar semáforos
 /*int s_flag;
 s_flag = O_CREAT | O_EXCL;*/
  

  while(1){
        value = r4_nor ( &seed, kn, fn, wn );
        if (value < 0) value=value*(-1);
        sensor_reading= floor(value*Dsd+Davg);
      	delay=floor(1000000/(value*Tsd+Tfreq));
  	usleep(delay);
  	rc = write(fd_cli, &sensor_reading, 1);
  	read(fd_cli, &sensor_reading, 1);	
  	// DEBUG <- 0 TO TEST AWTU
  	if (DEBUG) printf ( "data to be send id-sensor %d, type %d, record %d, delay %d ms. \n",ID_sensor,Stype,sensor_reading,delay/1000);
  	// INSERTAR IPC PARA ENVIO DE MENSAJES AQUI!!
  	if(ID_sensor == 1){
  		break;
  	}
       
    }
    close(fd_cli);
    exit(0);
  
  return(EXIT_SUCCESS);

}
//Implementación de funciones
/*void desconectarProceso(){
	printf("Desconectando del servidor\n");
	close(fd_cli);
	//break;
}*/




