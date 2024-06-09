#!/bin/sh

# crawlertest.sh
# Ben Pable - CS50 S24
#
# unit test for crawler.c
#
# Run with ./crawlertest.sh > crawlerTest.out

TEMPDIR="crawler_test_output"

# If TEMPDIR exists remove and make new
if [ -e "$TEMPDIR" ]
then
    rm -rf $TEMPDIR
fi
mkdir $TEMPDIR

echo -e "\n********\nTESTFILE OUTPUT FOR CRAWLER\n********\n\ntesting start...\n\n"

##### argument tests #####
echo -e "test for only 1 arg: \n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html

echo -e "\n\ntest for only 2 args: \n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./$TEMPDIR

echo -e "\n\ntest for 4 args: \n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./$TEMPDIR 2 3

echo -e "\n\ntest for non-existent directory: \n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./fakeDir 2

echo -e "\n\ntest for too large of depth: \n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./$TEMPDIR 11

echo -e "\n\ntest for too small of depth: \n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./$TEMPDIR -1

echo -e "\n\ntest for non-internal domain: \n"

./crawler https://en.wikipedia.org/wiki/Unistd.h ./$TEMPDIR 2

echo -e "\n\ntest for invalid domain: \n"

./crawler fakedomain.org ./data/ 2

echo -e "\n\ntest normal function: \n"

./crawler "http://cs50tse.cs.dartmouth.edu/tse/letters/index.html" ./$TEMPDIR 2

# Check if all files exist
if [ -e "$TEMPDIR/.crawler" ] && [ -e "$TEMPDIR/1" ] && [ -e "$TEMPDIR/2" ] && [ -e "$TEMPDIR/3" ]; then
    echo -e "All necessary files exist in the directory.\n"
else
    echo -e "At least one file is missing!\n"
fi

#check if any extra were made, i.e. if wikipedia pages were saved
if [ -e "$TEMPDIR/4" ]; then
    echo -e "Too many files found!.\n"
else
    echo -e "No unnecessary saved files found.\n"
fi

rm -rf $TEMPDIR

echo -e "\ntesting complete, exiting..."