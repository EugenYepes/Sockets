//Cliente para conectarse al servidor

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> //write(),read() equivalentes a fwrite/fread pero de sistema operativo

#include <arpa/inet.h>  //htons()
#include <sys/socket.h> //bind
#include <sys/types.h>  //bind

#define PORT 5555
#define TAM_MENSAJE 1000

int main(int argc, char **argv)
{
    /*la posicion argv[1] contiene la ip al servidor 
    argv[2] contiene el mensaje para enviar al servidor*/
    int sockFD;
    struct sockaddr_in server_addr;
    char mensaje[1000] = "hola";
    int errores;
    if (argc == 2)
    {
        printf("Error en la cantidad de argumentos\n");
        printf("Recuerde ./ejecutable iP Mensaje\n");
        exit(-1);
    }

    sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD == -1)
    {
        printf("Error en la funcion sock\n");
        exit(-1);
    }

    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    errores = connect(sockFD, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (errores == -1)
    {
        printf("Connect:%d\n", errores);
        printf("Error en la coneccion\n");
        exit(-1);
    }

    if (recv(sockFD, mensaje, TAM_MENSAJE, 0) == -1)
    {
        printf("Error en la funcion recv\n");
        exit(-1);
    }
    printf("mensaje:%s\n", mensaje);

    write(sockFD, argv[2], strlen(argv[2]));
    close(sockFD);
    return 0;
}