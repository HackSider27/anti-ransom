#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
 FILE *read_fp;
 char buffer[BUFSIZ +1];
 int chars_read;
 memset(buffer, '\0', sizeof(buffer));
 read_fp = popen("tcpconnect.bt", "r");
 if (read_fp != NULL) {
  chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
  if (chars_read > 0) {
   printf("Output was:-\n%s\n", buffer);
  }
  pclose(read_fp);
  exit(EXIT_SUCCESS);
 }
 exit(EXIT_FAILURE);
}
