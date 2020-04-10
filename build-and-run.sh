#!/bin/bash
sudo docker build -t andrekrippahl/phase5-computao-em-nuvem .
sudo docker run -p 80:8080 -d andrekrippahl/phase5-computao-em-nuvem
