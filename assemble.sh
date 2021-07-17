#!/bin/env bash

URL="https://github.com/TheThirdOne/rars/releases/download/continuous/rars_46ab74d.jar"
JAR="rars.jar"

if [[ -f "$JAR" ]]; then
  echo "Found rars jarfile"
else
  echo "Couldn't find rars jar; downloading"
  curl -L "$URL" -o "$JAR"
fi

if [[ -n "$1" ]]; then
  echo "Assembling $1"
  java -jar "$JAR" a "$1" dump .text Binary text.bin mc CompactTextAtZero
  java -jar "$JAR" a "$1" dump .data Binary data.bin mc CompactTextAtZero
else
  echo "Usage: $0 [asm file]"
fi

