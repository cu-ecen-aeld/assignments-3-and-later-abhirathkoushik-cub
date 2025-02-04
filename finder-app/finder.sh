#!/bin/sh


#  Copyright (C) 2025 by Abhirath Koushik

#  Redistribution, modification or use of this software in source or binary
#  forms is permitted as long as the files maintain this copyright. Users are
#  permitted to modify this and use it to learn about the field of embedded
#  software. Abhirath Koushik and the University of Colorado are not liable for
#  any misuse of this material.

#  @file    finder.sh
#  @brief   This script searches for a string recursively in a given directory
#           and prints the number of matching lines.

#  @author  Abhirath Koushik
#  @date    01-20-2025


# Check Input Arguments for the Script
if [ $# -ne 2 ]
then
    echo "ERROR: Required 2 Arguments (Directory Path and Search String) not present"
    exit 1
fi

filesdir=$1
searchstr=$2

# Check if the Directory Exists
if [ ! -d "$filesdir" ]
then
    echo "ERROR: Given Directory '$filesdir' does not exist"
    exit 1
fi

# Count the number of lines in the Directory
num_files=$(find "$filesdir" -type f | wc -l)

# Count the Matching Lines where the Search String was found
num_match_lines=$(grep -r "$searchstr" "$filesdir" | wc -l)

# Print the Results
echo "The number of files are $num_files and the number of matching lines are $num_match_lines"
