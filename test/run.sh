#! /bin/sh
# Checking parameters
if [ $# -ne 1 ]
then test_directory='test_cases'
else test_directory=$1
fi

if [ ! -d $test_directory ]
then echo "ERROR : test directory \"${test_directory}\" is missing"
    return 3
fi

# Prepare output directory
output_directory="test_outputs"
if [ -d $output_directory ]
then \rm -rf $output_directory 
fi
mkdir $output_directory

summary_file="test.log"
if [ -e $summary_file ]
then \rm -rf $summary_file
fi

problem=0

## List test cases
for test in `ls $test_directory`
do
  directory=${test_directory}'/'${test}
  echo "Test : ${test}"

  ## Check input file existence
  input_file=${directory}'/'input.xml
  if [ ! -e $input_file ]
  then echo "ERROR : File $input_file is missing"
      return 3
  fi

  ## Check ref output file existence
  ref_file=${directory}'/'output.ref
  if [ ! -e $ref_file ]
  then echo "ERROR : File $ref_file is missing"
      return 3
  fi

  ## Remove output files
  output_file='status.log'
  if [ -e $output_file ]
  then \rm $output_file
  fi

  ## Remove log files
  log_file='run.log'
  if [ -e $log_file ]
  then \rm $log_file
  fi

  mkdir ${output_directory}/${test}

  ## Run tests
  ./bin/dicoplus.exe $input_file --refresh_delay=0  > $log_file 2>&1

  ## Check output file existence
  if [ ! -e $output_file ]
  then echo "ERROR : Output file is missing"
      return 3
  fi
  diff_file=diff.log
  diff $output_file $ref_file > $diff_file  2>&1
  if [ $? -eq 0 ]
  then echo " -> ${test} PASSED" | tee -a $summary_file
  else echo " -> ${test} FAILED" | tee -a $summary_file
      problem=1
  fi

  mv $output_file ${output_directory}/${test}/${output_file}
  mv $log_file ${output_directory}/${test}/${log_file}
  mv $diff_file ${output_directory}/${test}/${diff_file}
done
# Print global result
if [ $problem -eq 0 ]
then echo "TEST SUCCESSFULL" | tee -a $summary_file
else echo "ERROR : Some tests failed" | tee -a $summary_file
fi
#EOF
