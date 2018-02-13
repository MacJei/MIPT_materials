#! /usr/bin/env bash

block_id=$(hdfs fsck $1 -files -blocks -locations | grep -o -m 1 blk_[0-9]*)
hdfs fsck -blockId $block_id | grep -o -m 1 mipt-node[0-9]*.atp-fivt.org

