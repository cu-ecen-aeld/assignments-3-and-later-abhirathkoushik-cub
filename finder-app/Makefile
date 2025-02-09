#  Copyright (C) 2025 by Abhirath Koushik

#  Redistribution, modification or use of this software in source or binary
#  forms is permitted as long as the files maintain this copyright. Users are
#  permitted to modify this and use it to learn about the field of embedded
#  software. Abhirath Koushik and the University of Colorado are not liable for
#  any misuse of this material.

#  @file    Makefile
#  @brief   This script is used to automate the compilation and cleanup processes of the writer script.

#  @author  Abhirath Koushik
#  @date    01-26-2025

# Check and use gcc cross compile if variable is defined
ifdef CROSS_COMPILE
      CC=$(CROSS_COMPILE)gcc
else
# Else use native gcc compile
    CC=gcc
endif

all: writer
writer: writer.c
	$(CC) -o writer writer.c

# Cleanup of the Writer Script and .o files
.PHONY: clean
clean:
	rm -f *.o writer
