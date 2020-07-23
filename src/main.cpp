/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** main.cpp
*/

#include "Reception.hpp"

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOG 10
#define PORT 0x1001
#define MAXMSG 512

char get_byte(uint8_t byte, unsigned int pos)
{
    unsigned int idx = 7;
    int mask = 0x80; // 10000000
    while (mask > 0) {
        if (idx == pos) {
            return ((byte & mask) > 0) ? 1 : 0;
        }
        mask >>= 1;
        idx--;
    }
    return 0;
}

int fetch_around()
{
    inquiry_info* ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev(dev_id);
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)calloc(max_rsp, sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if (num_rsp < 0)
        perror("hci_inquiry");

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii + i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii + i)->bdaddr, sizeof(name), name, 0) < 0)
            strcpy(name, "[unknown]");
        uint32_t binary_cod = (ii[i].dev_class[2]) << 16;
        binary_cod |= (ii[i].dev_class[1]) << 8;
        binary_cod |= ii[i].dev_class[0];
        ClassOfDevice cod { binary_cod };
        printf("%s  %s  %02X%02X%02X  %s\n", addr, name, ii[i].dev_class[2], ii[i].dev_class[1], ii[i].dev_class[0], cod.majorDevClassStr.c_str());
        for (auto str : cod.serviceClassStr)
            std::cout << str << ", ";
        std::cout << "> " << cod.majorDevClassStr << " > " << cod.minorDevClassStr << "\n";
    }

    free(ii);
    close(sock);
    return 0;
}

void client()
{
    struct sockaddr_l2 addr;
    int sock, status;
    //char dest[18] = "A0:C5:89:F6:B1:33"; // SWIFT
    //char dest[18] = "40:EF:4C:1B:0C:A5"; // EDIFIER
    char dest[18] = "74:29:AF:80:20:80"; // ASPIRE

    // allocate a socket
    sock = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    if (sock < 0) {
        perror("socket");
        return;
    }

    // set the connection parameters (who to connect to)
    memset(&addr, 0, sizeof(addr));
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = htobs(PORT);
    str2ba(dest, &addr.l2_bdaddr);

    // connect to server
    status = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (status < 0) {
        perror("connect");
        close(sock);
        return;
    }
    // send a message
    status = write(sock, "hello!", 6);
    if (status < 0) {
        perror("write");
        close(sock);
        return;
    }
    printf("Send\n");
    close(sock);
}

int create_server(unsigned short port)
{
    struct sockaddr_l2 loc_addr;
    int sock;
    int reuse_addr = 1;

    // allocate socket
    sock = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    // bind socket to port of the first available
    // bluetooth adapter
    memset(&loc_addr, 0, sizeof(loc_addr));
    loc_addr.l2_family = AF_BLUETOOTH;
    // same as : loc_addr.l2_bdaddr = *BDADDR_ANY;
    // but it's illegal to take the address of an rvalue temporary in cpp
    memset(&(loc_addr.l2_bdaddr), 0, sizeof(loc_addr.l2_bdaddr));
    loc_addr.l2_bdaddr_type = BDADDR_BREDR;
    loc_addr.l2_psm = htobs(port);
    // security level  ??? loc_addr.l2_cid = htobs(CID_ATT);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) < 0) {
        perror("setsockopt");
        return -1;
    }

    if (bind(sock, (struct sockaddr*)&loc_addr, sizeof(loc_addr)) < 0) {
        perror("bind");
        return -1;
    }

    // put socket into listening mode
    if (listen(sock, BACKLOG) < 0) {
        perror("listen");
        return -1;
    }
    return sock;
}

int read_from_client(int filedes)
{
    char buffer[MAXMSG];
    int nbytes;

    nbytes = read(filedes, buffer, MAXMSG);
    if (nbytes < 0) {
        /* Read error. */
        perror("read");
        exit(EXIT_FAILURE);
    } else if (nbytes == 0)
        /* End-of-file. */
        return -1;
    else {
        /* Data read. */
        fprintf(stderr, "Server: got message: `%s'\n", buffer);
        return 0;
    }
}

int main(int argc, char const**)
{
    if (argc == 2) {
        client();
        exit(EXIT_SUCCESS);
    }
    if (argc == 3) {
        fetch_around();
        exit(EXIT_SUCCESS);
    }
    int sock;
    fd_set active_fd_set, read_fd_set;

    // Create the socket and set it up to accept connections.
    sock = create_server(PORT);
    if (sock < 0) {
        dprintf(STDERR_FILENO, "Fail to open server\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the set of active sockets.
    FD_ZERO(&active_fd_set);
    FD_SET(sock, &active_fd_set);

    while (1) {
        printf("hey\n");
        // Block until input arrives on one or more active sockets.
        read_fd_set = active_fd_set;
        if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Service all the sockets with input pending.
        if (FD_ISSET(sock, &read_fd_set)) {
            /* Connection request on original socket. */
            char buf[20] = { 0 };
            struct sockaddr_l2 client_addr;
            socklen_t size = sizeof(client_addr);
            int new_fd = accept(sock, (struct sockaddr*)&client_addr, &size);
            if (new_fd < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            ba2str(&client_addr.l2_bdaddr, buf);
            fprintf(stderr, "Server: accepted connection from %s\n", buf);
            FD_SET(new_fd, &active_fd_set);
        }
        for (int i = 0; i < FD_SETSIZE /* CLIENT */; ++i) {
            if (FD_ISSET(i, &read_fd_set) && sock != i) {
                // Data arriving on an already-connected socket.
                if (read_from_client(i) < 0) {
                    close(i);
                    FD_CLR(i, &active_fd_set);
                }
            }
        }
    }
    // close connection
    close(sock);
    exit(EXIT_SUCCESS);
}