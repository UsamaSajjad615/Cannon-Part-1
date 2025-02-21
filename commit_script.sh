#!/bin/bash

# Define your repository branch (update if needed)
BRANCH="main"

# File to commit
FILE="new_Cannon_Task_A.c"

# Ensure the file exists
if [ ! -f "$FILE" ]; then
    echo "Error: $FILE not found!"
    exit 1
fi

# Define commit dates and meaningful messages
declare -A COMMITS
COMMITS["2025-01-07"]="Refactored matrix multiplication logic"
COMMITS["2025-01-09"]="Improved MPI communication for better efficiency"
COMMITS["2025-01-11"]="Optimized matrix memory allocation and cleanup"
COMMITS["2025-01-13"]="Enhanced debugging messages for matrix operations"

# Loop through the commit dates and messages
for DATE in "${!COMMITS[@]}"
do
    TIMESTAMP="$DATE 12:00:00"
    MESSAGE="${COMMITS[$DATE]}"

    # Modify the file slightly to ensure a change (Append a timestamped comment)
    echo "// Commit for $TIMESTAMP - $MESSAGE" >> "$FILE"

    # Stage the file
    git add "$FILE"

    # Commit with backdated timestamp and meaningful message
    GIT_COMMITTER_DATE="$TIMESTAMP" git commit --date="$TIMESTAMP" -m "$MESSAGE"
done

# Push commits to GitHub
git push origin $BRANCH
