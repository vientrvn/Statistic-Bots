// NOTE TO SELF:
// use strtok() to break a string into a series of tokens using a delimiter
// use sprintf() to send formatted output to a astring pointed to, by str
// use put(str) to write a line or string to the output(stdout) stream

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>

#define MAXLINE 8192 /* Max text line length */

int open_clientfd(char *hostname, int port)
{
  // The client's socket file descriptor.
  int clientfd;

  // The hostent struct is used to get the IP address of the server
  // using DNS.
  //
  // struct hostent {
  //   char *h_name;        // official domain name of host
  //   char **h_aliases;    // null-terminated array of domain names
  //   int  h_addrtype;     // host address type (AF_INET)
  //   int  h_length;       // length of an address, in bytes
  //   char **h_addr_list;  // null-terminated array of in_addr structs
  // };
  struct hostent *hp;

  // serveraddr is used to record the server information (IP address
  // and port number).
  //
  // struct sockaddr_in {
  //   short            sin_family;   // e.g. AF_INET
  //   unsigned short   sin_port;     // e.g. htons(3490)
  //   struct in_addr   sin_addr;     // see struct in_addr, below
  //   char             sin_zero[8];  // zero this if you want to
  // };
  struct sockaddr_in serveraddr;

  printf("Echo Client is creating a socket.\n");

  // First, we create the socket file descriptor with the given
  // protocol and protocol family.
  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return -1;

  // Query DNS for the host (server) information.
  if ((hp = gethostbyname(hostname)) == NULL)
    return -2;

  // The socket API requires that you zero out the bytes!
  bzero((char *)&serveraddr, sizeof(serveraddr));

  // Record the protocol family we are using to connect.
  serveraddr.sin_family = AF_INET;

  // Copy the IP address provided by DNS to our server address
  // structure.
  bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr,
        hp->h_length);

  // Convert the port from host byte order to network byte order and
  // store this in the server address structure.
  serveraddr.sin_port = htons(port);

  printf("Echo Client is trying to connect to %s (%s:%d).\n", hostname,
         inet_ntoa(serveraddr.sin_addr), port);

  // Establish a connection with the server.
  if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    return -1;

  printf("Echo Client connected.\n");

  // Return the connected file descriptor.
  return clientfd;
}

void sortArray(int nums[], int n)
{
  int i, key, j;
  for (i = 1; i < n; i++)
  {
    key = nums[i];
    j = i - 1;

    /* Move elements of arr[0..i-1], that are
      greater than key, to one position ahead
      of their current position */
    while (j >= 0 && nums[j] > key)
    {
      nums[j + 1] = nums[j];
      j = j - 1;
    }
    nums[j + 1] = key;
  }
}

int main(int argc, char **argv)
{
  // The client socket file descriptor.
  int clientfd;

  // The port number.
  int port;

  // Variable to store the host/server domain name.
  char *host;

  // A buffer to receive data from the server.
  char buf[MAXLINE];

  // NetID for the program arguments
  char *netID;

  // First, check the program arguments:
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s <NetID> <Port> <IP Address>\n", argv[0]);
    exit(0);
  }

  // First argument is netID, second is port, and third is host:
  netID = argv[1];
  port = atoi(argv[2]);
  host = argv[3];

  // Open the client socket file descriptor given the host and port:
  clientfd = open_clientfd(host, port);

  char selfID[MAXLINE];
  strcpy(selfID, "cs230 HELLO ");
  strcat(selfID, netID);
  strcat(selfID, "\n");

  // send() function sends data on the socket with descriptor socket
  // ssize_t send(int socket, char *buffer, int length, int flags);
  send(clientfd, selfID, strlen(selfID), 0);

  // An infinite loop which will run till a break statement is issued explicitly
  while (1)
  {
    // recv() function receives data on a socket with descriptor socket and stores it in a buffer
    recv(clientfd, buf, MAXLINE, 0);

    // Parsing the buff
    int nums[5];

    char *parsed_buf = strtok(buf, " ");
    char arr_buf[8][MAXLINE];

    int i = 0;
    while (parsed_buf != NULL)
    {
      strcpy(arr_buf[i], parsed_buf);
      i++;
      parsed_buf = strtok(NULL, " ");
    }

    if (strcmp(arr_buf[2], "BYE\n") == 0)
      break;

    for (int i = 3; i < 8; i++)
    {
      nums[i - 3] = atoi(arr_buf[i]);
    }

    // Sort the numbers by using insertion sort
    sortArray(nums, 5);

    // Perform the operation
    if (!strcmp("max", arr_buf[2]))
    {
      char result[MAXLINE];
      sprintf(result, "cs230 %d\n", nums[4]);
      send(clientfd, result, strlen(result), 0);
    }
    else if (!strcmp("min", arr_buf[2]))
    {
      char result[MAXLINE];
      sprintf(result, "cs230 %d\n", nums[0]);
      send(clientfd, result, strlen(result), 0);
    }
    else if (!strcmp("median", arr_buf[2]))
    {
      char result[MAXLINE];
      sprintf(result, "cs230 %d\n", nums[2]);
      send(clientfd, result, strlen(result), 0);
    }
  }

  // Close the file descriptor:
  close(clientfd);
  exit(0);
}
