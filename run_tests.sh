#!/bin/bash

# Define the device path and the base name for the data files
DEVICE="/dev/sort"
DATA_FILE_PREFIX="data/data_100_"
RESULTS_FILE="testkernel_threads_100_results.txt"

# Clear previous results file
echo "Performance Test Results" > $RESULTS_FILE

# Loop over the range of file indices
for i in $(seq 1 10); do
    # Construct the filename
    DATA_FILE="${DATA_FILE_PREFIX}${i}.txt"
    
    # Run perf stat and append output to results file
    echo "Running test for $DATA_FILE" >> $RESULTS_FILE
    sudo perf stat --repeat 5 -e cache-misses,cache-references,instructions,cycles ./user "$DEVICE" "$DATA_FILE" &>> $RESULTS_FILE
    
    # Check the exit status of the program
    if [ $? -ne 0 ]; then
        echo "Test failed for $DATA_FILE" >> $RESULTS_FILE
        exit 1
    fi

    echo "" >> $RESULTS_FILE
done

echo "All tests completed successfully." >> $RESULTS_FILE
