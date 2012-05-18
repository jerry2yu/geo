#generate training, validation, testing file list
#First split 20% of total files for testing. The remain 80% will be 
#used for training and validation, and they will be splitted by the
#n fold cross validation way.
#Usage: ./genFileLists all|pizza|contact|hotel
fold="10"
../../GenData/src/splitFileList $1/FileList/fileList 0.2 $fold
i="1"
while [ $i -le $fold ]
do
  mv splitFileListTraining$i $1/FileList/trainingFileList$i  
  mv splitFileListValidating$i $1/FileList/validatingFileList$i  
  i=`expr $i + 1`
done
mv splitFileListTesting $1/FileList/testingFileList  

