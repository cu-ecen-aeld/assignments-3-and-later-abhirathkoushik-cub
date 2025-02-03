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

# GCC Compilation for Native x86-64 version
default:
	gcc -g -Wall -c -o writer.o writer.c
	gcc -g -Wall -I/ writer.o -o writer

# GCC Compilation for Cross-Compiling Arm Aarch64 version
cross:
	$(CROSS_COMPILE)gcc -g -Wall -c -o writer.o writer.c
	$(CROSS_COMPILE)gcc -g -Wall -I/ writer.o -o writer

# Cleanup of the Writer Script and .o files
clean:
	rm -rf ./writer
	rm -f *.o
