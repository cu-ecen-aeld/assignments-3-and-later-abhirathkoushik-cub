#  Copyright (C) 2025 by Abhirath Koushik

#  Redistribution, modification or use of this software in source or binary
#  forms is permitted as long as the files maintain this copyright. Users are
#  permitted to modify this and use it to learn about the field of embedded
#  software. Abhirath Koushik and the University of Colorado are not liable for
#  any misuse of this material.

#  @file    Makefile
#  @brief   This script is used to automate the compilation and cleanup processes of the aesdsocket script.

#  @author  Abhirath Koushik
#  @date    02-20-2025

# Check and use gcc cross compile if variable is defined
ifdef CROSS_COMPILE
      CC ?= $(CROSS_COMPILE)gcc
else
# Else use native gcc compile
    CC ?= gcc
endif

all: aesdsocket
aesdsocket: aesdsocket.c
	$(CC) -o aesdsocket aesdsocket.c -Wall -Werror

# Cleanup of the aesdsocket Script and .o files
.PHONY: clean
clean:
	rm -f *.o aesdsocket
