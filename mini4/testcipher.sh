#!/bin/bash

#Samantha Handal 260983914

# if there doesn't exist a file named cipher.c in the current directory
# tell user and quit script
if ! ls | grep -q "cipher.c"
then
	echo "there is no file cipher.c in this directory"
	echo "exiting testcipher.sh..."
	exit 1
fi

#compile and give execute permissions to cipher.c
gcc cipher.c -o cipher
chmod +x cipher

# if there is no executable "cipher" in the current directory (therefore did not
# compile), tell user and quit script
if ! ls | grep -q "cipher"
then
	echo "could not compile cipher.c"
	echo "exiting testcipher.sh..."
	exit 1
fi

# variable to keep track of number of errors in script
numOfErrors=0

# CODE FOR TEST 1

# printing header for tests 1, says what it tests and how
echo "=======<<TEST 1>>======="
echo "program is not passed 3 arguments"
echo "./cipher"
echo "========================"
echo ""

# runs the tests with parameters for example 1 in the assignment instructions
# we use echo -n because I thought it would be pretty if the output of the c program 
# would print next to the "Output: "
# also variable exitCodeTest1 stores the exit code of the previous line (so of the 
# c program)
echo -n "Output:  "
./cipher
exitCodeTest1=$?

# if the output of the test does not give what the assignment says it should, I notified the
# user telling them it didnt print properly, and I tell them what it should print
# I also increment the numOfErrors variable so that at the end you can see all your mistakes
if [[ $(./cipher) != "Usage : ./cipher [-e|-d] <key> <MESSAGE>" ]]
then
	echo ""
	echo "***"
	echo "ERROR!! error message not printing properly..."
	echo "EXPECTED: Usage : ./cipher [-e|-d] <key> <MESSAGE>"
	((numOfErrors++))
	echo "***"
	echo ""	
fi

# I then print the exit code from the c program (store before in exitCodeTest1 var)
echo "Exit code from test 1: $exitCodeTest1"

# if the test produces the right output, then I notified the used that the test passed
if [[ $(./cipher) == "Usage : ./cipher [-e|-d] <key> <MESSAGE>" ]]
then
	echo ""
	echo "~test passed~"
fi

echo ""

# CODE FOR TEST 2

# its the same procedure for every test, so I won't copy and paste the same sentice
# 5 more times...
echo ""
echo "=======<<TEST 2>>======="
echo "if first argument is not a valid options"
echo "./cipher -k 5 HELLOTHERE"
echo "========================"
echo ""

echo -n "Output:  "
./cipher -k 5 HELLOTHERE
exitCodeTest2=$?

if [[ $(./cipher -k 5 HELLOTHERE) != "Error -k is not a valid option" ]]
then
	echo ""
	echo "***"
        echo "ERROR!! error message not printing properly..."
	echo "EXPECTED: Error -k is not a valid option"
	((numOfErrors++))
	echo "***"
	echo ""
fi

echo "Exit code from test 2: $exitCodeTest2"

if [[ $(./cipher -k 5 HELLOTHERE) == "Error -k is not a valid option" ]]
then
	echo ""
	echo "~test passed~"
fi

echo ""

# CODE FOR TEST 3

echo ""
echo "=======<<TEST 3>>======="
echo "if second argument is not a valid options"
echo "./cipher -e 4.r HELLOTHERE"
echo "========================"
echo ""

echo -n "Output:  "
./cipher -e 4.r HELLOTHERE
exitCodeTest3=$?

if [[ $(./cipher -e 4.r HELLOTHERE) != "Error 4.r is not a valid key" ]]
then
	echo ""
        echo "***"
        echo "ERROR!! error message not printing properly..."
        echo "EXPECTED: Error 4.r is not a valid key"
        ((numOfErrors++))
        echo "***"
	echo""
fi

echo "Exit code from test 3: $exitCodeTest3"

if [[ $(./cipher -e 4.r HELLOTHERE) == "Error 4.r is not a valid key" ]]
then
	echo ""
	echo "~test passed~"
fi

echo ""

# CODE FOR TEST 4

echo ""
echo "=======<<TEST 4>>======="
echo "For valid arguments, the program should produce the encrypted message"
echo "./cipher -e 3 THISISASECRETMESSAGE"
echo "========================"
echo ""

echo -n "Output:  "
./cipher -e 3 THISISASECRETMESSAGE
exitCodeTest4=$?

if [[ $(./cipher -e 3 THISISASECRETMESSAGE) != "TIETSHSSSCEMSAEIAREG" ]]
then
        echo ""
        echo "***"
        echo "ERROR!! cipher not printing properly..."
        echo "EXPECTED: TIETSHSSSCEMSAEIAREG"
        ((numOfErrors++))
        echo "***"
        echo""
fi

echo "Exit code from test 4: $exitCodeTest4"

if [[ $(./cipher -e 3 THISISASECRETMESSAGE) == "TIETSHSSSCEMSAEIAREG" ]]
then
	echo ""
	echo "~test passed~"
fi

echo ""

# CODE FOR TEST 5

echo ""
echo "=======<<TEST 5>>======="
echo "For valid arguments, the program should produce the encrypted message"
echo "./cipher -e 3 "
echo "========================"
echo ""

echo -n "Output:  "
./cipher -e 3 ""
exitCodeTest5=$?

if [[ $(./cipher -e 3 "") != "" ]]
then
        echo ""
        echo "***"
        echo "ERROR!! cipher not printing properly..."
        echo "EXPECTED: "
        ((numOfErrors++))
        echo "***"
        echo""
fi

echo "Exit code from test 5: $exitCodeTest5"

if [[ $(./cipher -e 3 "") == "" ]]
then
        echo ""
        echo "~test passed~"
fi

echo ""

# CODE FOR TEST 6
echo ""
echo "=======<<TEST 6>>======="
echo "For valid arguments, If the program is invoked with -d option"
echo "./cipher -d 3 TIETSHSSSCEMSAEIAREG"
echo "========================"
echo ""

echo -n "Output:  "
./cipher -d 3 TIETSHSSSCEMSAEIAREG
exitCodeTest6=$?

if [[ $(./cipher -d 3 TIETSHSSSCEMSAEIAREG) != "" ]]
then
        echo ""
        echo "***"
        echo "ERROR!! cipher not printing properly..."
        echo "EXPECTED: "
        ((numOfErrors++))
        echo "***"
        echo""
fi

echo "Exit code from test 6: $exitCodeTest6"

if [[ $(./cipher -d 3 TIETSHSSSCEMSAEIAREG) == "" ]]
then
        echo ""
        echo "~test passed~"
fi

echo ""

# at the end of the script, I notify the user how many tests they passed
# using the numOfErrors variable and subtracting it from the total number of tests
echo "========================"
echo ""
numTestsPassed=$((6-numOfErrors))
echo "number of tests passed: $numTestsPassed of 6"
echo ""
echo "========================"

