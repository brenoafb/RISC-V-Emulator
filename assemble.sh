#!/bin/env sh

java -jar rars.jar a "$1" dump .text Binary text.bin mc CompactTextAtZero
java -jar rars.jar a "$1" dump .data Binary data.bin mc CompactTextAtZero

