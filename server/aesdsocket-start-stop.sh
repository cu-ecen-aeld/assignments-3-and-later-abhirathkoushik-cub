#!/bin/sh

#  Copyright (C) 2025 by Abhirath Koushik

#  Redistribution, modification or use of this software in source or binary
#  forms is permitted as long as the files maintain this copyright. Users are
#  permitted to modify this and use it to learn about the field of embedded
#  software. Abhirath Koushik and the University of Colorado are not liable for
#  any misuse of this material.

#  @file    aesdsocket-start-stop.sh
#  @brief   This script is used to start and stop the aesdsocket program  
#           using start-stop-daemon.

#  @author  Abhirath Koushik
#  @date    02-21-2025

case "$1" in
    start)
        echo "Starting aesdsocket program!"
        start-stop-daemon -S --background -n aesdsocket -a /usr/bin/aesdsocket
        ;;
    stop)
        echo "Stopping aesdsocket program!"
        start-stop-daemon -K -n aesdsocket
        ;;
    *)
        echo "Usage $0 {start | stop}"
    exit 1
esac
