#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <sys/ioctl.h>
//#include <net/if.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MES_BEAT 0
#define MES_RESP 1
#define MES_QUIT 2

#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_TRA 4

const char* greeting     = "%d-- TCP Matrix Calculator --\n";
const char* operationmes = "%dChoose the operation (add, sub, mul, tra)\n> ";
const char* sizemes      = "%dChoose %s operand size (rows cols)\n> ";
const char* operandmes   = "%dChoose %s operand (%d x %d)\n> ";
const char* resultmes    = "%dResult:\n";
const char* repeatmes    = "%dDo you want to calculate again? (y/n)\n> ";
const char* quitmes      = "%dBye bye!\n";

void add(const double* x, const double* y, const int row, const int col,
         double** z, int* zrow, int* zcol) {
  if (*z) free(*z);

  *z = (double*) malloc(sizeof(double) * row * col);
  int i;
  for (i = 0; i < row * col; ++i) {
    (*z)[i] = x[i] + y[i];
  }

  *zrow = row;
  *zcol = col;
}

void sub(const double* x, const double* y, const int row, const int col,
         double** z, int* zrow, int* zcol) {
  if (*z) free(*z);

  *z = (double*) malloc(sizeof(double) * row * col);
  int i;
  for (i = 0; i < row * col; ++i) {
    (*z)[i] = x[i] - y[i];
  }

  *zrow = row;
  *zcol = col;
}

void mul(const double* x, const double* y, const int row, const int col,
         double** z, int* zrow, int* zcol) {
  if (*z) free(*z);

  *z = (double*) malloc(sizeof(double) * row * row);
  int i, j, k;
  for (i = 0; i < row; ++i) {
    for (j = 0; j < row; ++j) {
      int s = 0;
      for (k = 0; k < col; ++k)
        s += x[i * col + k] * y[k * row + j];
      (*z)[i * row + j] = s;
    }
  }

  *zrow = row;
  *zcol = row;
}

void tra(const double* x, const int row, const int col,
         double** z, int* zrow, int* zcol) {
  if (*z) free(*z);

  *z = (double*) malloc(sizeof(double) * row * col);
  int i, j;
  for (i = 0; i < row; ++i)
    for (j = 0; j < col; ++j)
      (*z)[j * row + i] = x[i * col + j];

  *zrow = col;
  *zcol = row;
}

