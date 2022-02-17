#!/bin/bash 

PATH="src/"

# reads test files and sizes 
readarray -t FILES < test/config/test_files
readarray -t SIZES < test/config/test_sizes

for FILE in "${FILES[@]}"
do

    echo ""
    echo "--------------------"
    echo "Moving $FILE"
    echo "--------------------"
    echo ""

    /bin/cp $PATH/$FILE $PATH/prefetcher.cc

    for SIZE in "${SIZES[@]}"
    do
        echo ""
        echo "-------------------"
        echo "$FILE - $SIZE"
        echo "-------------------"
        echo ""

        # updates the size 
        /usr/bin/python3 scripts/update_size.py $SIZE
        
        # todo: execute 

        # moves the output 
        /bin/mkdir test/output/$FILE/ 2>/dev/null; /bin/cp stats.txt test/output/$FILE/$SIZE.txt
    done
done


    