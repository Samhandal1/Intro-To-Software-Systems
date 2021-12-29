#!/bin/bash

# Samantha Handal 260983914

# variable for input directory which contains the logs

dirname=$1
opfile=$2

# if the script is not invoked with the correct number of arguments, prompt usage instructions and exit
# prompted when the usering isn't imputting one arguement after the script name

if [[ $# -ne 2 ]]
then
        echo "Usage $0 <dirname> <opfile>"
        exit 1
fi

# if the passed argument is not a valid directory name, it should throw an error message and exit
# the error messgage is passed as error message and not just a normal output (hence redirrecting messsage to stderr)

if [[ ! -d $dirname ]]
then
        echo "Error $dirname is not a valid directory">/dev/stderr
        exit 1
fi

# if the header of the file has the additional feils join time and leave time, then we call this funtion
# on the file to account for the extra feild in each line, the function extracts the nessesary information
# from the csv file (labFile) and appends it's output to the specified output file

joinLeaveTime()
{
	# local variables passed to the function

	labFile=$1
	outputFile=$2

	# we want to extract the name of the file when given an absosute path, we do this using the
	# basename command and the filename is now stored in the fileName variable
	# we then want to extract the lab letter from the file name so we can use it in the output
	# file, using the manual for awk (man awk), i used the awk functions toupper() and substr().
	# substr() extracts the number of characters at an index in the line that is fed to awk
	# toupper() makes that character uppercase weather it is already uppercase or lowercase doesnt
	# matter

	fileName=$(basename "$labFile")
	labNum=$(echo "$fileName" | awk '{print toupper(substr($0,5,1))}')
	
	# in this awk statement i set a awk variable to the lab letter extracted in the previous step (num)
	# then setting the read feild seperator and output feild separator to comas (this is how csv files 
	# are seperated) i search for all the lines with the symbol @ because this means they have an email
	# in the line, all lines with a valid email are they lines we need to transfer to the output file,
	# this is also a shortcut to skip reader the header of the lab.csv file. I then redirrect (and append)
	# the output to the output file

	awk -v num="$labNum" 'BEGIN { FS=",";OFS="," }
	/@/ {print $2,$1,num,$5}' $labFile >> $outputFile
}

totalDuration()
{
	labFile=$1
	outputFile=$2
	fileName=$(basename "$labFile")
	labNum=$(echo "$fileName" | awk '{print toupper(substr($0,5,1))}')
	
	awk -v num="$labNum" 'BEGIN { FS=",";OFS="," }
	/@/ {print $2,$1,num,$3}' $labFile >> $outputFile
}

touch $2
echo "User Email,Name (Original Name),Lab,Total Duration (Minutes)">$2

# for loop which executes ________ for each lab file found
# find is evoked and recursively looks for log files under the directory inputted by the user
# for each file (f), using -iname is searching case insensitive (i)
for f in $(find $dirname -iname 'lab-[a-i].csv')
do
	if grep -q 'Total Duration (Minutes)' $f
	then
		totalDuration $f $2
	fi

	if grep -q 'Join Time' $f
	then
		joinLeaveTime $f $2
	fi
done