int main(int argc, char** argv) {
  // temps
  int i, j;
  char buffer[1024];
  char cpbf[1024];

  unsigned short port = 11235;

  // Socket
  int serverfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverfd <= 0) {
    perror("Failed to create socket");
    return 1;
  }

  struct sockaddr_in addr;
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port        = htons(port);

  // Socket bind
  if (bind(serverfd, (const struct sockaddr*) &addr, sizeof(addr)) < 0) {
    perror("Failed to bind socket");
    return 1;
  }

  // Listen to connections
  printf("Waiting for client...\n");
  listen(serverfd, 1);

  struct sockaddr_in connaddr;
  socklen_t connlen = sizeof(connaddr);
  int sockfd = accept(serverfd, (struct sockaddr*) &connaddr, &connlen);
  if (sockfd < 0) {
    perror("Failed to accept connection");
    return 1;
  }
  printf("Client connected!\n");

  // Send greeting
  printf("Sending greeting...");
  sprintf(buffer, greeting, MES_BEAT);
  write(sockfd, buffer, strlen(buffer)+1);
  printf("OK!\n");

  // Receive greeting handshak
  printf("Waiting response...");
  if (read(sockfd, buffer, 1024) <= 0) {
    perror("Failed to read from connected socket");
    return 1;
  }
  printf("received: %s", buffer);

  int operation = -1;
  int row = 0, col = 0;
  double* x = 0;
  double* y = 0;
  double* z = 0;

  int quit= 0;
  while (!quit) {
    // Receive operation
    do {
      printf("Sending operation message...");
      sprintf(buffer, operationmes, MES_RESP);
      write(sockfd, buffer, strlen(buffer)+1);
      printf("OK!\n");

      printf("Waiting response...");
      if (read(sockfd, buffer, 1024) <= 0) {
        perror("Failed to read from connected socket");
        return 1;
      }
      printf("received: %s", buffer);

      operation = -1;
      if (strstr(buffer, "add")) operation = OP_ADD;
      if (strstr(buffer, "sub")) operation = OP_SUB;
      if (strstr(buffer, "mul")) operation = OP_MUL;
      if (strstr(buffer, "tra")) operation = OP_TRA;
    } while (operation < 0);

    // Receive operand size
    do {
      printf("Sending operand size...");
      sprintf(buffer, sizemes, MES_RESP, "first");
      write(sockfd, buffer, strlen(buffer)+1);
      printf("OK!\n");

      printf("Waiting response...");
      if (read(sockfd, buffer, 1024) <= 0) {
        perror("Failed to read from connected socket");
        return 1;
      }
      printf("received: %s", buffer);
    } while (sscanf(buffer, "%d %d", &row, &col) < 2);

    // Receive first operand
    x = (double*) malloc(sizeof(double*) * row * col);
    printf("Sending operand message...");
    sprintf(buffer, operandmes, MES_RESP, "first", row, col);
    write(sockfd, buffer, strlen(buffer)+1);
    printf("OK!\n");

    printf("Waiting first operand...\n");
    for (i = 0; i < row * col;) {
      if (i > 0) {
        sprintf(buffer, "%dRemaining %d more number(s)\n> ", MES_RESP, row * col - i);
        write(sockfd, buffer, strlen(buffer)+1);
      }
      if (read(sockfd, buffer, 1024) <= 0) {
        perror("Failed to read from connected socket");
        return 1;
      }

      printf("received: %s", buffer);
      char* p = buffer;
      for (; i < row * col && p[0] != '\0'; ++i) {
        x[i] = strtod(p, &p);
        if (x[i] == HUGE_VAL || x[i] == -HUGE_VAL)
          break;
        while (p[0] == '\n' || p[0] == ' ') p++;
      }
    }

    if (operation != OP_TRA) {
      // Receive second operand
      y = (double*) malloc(sizeof(double*) * row * col);

      printf("Sending operand message...");
      int t = (operation == OP_ADD || operation == OP_SUB);
      sprintf(buffer, operandmes, MES_RESP, "second", t? row : col, t? col : row);
      write(sockfd, buffer, strlen(buffer)+1);
      printf("OK!\n");

      printf("Waiting second operand...\n");
      for (i = 0; i < row * col;) {
        if (i > 0) {
          sprintf(buffer, "%dRemaining %d more number(s)\n> ", MES_RESP, row * col - i);
          write(sockfd, buffer, strlen(buffer)+1);
        }
        if (read(sockfd, buffer, 1024) <= 0) {
          perror("Failed to read from connected socket");
          return 1;
        }

        printf("received: %s", buffer);
        char* p = buffer;
        for (; i < row * col && p[0] != '\0'; ++i) {
          y[i] = strtod(p, &p);
          while (p[0] == '\n' || p[0] == ' ') p++;
        }
      }

      // Apply operation
      if (operation == OP_ADD) add(x, y, row, col, &z, &row, &col);
      if (operation == OP_SUB) sub(x, y, row, col, &z, &row, &col);
      if (operation == OP_MUL) mul(x, y, row, col, &z, &row, &col);
    } else {
      tra(x, row, col, &z, &row, &col);
    }

    // Send result
    printf("Sending result message...");
    sprintf(buffer, resultmes, MES_BEAT);

    char* p = buffer + strlen(buffer)+1;
    for (i = 0; i < row; ++i) {
      for (j = 0; j < col; ++j) {
        p += sprintf(p, "%f ", z[i * col + j]);
      }
      p += sprintf(p, "\n");
    }
    p += sprintf(p, "\n");

    write(sockfd, buffer, strlen(buffer)+1);
    printf("OK!\n");

    printf("Waiting response...");
    if (read(sockfd, buffer, 1024) <= 0) {
      perror("Failed to read from connected socket");
      return 1;
    }
    printf("received: %s", buffer);

    do {
      printf("Sending repeat message...");
      sprintf(buffer, repeatmes, MES_RESP);
      write(sockfd, buffer, strlen(buffer)+1);
      printf("OK!\n");

      if (read(sockfd, buffer, 1024) <= 0) {
        perror("Failed to read from connected socket");
        return 1;
      }
      printf("received: %s", buffer);
    } while (buffer[0] != 'y' && buffer[0] != 'n');

    if (buffer[0] == 'n') {
      printf("Sending quit message...");
      sprintf(buffer, quitmes, MES_QUIT);
      write(sockfd, buffer, strlen(buffer)+1);
      printf("OK!\n");

      if (read(sockfd, buffer, 1024) <= 0) {
        perror("Failed to read from connected socket");
        return 1;
      }

      quit = 1;
    }
  }

  // Free memory
  if (x) free(x);
  if (y) free(y);
  if (z) free(z);

  close(sockfd);
  close(serverfd);

  return 0;
}
