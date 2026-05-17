#!/bin/bash

# 1. Compile the engine with max speed flags
echo "🚀 Compiling Hackathon Squad Engine..."
g++ -std=c++17 -O3 solution.cpp -o solution

echo "🧠 Running Automated Test Suite..."
echo "--------------------------------------------------"

# 2. Loop through every input file IN THE test_suite FOLDER
for input_file in test_suite/input_*.txt; do
    
    # Dynamically find the matching output file (replaces 'input' with 'output')
    expected_file="${input_file/input/output}"

    # Check if the matching expected output exists
    if [[ -f "$expected_file" ]]; then
        
        # Run our engine and save the output to a temporary file
        ./solution < "$input_file" > my_answer.txt
        
        # Extract the score (Line 1) from both files
        # Because MWIS is an approximation problem, your exact team of coders (Line 2) might differ from the official answer, but what really matters is the Score (Line 1). [cite: 2311]
        my_score=$(head -n 1 my_answer.txt)
        expected_score=$(head -n 1 "$expected_file")

        # Since this is an approximation, we pass if our score is >= the official target
        if [[ "$my_score" -ge "$expected_score" ]]; then
            echo -e "✅ PASS | $input_file"
            echo "   Our Score: $my_score | Target: $expected_score"
        else
            echo -e "❌ FAIL | $input_file"
            echo "   Our Score: $my_score | Target: $expected_score"
        fi
    else
        echo "⚠️ Missing expected output for $input_file"
    fi
done

# 3. Clean up the temporary file
rm -f my_answer.txt
echo "--------------------------------------------------"
echo "🏆 Testing Complete!"