#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/9Lq7BNBnBycd6nxy.socket"
#define BUFFER_SIZE 12

int main(int argc, char *argv[])
{
    struct sockaddr_un name;
    int i;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];
    /* Создание локального сокета. */
    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    /*
     * Для переносимости очищаем всю структуру, так как в некоторых
     * реализациях имеются дополнительные (нестандартные) поля.
     */
    memset(&name, 0, sizeof(struct sockaddr_un));
    /* Соединяем сокет с именем сокета. */
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
    ret = connect (data_socket, (const struct sockaddr *) &name,
                   sizeof(struct sockaddr_un));
    if (ret == -1) {
        fprintf(stderr, "Сервер выключен.\n");
        exit(EXIT_FAILURE);
    }
    /* Посылаем аргументы. */
    for (i = 1; i < argc; ++i) {
        ret = write(data_socket, argv[i], strlen(argv[i]) + 1);
        if (ret == -1) {
            perror("write");
            break;
        }
    }
    /* Отправка результата. */
    strcpy (buffer, "END");
    ret = write(data_socket, buffer, strlen(buffer) + 1);
    if (ret == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    /* Получение результата. */
    ret = read(data_socket, buffer, BUFFER_SIZE);
    if (ret == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    /* Проверяем, что буфер завершается 0. */
    buffer[BUFFER_SIZE - 1] = 0;
    printf("Result = %s\n", buffer);
    /* Закрытие сокета. */
    close(data_socket);
    exit(EXIT_SUCCESS);
}
