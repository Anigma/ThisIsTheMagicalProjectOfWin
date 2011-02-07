#!/bin/sh

make -k
passed=0
failed=0
xcpassed=0
xcfailed=0

echo "Test ucontext parsing"
./parseUcontext > checkUcontext.grade.out
cat checkUcontext.grade.out | awk -f checkUcontext.awk
if cat checkUcontext.grade.out | awk -f checkUcontext.awk | grep "Might-be-OK-count: 7" > /dev/null
then
  passed=`expr 1 + $passed`
  echo Basic parsing of context looks OK
else
  failed=`expr 1 + $failed`
  echo Basic parsing of context looks BAD
fi

echo "Test nonpreemptive"
./doTest > doTest.grade.out
if (cat doTest.grade.out; echo XYZ_BREAK_TOKEN_XYZ; cat doTest.expected) | awk -f checkAll.awk | grep "checkAll-OK" > /dev/null
then
   passed=`expr 1 + $passed`
   echo OK: Basic non-preemptive looks OK
else
   failed=`expr 1 + $failed`
   (cat doTest.grade.out; echo XYZ_BREAK_TOKEN_XYZ; cat doTest.expected) | awk -f checkAll.awk
   echo Basic non-preemptive looks BAD
fi

echo "Test (XC) preemptive (this will take 10-15 seconds)"
./doTest2 > doTest2.grade.out
if (cat doTest2.grade.out; echo XYZ_BREAK_TOKEN_XYZ; cat doTest2.expected) | awk -f checkAll.awk | grep "checkAll-OK" > /dev/null
then
   xcpassed=`expr 1 + $xcpassed`
   echo OK: Basic tests still look OK
else
   xcfailed=`expr 1 + $xcfailed`
   (cat doTest2.grade.out; echo XYZ_BREAK_TOKEN_XYZ; cat doTest2.expected) | awk -f checkAll.awk
   echo Basic non-preemptive looks BAD
fi

# Should make it around at least 5 times...
# I usually get around 22 times in my tests.
# But if this fails, be sure to check it manually.
# Maybe the machine is just slow.
# Try increasing duration of test.
if cat doTest2.grade.out | grep "9 passes potato for 5-st/nd/rd time" > /dev/null
then
   xcpassed=`expr 1 + $xcpassed`
   echo 9-5 OK
else
   xcfailed=`expr 1 + $xcfailed`
   echo 9-5 MAY BE WRONG but check doTest2.grade.out manually it is timing sensitive 
fi

echo "Output passed: $passed  Output failed: $failed"
echo "Output xcpassed: $xcpassed  Output xcfailed: $xcfailed"

