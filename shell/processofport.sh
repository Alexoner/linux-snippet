#!/bin/sh

netstat -nlp | grep :$1 || lsof -i :$1 | grep LISTEN
