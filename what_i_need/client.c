/*client.c*/

#include <errno.h>
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
    /*initialization*/
    struct sockaddr_un dist;
    int i;
    int res_work;
    int data_socket;
    char buffer[BUFFER_SIZE];
    char command[3];

    /*creation of local socket for unix*/
    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);

    /*check of creation*/
    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&dist, 0, sizeof(struct sockaddr_un));

    /*connect socket with socket name (SOCKET_NAME)*/
    dist.sun_family = AF_UNIX;
    strncpy(dist.sun_path, SOCKET_NAME, sizeof(dist.sun_path) - 1);
    res_work = connect (data_socket, (const struct sockaddr *) &dist,
                   sizeof(struct sockaddr_un));

    /*check of conecction*/
    if (res_work == -1) {
        fprintf(stderr, "Server off, try later.\n");
        exit(EXIT_FAILURE);
    }
    //for(;;){
      //printf("%s\n", "Enter command: ");
      //scanf("%s\n", command);

      /*sending arguments from comman line*/
       for (i = 1; i < argc; ++i) {
          res_work = write(data_socket, argv[i], strlen(argv[i]) + 1);
          if (res_work == -1) {
              perror("write");
              break;
          }

        }
      /*telling the end of massage to local server*/
      strcpy (buffer, "END");
      res_work = write(data_socket, buffer, strlen(buffer) + 1);
      if (res_work == -1) {
          perror("write");
          exit(EXIT_FAILURE);
      }

      /*getting of resault of working on local server*/
      res_work = read(data_socket, buffer, BUFFER_SIZE);
      if (res_work == -1) {
          perror("read");
          exit(EXIT_FAILURE);
      }
      buffer[BUFFER_SIZE - 1] = 0;
      printf("%s\n", buffer);

    //}

    /*cleaning and disconnecting sesion (closing sokets)*/
    close(data_socket);
    exit(EXIT_SUCCESS);

}
