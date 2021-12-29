#!/bin/bash

# Samantha Handal 260983914

inputCSV=$1
outputHTML=$2

if [[ $# -ne 2 ]]
then
	echo "Usage $0 <CSV File> <HTML File>"
	exit 1
fi

echo "<TABLE>" > $outputHTML

# \%regexp% ... if regular expression contains backslashes, we need to tell sed to 
# treat it as part of the expression
# extra information on sed was found with this website
# https://www.gnu.org/software/sed/manual/sed.html#Common-Commands
sed -e 's/^/<TR><TD>/' -e 's/,/<\/TD><TD>/g' -e 's/$/<\/TD><\/TR>/' $inputCSV >> $outputHTML

echo "</TABLE>" >> $outputHTML
