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
 * @file    threading.c
 * @brief   This file consists of the functions related to Threading and Obtaining Mutexes.
 *
 *
 * @author  Abhirath Koushik
 * @date    02-08-2025
 *
 */

#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* data = (struct thread_data*) thread_param;
    
    // Wait before attempting to acquire mutex
    usleep(data->wait_to_obtain_ms * 1000);
    
    // Acquire mutex
    if (pthread_mutex_lock(data->mutex) != 0) {
        ERROR_LOG("Failed to lock mutex");
        data->thread_complete_success = false;
        return thread_param;
    }
    
    // Hold mutex for specified time
    usleep(data->wait_to_release_ms * 1000);
    
    // Release mutex
    if (pthread_mutex_unlock(data->mutex) != 0) {
        ERROR_LOG("Failed to unlock mutex");
        data->thread_complete_success = false;
        return thread_param;
    }
    
    data->thread_complete_success = true;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    // Allocate thread data
    struct thread_data *data = malloc(sizeof(struct thread_data));
    if (!data) {
        ERROR_LOG("Memory allocation failed");
        return false;
    }

    // Initialize thread data
    data->mutex = mutex;
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;
    data->thread_complete_success = false;
    
    // Create thread
    if (pthread_create(thread, NULL, threadfunc, data) != 0) {
        ERROR_LOG("Thread creation failed");
        free(data);
        return false;
    }
    
    return true;
}
