#!/bin/bash

#Samantha Handal 260983914

#variables used throughout the script
#backupdirname and filetobackup take user imput for backup directory and file wanted to backup
#date stores the correct format of date, ie 20210129
#filename strips directory and keeps suffix (using basename) from imput of file to backup,
#imput for this variable can include path of where to find it, which we want to discard 

backupdirname=$1
filetobackup=$2
date=$(date '+%Y%m%d')
filename=$(basename "$2")

#series of if statements checks if user is imputing properly, so script will run smoothly
if [[ $# != 2 ]]
then
	#if number of tokens imputted is NOT equal to 3 (0,1,2), prompt user how to use script and exit script 
	echo "Usage: ./backup.sh backupdirname filetobackup"
	exit 1
elif [[ ! -d $backupdirname ]]
then
	#if imput for backup directory is NOT(!) an existing directory(-d), then tell user and exit script
	echo "Error!! $backupdirname is not a valid directory"
	exit 1
elif [[ ! -f $filetobackup ]]
then
	#if imput for file to backup is NOT(!) an existing file(-f), then tell user and exit script
	echo "Error!! $filetobackup is not a valid file"
	exit 1
elif ls $backupdirname | grep -q "$date"
then
	#if statement is true if there is a file named with the date (in the correct format, using pattern match) within the backup directory
	#grep -q supresses printing the file, so it just return true or false
	echo "Backup file already exists for $date"
	exit 1
else
	#if all the conditions are met, make backup by copying file you want to backup to backup directory
	#when copying to backup directory, append the file name and date so the copied file's name gets changed to include the backup date 
	cp $filetobackup ${backupdirname}/$filename.$date

fi
