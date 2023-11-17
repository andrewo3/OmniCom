#!/bin/bash

gcc ../tools/jumpbeats.c -o /tmp/jumpbeats
/tmp/jumpbeats $1 $2
