#include <stdarg.h>
 #include <stdio.h>
 #include <errno.h>
 #include <string.h>
 #include <winsock2.h>
 #define bzero(b,len) (memset((b), '\0', (len)), (void) 0)



 int main() {

     // Define some parameters
     WSADATA wsaData;
     int sockfd, bytes_read, iResult, j;
     struct hostent *remoteHost;
     char *host_name = "api.steampowered.com";
     struct sockaddr_in dest;
     char buffer[2000];
     char hdr[1000];

     // Create Server Client Strings
     bzero(hdr, sizeof(hdr));
     strcpy(hdr, "GET http://api.steampowered.com/ISteamUserStats/GetPlayerAchievements/v1/?key=X&steamid=X&appid=473690\n");
     //strcat(hdr, "api.steampowered.com\r\n");

     // Clean things up a bit before starting
     printf("\n\n");

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
    return 1;
    }

     // Build the address
     remoteHost = gethostbyname(host_name);
     if (remoteHost == NULL) {
        printf("error: gethostbyname(\"%s\")\n", host_name);
        return 0;
    }

    // Create Socket
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
          printf("Socket not created\n");
          return 0;
    }

    // Initialize server address/port struct
    bzero(&dest, sizeof(dest));
     dest.sin_family = AF_INET;
    dest.sin_port = htons(80);

    // *** Added this line to fix the code ***
    dest.sin_addr.s_addr = inet_addr("23.63.165.179");

    dest.sin_family = AF_INET;
    dest.sin_port = htons(80);
    if ( inet_addr("23.63.165.179") == 0 ) {
         printf("Incorrect Address Expression\n");
         return 0;
    }

    // Connect Socket
    if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(struct sockaddr_in)) != 0 ) {
         printf("Socket Connection Failed %s, %d\n", strerror(errno), WSAGetLastError());
         close(sockfd);
         WSACleanup();
         return 0;
    }
    // Send data
    if (send(sockfd, hdr, strlen(hdr), 0) < 0) {
         printf("Send Data Failed, %d\n", WSAGetLastError());
         return 0;
    }

    printf("Socket successfully sent\n");
    printf("\nSend Message - TxBufferSize = %d\n\n",strlen(hdr));
    printf("%s", hdr);

    bzero(buffer, sizeof(buffer));
    bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);

    if (bytes_read < 0) {
         printf("Read Data Failed %s, %d\n", strerror(errno), WSAGetLastError());
    }

    if (bytes_read > 0) {
        // Print out receive buffer
        printf("\n\nReceived Message -- RxSize = %d \n\n", strlen(buffer));
        printf("%s", buffer);
    }

    if (bytes_read == 0) {
        printf("No Read Bytes Received\n");
    }

    /*---Clean up---*/
    close(sockfd);
    WSACleanup();
    return 0;
}
