#Evaluation using C45. 
#Usage: ./evalC45.sh pizza|geo|hotel|all testName( such as "Local"|"Geo"|"Pos"|"LocalGeo"|"LocalGeoPos", etc. )
i="1"
j="1"
mkdir -p $1/result
rm -f result.tmp
# eval for different ngram n
while [ $i -le 9 ]
do
  # n fold cross validation
  while [ $j -le 10 ]
  do
	echo "***** $2($1) ngram $i fold $j *****" >> $1/result/result$i.out
	echo "***** $2($1) ngram $i fold $j *****" >> $1/result/tree$i.out
	rm -f address.names
        rm -f address.data
	rm -f address.test
	echo "$2($1) Training and Validating word ngram $i with training dataset, fold $j"
	cp $1/dataset/training$i-$j.names address.names
	cp $1/dataset/training$i-$j.data address.data
	cp $1/dataset/validating$i-$j.data address.test
        ../../Evaluation/src/C45/c4.5 -f address -u -v 1 >> $1/result/tree$i-$j.out
	cat $1/result/tree$i-$j.out >> $1/result/tree$i.out
	echo "$2($1) Testing word ngram $i with validating(for pruning trees) and testing, fold $j"
        cp $1/dataset/testing$i.data address.test
        ../../Evaluation/src/C45/c45label -f address
	mv address.classif $1/result/predict$i-$j.out
        ../../Evaluation/src/getExtractionResult $1/result/predict$i-$j.out > $1/result/result$i-$j.out
	cat $1/result/result$i-$j.out >> $1/result/result$i.out
        j=`expr $j + 1`
  done
  #copy the n fold cross validation result
  echo "***** $2($1) ngram $i for $1 *****" >> $1/result/result.out
  ../../Evaluation/src/sum10FoldCV result.tmp >> $1/result/result.out
  rm result.tmp
  j="1"
  i=`expr $i + 2` 
done
