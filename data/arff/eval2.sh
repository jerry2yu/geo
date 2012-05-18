#Generate data and evaluate it. 
#usage: ./eval datasetname(pizza|hotel|contact|all) testname(local|pos|punctuation|..)
echo "===== Generating and evaluating $1 - $2 ====="
./genC45.sh $1 $2
./evalC45.sh $1 $2
mkdir -p $1/result$2
#copy the result to a new folder
cp $1/result/* $1/result$2/.
rm $1/result/*.out
