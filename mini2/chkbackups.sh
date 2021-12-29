#!/bin/bash

#Samantha Handal 260983914

#variables from imput
backupdirname=$1
sourcedir=$2

#function which checks if there is already a backup file within the backup directory for the current day
#if there is none, then script prints there is no backup today for a certain file
#if all files are backed up, then script prints statement which says all files are backed up
CheckForBackup()
{
	#loops through all files in source directory
        for file in "$sourcedir"/*
        do
		#sourcedir/* outputs full path, we just want the name of the individual file we are checking
		#name stores the file name with type, date stores the correct format of date, ie 20210129 
		#todayname concatinates name and date so we can use this variable to properly search for backup files

                name=$(basename "$file")
                date=$(date '+%Y%m%d')
                todayname=${name}.$date

		#if statement is true if there is NO file named with filename.type.date within the backup directory
		#grep -q supresses printing the file, so it just return true or false

                if ! (ls $backupdirname | grep -q "$todayname")
                then
                        echo "$name does not have a backup for today"
			exit 0
                fi

        done

	#once all the files within the source directory are pattern checked, this means that all the files where backed up
        echo "All files in $sourcedir have backups for today in $backupdirname"

}

#series of if statements checks if user is imputing properly, so script will run smoothly
if [[ $# -ne 1 ]] && [[ $# -ne 2 ]]
then
	#if number of tokens imputted is NOT equal to 3 (0,1,2), AND is not equal to 2 (0,1), prompt user how to use script and exit script 
	echo "Usage: ./chkbackups.sh backupdirname [sourcedir]"
	exit 1
elif [[ ! -d $backupdirname ]]
then
	#if imput for backup directory is NOT(!) an existing directory(-d), then tell user and exit script
	echo "Error!! $backupdirname is not a valid directory"
	exit 1
elif [[ $# -eq 1 ]]
then
	#if only one argument is passed, ie want to check current directory
	#do not need to check if current directory is a directory because if you are running a program in it then it exists
	#do not need to check if directory is empty because if you are running a program in it, then it will have atleast one file
	#goes straight to checking for backups
        sourcedir=$(pwd)
	CheckForBackup
elif [[ ! -d $sourcedir ]]
then
	#if imput for source is NOT(!) an existing directory(-d), then tell user and exit script
	echo "Error!! $sourcedir is not a valid directory"
	exit 1
elif [[ -z "$(ls -A $sourcedir)" ]]
then
	#if string "$(ls -A $sourcedir)" has 0 elements(-z), ie if there are no files listed in source directory then tell user and exit script
	#(-A) lists all files including hidden, but excluding parent and current directory
	echo "Error!! $sourcedir has no files"
	exit 1
else
	#if all conditions are met, then check for backup files (by calling the CheckForBackup method)
	CheckForBackup
fi
