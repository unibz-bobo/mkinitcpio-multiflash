/****************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Julian Sanin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

int bind_socket(const char *dgram_port);

char *decode_address(
        int fd_socket, const char *dgram_message, long timeout_sec);

const char *USAGE =
        "USAGE: %s <dgram_message> <dgram_port> <dgram_timeout_sec>\n";

int main(int argc, char *argv[]) {
    char *dgram_message = NULL;
    char *dgram_port = NULL;
    char *dgram_timeout_sec = NULL;
    char *ip_address = NULL;
    int fd_socket = 0;

    // Check arguments
    if(argc != 4) {
        printf(USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }
    dgram_message = argv[1];
    dgram_port = argv[2];
    dgram_timeout_sec = argv[3];

    // Assign UDP socket
    fd_socket = bind_socket(dgram_port);

    // Match message and print source IP address
    ip_address = decode_address(
            fd_socket, dgram_message, strtol(dgram_timeout_sec, NULL, 10));
    printf("%s\n", ip_address);
    free(ip_address);
    ip_address = NULL;
    close(fd_socket);

    return EXIT_SUCCESS;
}

int bind_socket(const char *dgram_port) {
    struct addrinfo ai_hints;
    struct addrinfo *ai_results = 0, *ai_item = 0;
    int fd_socket = 0, error = 0;

    memset(&ai_hints, 0, sizeof(struct addrinfo));
    ai_hints.ai_family = AF_UNSPEC;                 // Allow IPv4 or IPv6
    ai_hints.ai_socktype = SOCK_DGRAM;              // Datagram socket
    ai_hints.ai_protocol = 0;                       // Any protocol
    ai_hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; // Wildcard IPv4/6

    // hostname = NULL, wildcard IP 0.0.0.0 or 0:0:0:0:0:0:0:0
    error = getaddrinfo(NULL, dgram_port, &ai_hints, &ai_results);
    if(error != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }

    // getaddrinfo() returns a list of address structures.
    // Try each address until we successfully bind(2).
    // If socket(2) (or bind(2)) fails, we (close the socket
    // and) try the next address.
    for(ai_item = ai_results; ai_item != NULL; ai_item = ai_item->ai_next) {
        fd_socket = socket(
                ai_item->ai_family,
                ai_item->ai_socktype,
                ai_item->ai_protocol);
        if(fd_socket == -1) {
            continue;
        }
        if(bind(fd_socket, ai_item->ai_addr, ai_item->ai_addrlen) == 0) {
            break; // Success
        }
        close(fd_socket);
    }
    if(ai_item == NULL) {               // No address succeeded
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(ai_results);           // No longer needed

    return fd_socket;
}

char *decode_address(
        int fd_socket, const char *dgram_message, long timeout_sec) {
    char buf_msg[1024];
    char *buf_src = NULL;
    struct sockaddr_storage src_addr;
    socklen_t src_addr_len = sizeof(src_addr);
    ssize_t count = 0;
    int error = 0;
    struct timeval tv_timeout;

    buf_src = malloc(INET6_ADDRSTRLEN);
    memset(buf_src, 0, INET6_ADDRSTRLEN);
    memset(buf_msg, 0, sizeof(buf_msg));

    // Set timeout
    tv_timeout.tv_sec = timeout_sec;
    tv_timeout.tv_usec = 0;
    error = setsockopt(
            fd_socket, SOL_SOCKET, SO_RCVTIMEO,
            &tv_timeout, sizeof(tv_timeout));
    if(error != 0) {
        fprintf(stderr, "setsockopt: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Receive message and compare
    count = recvfrom(
            fd_socket, buf_msg, sizeof(buf_msg), 0,
            (struct sockaddr *)&src_addr, &src_addr_len);
    if(count < 0) {
        fprintf(stderr, "recvfrom: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else if(count == sizeof(buf_msg)) {
        fprintf(stderr, "Datagram too large for message buffer: truncated\n");
        exit(EXIT_FAILURE);
    } else if(strcmp(dgram_message, strtok(buf_msg, "\n\t\\:;, ")) == 0){
        // Get IP address
        error = getnameinfo(
                (struct sockaddr *)&src_addr, src_addr_len,
                buf_src, INET6_ADDRSTRLEN,
                0,0, NI_NUMERICHOST);
        if(error != 0) {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(error));
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "decode_address: Could not match '%s'\n", buf_msg);
        exit(EXIT_FAILURE);
    }
    return buf_src;
}
