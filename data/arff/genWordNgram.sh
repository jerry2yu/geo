#!/bin/sh
# Generating dataset for Weak

mkdir -p output
cd ../../GenData/src
i="1"
while [ $i -le 9 ]
do
        echo "Outputing training arff data for ngram $i"
        ./genData hotelTrainingFileList $i > ../../data/arff/output/wordTraining$i.arff
	echo "Outputing testing arff data for ngram $i"
	./genData hotelTestingFileList $i > ../../data/arff/output/wordTesting$i.arff
        i=`expr $i + 2`
done

cd ../../data/arff

