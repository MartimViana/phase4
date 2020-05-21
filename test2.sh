#!/bin/bash
rm speed.temp
rm thrust.temp
./compile.sh
./aircraft.out 1000 823 300
