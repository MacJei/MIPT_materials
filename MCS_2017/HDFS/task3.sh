#! /usr/bin/env bash

hdfs fsck $1 -blocks | grep 'Total blocks' | grep -o [0-9]* | head -1
