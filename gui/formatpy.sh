#!/bin/bash

# Beginning
hasErrors=false

successColor="\e[32m"  # Green
errorColor="\e[31m"    # Red
resetColor="\e[0m"

# Function to run a command and check its exit status
run_command() {
    local cmd="$1"
    local description="$2"
    
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "$description"
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    
    eval "$cmd"
    if [ $? -ne 0 ]; then
        echo -e "\n${errorColor}$description found issues.${resetColor}"
        hasErrors=true
    else
        echo -e "\n${successColor}$description completed successfully.${resetColor}"
    fi
    echo ""
}

# Run the tools
run_command "isort ." "Sorting imports with isort"
run_command "black --color ." "Formatting code with Black"
run_command "mypy ." "Running type checks with mypy"
run_command "flake8" "Linting code with flake8"
run_command "pydocstyle" "Checking docstrings with pydocstyle"

# Final message
if [ "$hasErrors" = true ]; then
    echo -e "\n${errorColor}One or more tools have found errors in the code...${resetColor}"
    echo -e "${errorColor}Please fix the errors in the code before pushing to version control.${resetColor}"
else
    echo -e "\n${successColor}All tools have executed and no issues were found.${resetColor}"
fi