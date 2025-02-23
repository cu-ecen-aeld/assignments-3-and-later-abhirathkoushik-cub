/*******************************************************************************
 * Copyright (C) 2025 by Abhirath Koushik
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Abhirath Koushik and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/

/**
 * @file    aesdsocket.c
 * @brief   This file is used to create a socket program to interact between the server and client.
 *
 *
 * @author  Abhirath Koushik
 * @date    02-21-2025
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <syslog.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT "9000"
#define BUFFER_SIZE 1024
#define FILE_PATH "/var/tmp/aesdsocketdata"

int server_fd=-1, client_fd=-1;

/*
 * This function is used to handle Signals to terminate the program and handle cleaup tasks.
 *
 * Parameters:
 *   signo: The signal number that triggered this handler
 *
 * Returns:
 *   None
 */
void signal_handler(int signo)
{
    syslog(LOG_INFO, "Caught signal, exiting");

    // Close open sockets
    if (client_fd != -1) {
        close(client_fd);
    }
    if (server_fd != -1) {
        close(server_fd);
    }

    // Remove the temporary file created
    remove(FILE_PATH);

    // Close syslog
    closelog();

    exit(0);
}

/*
 * This function is used to daemonize a given process and redirect the program outputs to detach from terminal
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None
 */
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        syslog(LOG_ERR, "Fork failed");
        exit(1);
    }

    if (pid > 0) {
        exit(0); // Parent exits
    }

    // Create a new session and detach from the controlling terminal
    if (setsid() < 0) {
        syslog(LOG_ERR, "Failed to create a new session");
        exit(1);
    }

    if (chdir("/") < 0) {
        syslog(LOG_ERR, "Failed to change working directory to /");
        exit(1);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Redirect stdin, stdout, and stderr to /dev/null to avoid interaction with terminal
    open("/dev/null", O_RDONLY);  // stdin
    open("/dev/null", O_WRONLY);  // stdout
    open("/dev/null", O_WRONLY);  // stderr
}

/*
 * This is the base function used for initiating the program execution.
 *
 * Parameters:
 *   argc: The number of command-line arguments passed to the program.
 *   argv: The array of command-line argument strings.
 *
 * Returns:
 *   On Success: 0
 *   On Failure: -1
 */
int main(int argc, char *argv[]) 
{
    struct addrinfo hints, *servinfo;
    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    int status;

    // Set up signal handling
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Open syslog
    openlog("aesdsocket", LOG_PID | LOG_PERROR, LOG_USER);

    // Parse arguments to check for -d option
    int daemon_mode = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            daemon_mode = 1;
        }
    }

    // Configure hints structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // Setting as IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP Stream Socket
    hints.ai_flags = AI_PASSIVE;     // Use wildcard address

    // Get address info for the specified port
     if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        syslog(LOG_ERR, "getaddrinfo failed");
        return -1;
    }
    
    // Create socket
    server_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (server_fd == -1) {
        syslog(LOG_ERR, "Failed to create socket");
        freeaddrinfo(servinfo);
        return -1;
    }

    // Set socket option to allow reuse of address and port
    int optval = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        syslog(LOG_ERR, "setsockopt failed");
        close(server_fd);
        freeaddrinfo(servinfo);
        return -1;
    }
    
    // Bind the socket to the address and port
    if (bind(server_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        syslog(LOG_ERR, "Bind failed");
        close(server_fd);
        freeaddrinfo(servinfo);
        return -1;
    }
    
    // Issue freeaddrinfo after bind step
    freeaddrinfo(servinfo);

    // Checking for Daemon after Binding if -d option was specified
    if (daemon_mode) {
        daemonize();
    }
    
    // Listen for connections
    if (listen(server_fd, 10) == -1) {
        syslog(LOG_ERR, "Listen failed");
        close(server_fd);
        return -1;
    }
    
    // Main server loop to handle incoming connections
    while (1) {
        // Accept connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == -1) {
            syslog(LOG_ERR, "Accept failed");
            continue;
        }

        struct sockaddr_in *client_in = (struct sockaddr_in *)&client_addr;
        syslog(LOG_INFO, "Accepted connection from %s", inet_ntoa(client_in->sin_addr));

        // Open file for appending
        int file_fd = open(FILE_PATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (file_fd == -1) {
            syslog(LOG_ERR, "Failed to open file");
            close(client_fd);
            continue;
        }
        
        // Receive data and write to file
        while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
            if (write(file_fd, buffer, bytes_received) == -1) {
                syslog(LOG_ERR, "Failed to write to file");
                break;
            }

            // Check for end of packet based on '\n' as the delimiter
            if (buffer[bytes_received - 1] == '\n') {
                break;
            }
        }
        
        // Close file after writing
        close(file_fd);

        // Reopen file for Reading Content
        file_fd = open(FILE_PATH, O_RDONLY);
        if (file_fd == -1) {
            syslog(LOG_ERR, "Failed to open file for reading");
            close(client_fd);
            continue;
        }

        // Read the file and send its content to the client
        while ((bytes_received = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
            send(client_fd, buffer, bytes_received, 0);
        }

        // Close file after reading
        close(file_fd);

        syslog(LOG_INFO, "Closed connection from %s", inet_ntoa(client_in->sin_addr));

        // Close Client Connection
        close(client_fd);
    }
    
    // Cleanup
    closelog();
    close(server_fd);
    return 0;
}
