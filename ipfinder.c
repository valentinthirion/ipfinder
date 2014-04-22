//
//  main.c
//  IPFinder
//
//  Created by Valentin Thirion on 11/04/14.
//  Copyright (c) 2014 The AppLab. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/socket.h>
#include<errno.h>
#include<netdb.h>
#include<arpa/inet.h>

char* getfield(char* line, int num)
{
    char* tok;
    for (tok = strtok(line, ",");
         tok && *tok;
         tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    
    if ((he = gethostbyname(hostname)) == NULL)
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }
    
    addr_list = (struct in_addr **) he->h_addr_list;
    
    for(i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return 0;
    }
    
    return 1;
}

int main(int argc, const char * argv[])
{
    if (argc != 4)
    {
        printf("Usage : IPFinder <input_file> <output_file> <#of dest>\n");
        return 0;
    }

    const char * input_filename = argv[1];
    const char * output_filename = argv[2];
    int nb;
    sscanf(argv[3],"%d",&nb);
    
    printf("Processing file: %s with %d addresses\n", input_filename, nb);

    // Open files
    FILE* input_file = fopen(input_filename, "r");
    FILE* output_file = fopen(output_filename, "w");

    if ((input_file == NULL) || (output_file == NULL))
    {
        printf("Error of files\n");
        return 0;
    }

    char line[100];
    int lineNumber = 1;
    while ((fgets(line, 50, input_file)) && (lineNumber < nb + 1))
    {
        char * host = getfield(strdup(line), 2);
        printf("line %d : %s\n", lineNumber, host);

        // Get IP Address
        char address[100];
        hostname_to_ip(host, address);

        // Write to the output file
        fprintf(output_file, "%s,%s\n", host, address);

        lineNumber++;
    }

    fclose(input_file);
    fclose(output_file);
    
    return 0;
}

