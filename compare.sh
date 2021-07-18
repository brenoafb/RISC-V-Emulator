#!/bin/env bash

# run the program with rars
java -jar rars.jar $1

# assemble
./assemble.sh $1

# run
./main.out text.bin data.bin
