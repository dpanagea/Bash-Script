#!/bin/bash

input=$1
dir=$2
filesPerDir=$3

#check if all arguments given
if [ "$#" -ne 3 ]; then
    echo "Not enough arguments."; exit -1
fi

#check if filesPerdir is positive numbe
[ -n "$3" ] && [ "$3" -eq "$3" ] && [ $3 -gt 0 ] 2>/dev/null
if [ $? -ne 0 ]; then
    echo "numFilesPerDirectory input isn't a number or it's negative"; exit -1
fi

rm -r $dir
mkdir $dir
cp $input $dir/$input
cd $dir
#for every line, check if the 4th item (country) exists as a subdir. If not, create the subdir, with the txt files
while read line; do
    IFS=' ' read -r -a arr <<< "$line"
    cntr="${arr[3]}"
    if [ ! -d "${arr[3]}" ]; then
        mkdir "${arr[3]}"
        cd "${arr[3]}"
        for ((i=1; i<=$filesPerDir; i++))
        do
            file="$cntr-$i.txt"
            touch $file
        done
        cd ..
    fi
    cd "${arr[3]}"

    #A monkey-ish way of finding where to write each line 
    : ' 
    file="$cntr-1.txt"
    max=$(wc -l <$file)
    i=2
    while [ $i -le $filesPerDir ]
    do
        file="$cntr-$i.txt"
        curr=$(wc -l <$file)
        if [ "$max" -gt "$curr" ]; then
            echo $line >> $file
            break
        fi
        let i+=1
    done
    if [ "$i" -gt $filesPerDir ]; then
        file="$cntr-1.txt"
        echo $line >> $file
    fi
    '

    #And a clever way
    file=$(wc -l *.txt | sort -n |head -1 | awk '{print $2}') 
    echo $line >> $file

    cd ..
done < "$input"
rm $input
cd ..

