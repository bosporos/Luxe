#!/usr/bin/env bash

gcc -I/usr/local/include -L/usr/local/lib -lwiringPi -o ledserial ledserial.c
