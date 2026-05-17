#!/bin/bash

trap "echo 'Exiting...'; pkill AlcoholGallery; exit 1" SIGINT

if [[ $1 == "add" ]]; then
  touch ./src/include/$2.hpp
  touch ./src/lib/$2.cpp
  echo "#include \"../include/$2.hpp\"" >> ./src/lib/$2.cpp
  echo "#pragma once" >> ./src/include/$2.hpp
elif [[ $1 == "test" ]]; then
  rm -rf ./AlcoholGallery
  make
  ./AlcoholGallery
elif [[ $1 == "make" ]]; then
  rm -rf ./AlcoholGallery
  make
else
  echo "Usage: $0 [add|remove] [filename]"
fi
