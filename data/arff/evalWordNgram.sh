#!/bin/sh
# Evaluate word ngram
cd output
rm -f *.out
i="5"
while [ $i -le 5 ]
do
        echo "Training word ngram $i with training arff dataset"
        java -mx4096m -cp ../../../../weka-3-4-8/weka.jar weka.classifiers.trees.J48 -t wordTraining$i.arff -i -k -d wordModel$i.model > wordTraining$i.out
        echo "Predicting word ngram $i with training model and testing arff dataset"
        java -mx4096m -cp ../../../../weka-3-4-8/weka.jar weka.classifiers.trees.J48 -l wordModel$i.model -T wordTesting$i.arff > wordTesting$i.out
        java -mx4096m -cp ../../../../weka-3-4-8/weka.jar weka.classifiers.trees.J48 -l wordModel$i.model -T wordTesting$i.arff -p 0 > wordPrediction$i.out
        java -mx4096m -cp ../../../../weka-3-4-8/weka.jar weka.classifiers.trees.J48 -l wordModel$i.model -T wordTesting$i.arff -i -k > wordTesting$i.out
	../../../Evaluation/src/getExtractionResult wordPrediction$i.out > wordResult$i.out
	cat wordResult$i.out >>wordResult.out
        i=`expr $i + 2`
done
cd ..
