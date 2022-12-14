/*server.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/ransomware1337.socket"
#define BUFFER_SIZE 50


int main(int argc, char *argv[])
{
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    char result[100];
    char buffer[BUFFER_SIZE];
    char str[BUFFER_SIZE];

    unlink(SOCKET_NAME);

    /*creation of local socket for unix*/
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&name, 0, sizeof(struct sockaddr_un));

    /*connect socket with socket name (SOCKET_NAME)*/
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
    ret = bind(connection_socket, (const struct sockaddr *) &name,
               sizeof(struct sockaddr_un));
    /*check of conecction*/
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /*preparing for getting requests.
    Thera are 20 of them here and they are in one process*/
    ret = listen(connection_socket, 20);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /*the main work*/
    for (;;) {
        /*waiting for incoming socket disconnection*/
        data_socket = accept(connection_socket, NULL, NULL);
        if (ret == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        //result = 0;
        for(;;) {
            /*waiting of new socket request*/
            ret = read(data_socket, buffer, BUFFER_SIZE);
            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            buffer[BUFFER_SIZE - 1] = 0;

            /*services of algo*/
            if (!strncmp(buffer, "DOWN", BUFFER_SIZE)) {
                down_flag = 1;
                break;
            }
            if (!strncmp(buffer, "END", BUFFER_SIZE)) {
                break;
            }

            if(!strncmp(buffer, "LifeOfFile", BUFFER_SIZE)){
              system("./bcctools/lifeOfFile");
              strcpy (str, "Lifefile ended");
            }

            if(!strncmp(buffer, "Bash", strlen("Bash"))){
              system("./bcctools/bashtrace");
              strcpy (str, "Bash ended");
            }

            if(!strncmp(buffer, "CPUCheck", BUFFER_SIZE)){
              system("./btrace/cpugist.bt");
              strcpy (str, "CPUCheck ended");
            }

            if(!strncmp(buffer, "TCPConnect", BUFFER_SIZE)){
              system("./btrace/tcpconnect.bt");
              strcpy (str, "TCPConnect ended");
            }

        }
        /*sending the answer to client*/
        sprintf(buffer, "%s", str);
        ret = write(data_socket, buffer, BUFFER_SIZE);
        if (ret == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        /*closing sockets*/
        close(data_socket);

        if (down_flag) {
            break;
        }
    }
    
    /*cleaning and disconnecting sesion (closing sokets)*/
    close(connection_socket);
    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}
