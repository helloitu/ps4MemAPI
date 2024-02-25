#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


const char *goodresp = "HTTP/1.1 200 OK.\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: 0\r\n\r\n";
const char *notfoundresp = "HTTP/1.1 404 Not Found.\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: 0\r\n\r\n";
const char *methodresp = "HTTP/1.1 405 Method Not Allowed.\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: 0\r\n\r\n";

struct paramdict {
    char **keys;
    char **values;
    int length;
    unsigned int size;
};

struct paramdict *paramdict_alloc() {
    struct paramdict *pd = (struct paramdict *)malloc(sizeof(struct paramdict));

    pd->size = 4096;
    pd->keys = (char **)malloc(pd->size);
    pd->values = (char **)malloc(pd->size);
    pd->length = 0;

    return pd;
}

int paramdict_add(struct paramdict *pd, char *key, char *value) {
    if(pd->length * sizeof(char *) >= pd->size) {
        pd->size += 4096;
        pd->keys = (char **)realloc(pd->keys, pd->size);
        pd->values = (char **)realloc(pd->values, pd->size);
    }

    pd->keys[pd->length] = key;
    pd->values[pd->length] = value;
    pd->length++;

    return 0;
}

char *paramdict_search(struct paramdict *pd, char *key) {
    int i;

    for(i = 0; i < pd->length; i++) {
        if(!strcmp(pd->keys[i], key)) {
            return pd->values[i];
        }
    }

    return NULL;
}

int paramdict_free(struct paramdict *pd) {
    free(pd->keys);
    free(pd->values);
    free(pd);

    return 0;
}

struct api_operation {
    char name[32];
    int (*handler)(int sock, struct paramdict *);
};

int handle_list(int sock, struct paramdict *params) {
    printf("address %s", paramdict_search(params, "address"));

    write(sock, goodresp, sizeof(goodresp));

    return 0;
}

int handle_unimpl(int sock, struct paramdict *params) {
    return 1;
}

struct api_operation operations[] = {
    { "list", handle_list },
    { "mapping", handle_unimpl },
    { "write", handle_unimpl },
    { "read", handle_unimpl },
    { "alloc", handle_unimpl },
    { "free", handle_unimpl },
    { "", 0 }
};

int handle_operation(int sock, char *operation, struct paramdict *params) {
    int i;

    for(i = 0; ; i++) {
        struct api_operation *oper = &operations[i];
        if(!oper->handler) {
            break;
        }

        if(!strcmp(oper->name, operation)) {
            return oper->handler(sock, params);
        }
    }

    return 1;
}

int handle_request(int sock) {
    unsigned char *buffer;
    int buffersize;
    int offset;
    int recvsize;
    int shouldcontinue;
    int i;

    buffersize = 4096;
    buffer = (unsigned char *)malloc(buffersize);

    shouldcontinue = 1;
    while(1) {
        recvsize = read(sock, buffer + offset, buffersize - offset);

        if(recvsize) {
            shouldcontinue = 1;

            // search for \r\n\r\n which tells us it is the end
            for(i = 0; i < buffersize; i++) {
                if(!strncmp(buffer + i, "\r\n\r\n", 4)) {
                    shouldcontinue = 0;
                    break;
                }
            }

            if(shouldcontinue) {
                buffersize += 4096;
                offset += recvsize;
                buffer = (unsigned char *)realloc(buffer, buffersize);
            } else {
                shouldcontinue = 1;
                break;
            }
        } else {
            shouldcontinue = 0;
            break;
        }

    }

    struct paramdict *pd = paramdict_alloc();

    if(shouldcontinue) {
        // we only handle GET requests with parameters inside the url
        if(strncmp(buffer, "GET", 3)) {
            write(sock, methodresp, sizeof(methodresp));
            goto finish;
        }

        // the handling of this input will destroy the buffer's structure

        // break first line
        *strstr(buffer, "\r\n") = 0;

        // break second space
        *strstr(strstr(buffer, " ") + 1, " ") = 0;

        char *path = buffer + 4 + 1; // + 1 skip past the 'GET /'

        char operation[32];
        memset(operation, 0, sizeof(operation));

        char *qmark = strstr(path, "?");
        if(qmark) {
            strncpy(operation, path, qmark - path);
            
            char *params = path + strlen(operation) + 1;
            char *p = strtok(params, "&");
            while(p != NULL) {
                char *equalsign = strstr(p, "=");
                *equalsign = 0;
                paramdict_add(pd, p, equalsign + 1);

                p = strtok(NULL, "&");
            }
        } else {
            strncpy(operation, path, sizeof(operation));
        }
        
        printf("request path: %s\n", path);

        if(handle_operation(sock, operation, pd)) {
            write(sock, notfoundresp, sizeof(notfoundresp));
            goto finish;
        }
    }

finish:
    free(buffer);
    paramdict_free(pd);
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        printf("could not create socket\n");
        return 1;
    }

    // forcefully attaching socket to the port 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        printf("failed to setsockopt\n");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(772);
    
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("failed to bind\n");
        return 1;
    }

    if(listen(server_fd, 3) < 0) {
        printf("failed to listen\n");
        return 1;
    }

    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        printf("failed to accept\n");
        return 1;
    }

    handle_request(new_socket);

    close(new_socket);
    close(server_fd);

    return 0;
} 