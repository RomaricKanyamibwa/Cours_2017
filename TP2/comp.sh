#!/bin/bash

gcc -Werror -Wall    pottsState.c -o pottsState  -lgsl -lgslcblas -lm -I /usr/lib/include -lm -g
