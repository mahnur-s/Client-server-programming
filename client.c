#include "ftp_test.h"

// NOTE: Please make sure that you fully understand the description before starting the coding. It will save your time!

int main(int argc, char *argv[]) {

    /*####################################################################################*/
    /*###################### DO NOT MODIFY THE CODE BELOW THIS LINE ######################*/
    int serv_sock; // server socket
    int clnt_sock; // client socket
    if (argc != 3 || strcmp(argv[1], "-p")) {
        printf("invalid command line\n");
        printf("./server -p <port>\n");
        exit(1);
    }
    /*###################### DO NOT MODIFY THE CODE ABOVE THIS LINE ######################*/
    /*####################################################################################*/

    // Initialize a server socket.
    // To ensure the server socket connect with client socket, 
    // you should initialize the server socket with the proper settings.
    // Note that you should use "INADDR_ANY" option when you setup the server address.
    /* Your Code */
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error\n");
        exit(1);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[2]));
    // Bind the server socket .
    /* Your Code */
    if (bind(serv_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("bind error\n");
        exit(1);
    }
    printf("Server start: listen on port %s\n", argv[2]);

    // Set the server socket to listening mode.
    /* Your Code */
    if (listen(serv_sock, 5) == -1) {
        printf("listen error\n");
        exit(1);
    }

    // Accept client sockets if they request to accept.
    /* Your Code */
    int client_sock;
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    client_sock = accept(serv_sock, (struct sockaddr *)&client_addr, &addr_size);
    if (client_sock < 0) {
        printf("accept error\n");
        exit(1);
    }

    // By now, we have initialized the server socket and made the socket accept the client sockets.
    // We will then implement the auxiliary loop to let the server socket deal with the requests from the clients.
    // Functions that you should implement are as follows: "Upload", "Download", and "Terminate".
    while (1) {

        char command[BUFSIZ];
        // Receive command from the client
        recv(client_sock, command, sizeof(command), 0);
        // Check if it is an "UPLOAD" command
        if (strcmp(command, "UPLOAD") == 0) {
            printf("file command: %s\n", command);
            char file_name[BUFSIZ];

            // Receive the file name from the client
            recv(client_sock, file_name, sizeof(file_name), 0);
            printf("file name: %s\n", file_name);
            char file_size_str[32];
            recv(client_sock, file_size_str, sizeof(file_size_str), 0);
            long file_size = atol(file_size_str);
            printf("file size: %ld\n", file_size);
            char file_path[BUFSIZ];
            sprintf(file_path, "server_files/%s", file_name);

            FILE *fp = fopen(file_path, "wb");
            if (fp == NULL) {
                printf("file creation error\n");
                send(client_sock, "False", sizeof("False"), 0);
                continue;
            } else {
                fseek(fp, file_size - 1, SEEK_SET);
                fputc('\0', fp);
                fseek(fp, 0, SEEK_SET);
                printf("file creation\n");
                send(client_sock, "True", sizeof("True"), 0);
                ssize_t bytesReceived;
                char buffer[BUFSIZ];
                long total = 0;

                do {
                    bytesReceived = recv(client_sock, buffer, sizeof(buffer), 0);
                    if (bytesReceived <= 0) {
                        printf("Error while reading file\n");
                        break;
                    }

                    fwrite(buffer, sizeof(char), bytesReceived, fp);
                    total += bytesReceived;
                } while (total < file_size);

                // Receive and write file contents

                fclose(fp);
                printf("upload complete: %s\n", file_name);
            }
        }

        // Download
        else if (strcmp(command, "DOWNLOAD") == 0) {

            int filelength;
            recv(client_sock, &filelength, sizeof(filelength), 0);
            printf("File Name Length: %d\n", filelength);
            // file_n[strcspn(file_n, "\n")] = '\0';

            char file2_name[256];
            recv(client_sock, file2_name, sizeof(file2_name), 0); // receiving the file name
            printf("filename complete: %s\n", file2_name);

            // size_t filename_length = strlen(file2_name);
            // printf("Length of filename: %zu\n", filename_length);


            char file_path[256];
            snprintf(file_path, sizeof(file_path), "server_files/%s", file2_name);
            printf("file_path_s: %s\n", file_path);
            FILE *fp2 = fopen(file_path, "rb");
            if (fp2 == NULL) {
                printf("File not found in server\n");
                send(client_sock, "False", sizeof("False"), 0);
                return False;
            } else {
                send(client_sock, "True", sizeof("True"), 0);
                fseek(fp2, 0, SEEK_END);
                long file2_size = ftell(fp2);
                fseek(fp2, 0, SEEK_SET);
                char file_2size_str[256];
                sprintf(file_2size_str, "%ld", file2_size);
                send(client_sock, file_2size_str, strlen(file_2size_str), 0); // sending file size
                char buffer[1024];
                ssize_t bytesRead;
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp2)) > 0) {
                    send(client_sock, buffer, bytesRead, 0); // sending contents
                }
                printf("Download completed: %s\n", file2_name);
                return True;
                // send(client_sock, "True", sizeof("True"), 0);
            }

        }

        // Terminate
        else if (strcmp(command, "TERMINATE") == 0) {
            close(client_sock);
            close(serv_sock);
            // return 0;
        }
    }

    // Close the server socket

}