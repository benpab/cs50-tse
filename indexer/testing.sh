#!/bin/bash

# testing.sh
# Ben Pable - CS50 S24
#
# test file for indexer.c
# 
# NOTE: requires crawler directory in relative path ../data/crawlerfiles
#
# Run with ./testing.sh > indexerTest.out

echo -e "\n********\nTESTFILE OUTPUT FOR indexer\n********\n\ntesting start...\n\n"

#make sure files are recent
make clean
make indextest
make

#testing for invalid parameters

echo -e "\ntest for only no args: \n"
./indexer
echo -e "\ntest for only 1 arg: \n"
./indexer ../data/crawlerfiles
echo -e "\ntest for too many args: \n"
./indexer ../data/crawlerfiles indexOne extraneousInput
echo -e "\ntest for fake directory: \n"
./indexer ./fakeDir indexOne

#make the directory temporarily read-only
echo -e "\ntest for inaccessible folder: \n"
chmod 444 ../data/crawlerfiles
./indexer ../data/crawlerfiles indexOne

echo -e "\ntest for running normally, should return nothing: \n"
chmod 777 ../data/crawlerfiles
#run normally
./indexer ../data/crawlerfiles indexOne

#run indextest
echo -e "test using indexer, should return nothing: \n"
./indextest indexOne indexTwo

#compare the two
#need to sort because hashtables do not preserve order, so it may have changes
sort ./indexOne -o ./indexOne
sort ./indexTwo -o ./indexTwo

echo -e "comparing files:\n"
if cmp -s ./indexOne ./indexTwo
then
   echo "Index successfully created and matched!"
else
   echo "Index not successfully created or matched"
fi