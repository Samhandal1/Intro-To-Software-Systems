#!/bin/bash

# variable for input directory which contains the logs
sensorlogdir=$1

# if the script is not invoked with the correct number of arguments, prompt usage instructions and exit
# prompted when the usering isn't imputting one arguement after the script name 
if [[ $# -ne 1 ]]
then
	echo "Usage $0 sensorlogdir"
	exit 1
fi

# if the passed argument is not a valid directory name, it should throw an error message and exit
# the error messgage is passed as error message and not just a normal output (hence redirrecting messsage to stderr)
if [[ ! -d $sensorlogdir ]]
then
	echo "Error! $sensorlogdir is not a valid directory name">/dev/stderr
	exit 1
fi

# function for printing temperature information (just the numbers no header)
temperatureInfo()
{
	# logFile is variable for first argument passed to the function, which in this case is the absolute path to a log file
	logFile=$1
	
	# grep filters the log file so that only lines with 'sensor readouts' are processed (this holds the sensor info for each hour)
        
	# sed replaces the first and second instace of "-" with ":" (we do this so awk will not get confused between 
	# hyphens and negative signs), sed also relaces 2 or more spaces with only one space for simplicity
	# backslash is used to indicate "+" means many and not a character to replace

	# these are now imputted to awk which uses a space and colin as field seperators, and sets the output field
	# seperator to a coma. First we check if any of our sensors is printing ERROR, if so, it is replaced by the value of 
	# last hour's reading, this is done through prevSensor variable which hold's the last line's field value and is updated after
	# each line execution. For each line, we then print Year,Month,Day,Hour,Sensor1,Sensor2,Sensor3,Sensor4,Sensor5 
	# held within the awk variables $n

	grep 'sensor readouts' $logFile | sed -e 's/-/:/' -e 's/-/:/' -e 's/  \+/ /' | awk '
		BEGIN { FS="[: ]";OFS="," }
		{ if ($9 == "ERROR") { $9=prevSensor1 } }
		{ if ($10 == "ERROR") { $10=prevSensor2 } }
		{ if ($11 == "ERROR") { $11=prevSensor3 } }
		{ if ($12 == "ERROR") { $12=prevSensor4 } }
		{ if ($13 == "ERROR") { $13=prevSensor5 } }
		{ print $1,$2,$3,$4,$9,$10,$11,$12,$13 }
		{ prevSensor1=$9; prevSensor2=$10; prevSensor3=$11; prevSensor4=$12; prevSensor5=$13 }'


}

# function for printing temperature max and min (just the numbers no header)
temperatureMaxMin()
{
	# same setup as last function, takes in same log file and filters its contents the same way (grep and sed) to then pass it to awk
	logFile=$1

	# using the same delimeters, we want to find the min and max value for each hour, in a for loop we compare each sensor to eachother
	# until we find the min and the max (which doesnt include sensors with errors). The referance sensor we use is initialize originally 
	# set to the first sensor, although if the first sensor has an ERROR, then we cannot use it as our base to compare, in this case we move 
	# along the list of sensors until our min variable does not hold ERROR as its value. Then we enter the for loop where we check each 
	# sensor... if it is not equal to ERROR and it is smaller (for min val) or bigger (for max val) than the current min variable value,
	# then we update the min or max value to that sensor reading. Then to get the name of the sensor for the printed log, we check if the
	# sensor is equal to the max value, if so we know the sensor number. Last line prints Year,Month,Day,Hour,MaxTemp,MaxSensor,MinTemp,MinSensor

	grep 'sensor readouts' $logFile | sed -e 's/-/:/' -e 's/-/:/' -e 's/  \+/ /' | awk '
		BEGIN { FS="[: ]";OFS="," }
		{ 
			min=$9
			if(min == "ERROR")min=$10
                        if(min == "ERROR")min=$11
                        if(min == "ERROR")min=$12
                        if(min == "ERROR")min=$13
			for(i=9;i<=13;i++)if($i != "ERROR" && $i<min)min=$i
		}
		{
			max=$9
			if(max == "ERROR")max=$10
			if(max == "ERROR")max=$11
			if(max == "ERROR")max=$12
			if(max == "ERROR")max=$13
			for(i=9;i<=13;i++)if($i != "ERROR" && $i>max)max=$i
		}
		{ maxSensor="Sensor1"; minSensor="Sensor1" }
		{ if ($10 == max) { maxSensor="Sensor2" } }
		{ if ($11 == max) { maxSensor="Sensor3" } }
		{ if ($12 == max) { maxSensor="Sensor4" } }
		{ if ($13 == max) { maxSensor="Sensor5" } }
		{ if ($10 == min) { minSensor="Sensor2" } }
		{ if ($11 == min) { minSensor="Sensor3" } }
		{ if ($12 == min) { minSensor="Sensor4" } }
		{ if ($13 == min) { minSensor="Sensor5" } }
		{ print $1,$2,$3,$4,max,maxSensor,min,minSensor }'


}

# for loop which executes temperatureInfo and temperatureMaxMin for each log file found
# find is evoked and recursively looks for log files under the directory inputted by the user
# for each file (f), the functions execute temperatureInfo and temperatureMaxMin then moves onto the next file
for f in $(find $sensorlogdir -name 'sensordata-*.log')
do
	# header for temperature information
	echo "Processing sensor data set for <$f>"
        echo "Year,Month,Day,Hour,Sensor1,Sensor2,Sensor3,Sensor4,Sensor5"
	
	# function call for temperatureInfo which takes as input the current full path to sensor file
	temperatureInfo $f
	
	echo "===================================="
	
	# header for maximum and minimum statistics readout
	echo "Readout statistics"
	echo "Year,Month,Day,Hour,MaxTemp,MaxSensor,MinTemp,MinSensor"
	
	# function call for temperatureMaxMin which takes as input the current full path to sensor file
	temperatureMaxMin $f

	echo "===================================="	
done

# function for printing error statistics (just the numbers no header)
errorStatistics()
{
	# same for loop as before, goes through each log file again... the for loop is in the function for simplicity as we are not executing 
	# this snippet of code on it (little output at the end)
	for f in $(find $sensorlogdir -name 'sensordata-*.log')
	do
	
	# same setup as last function, takes in same log file and filters its contents the same way (grep and sed) to then pass it to awk

	# in the begin statement, we set up variables for each sensor aswell as a total variable so we can update them as we go through each line, 
	# initializing these counter variables in BEGIN makes sure they will not reset after each line. We then check each sensor field if it
	# contains ERROR, if so the counter var associated with that sensor will increment by 1. At the END, the total variable will tabulate
	# the number of errors in the file then print Year,Month,Day,Sensor1,Sensor2,Sensor3,Sensor4,Sensor5,Total (only print once after
	# each line is processed as these statements are in the END portion of the awk statement)

	grep 'sensor readouts' $f | sed -e 's/-/:/' -e 's/-/:/' -e 's/  \+/ /' | awk '
        BEGIN { FS="[: ]";OFS=","
		errSensor1=0;errSensor2=0;errSensor3=0;errSensor4=0;errSensor5=0;totalerr=0 }
	{ Year=$1;Month=$2;Day=$3 }	
	{ if ($9 == "ERROR")errSensor1+=1 }
	{ if ($10 == "ERROR")errSensor2+=1 }
	{ if ($11 == "ERROR")errSensor3+=1 }
	{ if ($12 == "ERROR")errSensor4+=1 }
	{ if ($13 == "ERROR")errSensor5+=1 }
	END { totalerr=errSensor1+errSensor2+errSensor3+errSensor4+errSensor5
		print Year,Month,Day,errSensor1,errSensor2,errSensor3,errSensor4,errSensor5,totalerr }'

	done
}

# header for maximum and minimum statistics readout
echo "Sensor error statistics"
echo "Year,Month,Day,Sensor1,Sensor2,Sensor3,Sensor4,Sensor5,Total"

# function call for errorStatistics, its output is then pipped to sort command which uses comas as delimeters and 
# sorts the last collumn from largest to smallest amount of error, (do this with "r" reverse order). If 2 files have the same 
# number of errors, we then sort them based on chronological order (collumns 1 - year, 2 - month, 3 - day)
errorStatistics | sort -t"," -k9r,9 -k1,1 -k2,2 -k3,3

echo "===================================="
