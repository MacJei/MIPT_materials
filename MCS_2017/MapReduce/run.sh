#!/usr/bin/env bash

IN_DIR="/data/wiki/en_articles"
OUT_DIR="task1_"$(date +"%s%6N")
NUM_REDUCERS=8

hadoop jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
	-D mapreduce.job.name="mcs2017100 phase1" \
    -D mapreduce.job.reduces=$NUM_REDUCERS \
    -files mapper1.py,reducer1.py \
    -mapper mapper1.py \
    -reducer reducer1.py \
    -input /data/wiki/en_articles
    -output "${OUT_DIR}_phase1"


hadoop jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
	-D mapreduce.job.name="mcs2017100 phase2" \
    -D mapreduce.job.reduces=$NUM_REDUCERS \
    -files mapper2.py,reducer2.py \
    -mapper mapper2.py \
    -reducer reducer2.py \
    -input "${OUT_DIR}_phase1" \
    -output "${OUT_DIR}_phase2"

hdfs dfs -head 10 "${OUT_DIR}_phase2/part-00000"
hdfs dfs -rm -r -skipTrash "${OUT_DIR}*" > /dev/null
