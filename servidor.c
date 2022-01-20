/*servidor con hijos, atiende 5 clientes*/

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> //write(), read(), close() equivalentes a fwrite/fread pero de sistema operativo

#include <arpa/inet.h>  //htons()
#include <sys/socket.h> //bind
#include <sys/types.h>  //bind
#include <signal.h>     //manejaSIGCHILD
#include <sys/wait.h>   //wait

#define PORT 5555
#define TAM_MENSAJE 1000
#define CANTIDAD_CLIENTES 5

int contclientes = 0;

void miSIGCHLD(int e)
{
    printf("Un hijo finalizo, cliente desconectado\n");
    waitpid(-1, NULL, 0);
    contclientes--;
}

int main(void)
{
    //para el servidor, para la coneccion con el cliente
    int sockServerFD;
    struct sockaddr_in server_addr;
    //para el cliente
    int sockClienteFD;
    struct sockaddr_in cliente_addr;
    int longitudCliente;

    int i = 0;
    char mensaje[] = "hola soy el hijo del servidor";
    char mensajeRecibido[1000];

    sockServerFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockServerFD == -1)
    {
        printf("Error en la funcion socket\n");
        exit(-1);
    }
    //cargo la estructura del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    //asocio puerto y red
    if (bind(sockServerFD, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        printf("Error en la funcion bind\n");
        exit(-1);
    }

    if (listen(sockServerFD, 2) == -1)
    {
        printf("Error en la funcion listen\n");
        exit(-1);
    }
    signal(SIGCHLD, miSIGCHLD);

    while (i < CANTIDAD_CLIENTES)
    {
        sockClienteFD = accept(sockServerFD, (struct sockaddr *)&cliente_addr, (socklen_t *)&longitudCliente);
        printf("%d\n", sockClienteFD);
        if (fork() == 0)
        {
            //creo hijo para atender al cliente
            write(sockClienteFD, mensaje, strlen(mensaje));
            printf("mensaje enviado:%s\n", mensaje);
            if (recv(sockClienteFD, mensajeRecibido, TAM_MENSAJE, 0) == -1)
            {
                printf("Error en la funcion recv\n");
                exit(-1);
            }
            printf("mensaje:%s\n", mensajeRecibido);
            sleep(4);
            close(sockClienteFD);
            exit(0);
        }
        else
        {
            i++;
            printf("Ocupado, atendiendo al cliente numero: %d\n", i);
        }
    }
    sleep(5);
    close(sockClienteFD);
    close(sockServerFD);
    return 0;
}