#!/bin/sh
# Generating training, validating (n fold cross validation) and testing dataset for C4.5
# Usage: ./genWordNgram all|pizza|hotel|contact, testName( such as "Geo"|"Pos"|"Local"|"LocalGeo", etc. )
 
i="1"
j="1"
cd ../../GenData/src
# generating for different ngram n
while [ $i -le 9 ]
do 
  echo "$2($1) Outputing testing data for ngram $i"
  ./genData$2 ../../data/arff/$1/FileList/testingFileList $i 
  mv c45.names ../../data/arff/$1/dataset/testing$i.names
  mv c45.data ../../data/arff/$1/dataset/testing$i.data
  #n fold cross validation
  while [ $j -le 10 ]
  do
        echo "$2($1) Outputing training data for ngram $i fold $j"
        ./genData$2 ../../data/arff/$1/FileList/trainingFileList$j $i
	mv c45.names ../../data/arff/$1/dataset/training$i-$j.names
        mv c45.data ../../data/arff/$1/dataset/training$i-$j.data
	echo "$2($1) Outputing validating data for ngram $i fold $j"
	./genData$2 ../../data/arff/$1/FileList/validatingFileList$j $i 
        mv c45.names ../../data/arff/$1/dataset/validating$i-$j.names
        mv c45.data ../../data/arff/$1/dataset/validating$i-$j.data
        j=`expr $j + 1`
  done
  j="1"
  i=`expr $i + 2`
done
cd ../../data/arff/  

