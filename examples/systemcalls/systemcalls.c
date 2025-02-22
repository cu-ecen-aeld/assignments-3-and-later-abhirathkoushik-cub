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
 * @file    systemcalls.c
 * @brief   This script contains various functions to execute system calls using system, fork and execv.
 *
 *
 * @author  Abhirath Koushik
 * @date    02-02-2025
 *
 */

#include "systemcalls.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    if (cmd == NULL)
    {
        return false;
    }

    int ret = system(cmd); // Using system() to call 'cmd' 

    return (ret != -1) && (WIFEXITED(ret) && WEXITSTATUS(ret) == 0);
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    pid_t pid = fork();  // Create a child process using fork()

    if (pid == -1)  // Fork failed
    {
        return false;
    }

    else if (pid == 0) { // It is the Child process
        execv(command[0], command);
        // If execv returns, it must have failed
        exit(EXIT_FAILURE);

    } else { // It is the Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) { // Waiting for the child to finish
            va_end(args);
            return false;
        }
        va_end(args);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        return false;
    }
    
    pid_t pid = fork(); // Create a child process using fork()

    if (pid == -1)  // Fork failed
    {
        close(fd);
        return false;
    }

    else if (pid == 0) { // It is the Child process
        // Redirect stdout to the output file
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);  // Close the file descriptor

        execv(command[0], command);
        // If execv returns, it must have failed
        exit(EXIT_FAILURE);
    }
     
    else  // It is the Parent process
    {
        close(fd);  // Parent doesn't need to keep the file open

        int status;
        if (waitpid(pid, &status, 0) == -1) { // Waiting for the child to finish
            va_end(args);
            return false;
        }
        va_end(args);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}
