#!/bin/bash

# ==============================================================================
# Advent of Code Input & Puzzle Downloader
#
# Usage:
#   ./get_input.sh <day>          (Downloads for `DEFAULT_YEAR`)
#   ./get_input.sh <year> <day>   (Downloads for specific year)
#
# Requirements:
#   1. A file named .env in the root directory containing: SESSION=your_cookie
#   2. python3 installed (for HTML -> Markdown conversion)
# ==============================================================================

# --- Configuration ---
DEFAULT_YEAR=2025
# ---------------------

# 1. Argument Parsing
if [ "$#" -eq 1 ]; then
    YEAR=$DEFAULT_YEAR
    DAY=$1
elif [ "$#" -eq 2 ]; then
    YEAR=$1
    DAY=$2
else
    echo "Usage:"
    echo "  $0 <day>         (Downloads for $DEFAULT_YEAR)"
    echo "  $0 <year> <day>  (Downloads for specific year)"
    exit 1
fi

# 2. Session Setup
if [ -f .env ]; then
    export $(grep -v '^#' .env | xargs)
fi

if [ -z "$SESSION" ]; then
    echo "Error: SESSION cookie not found in .env"
    echo "Please create a .env file with your session cookie."
    exit 1
fi

# 3. Path Setup
BASE_DIR="aoc-$YEAR"
INPUT_DIR="$BASE_DIR/input"
PUZZLE_DIR="$BASE_DIR/puzzles"

PADDED_DAY=$(printf "%02d" $DAY)
URL_DAY=$(echo $DAY | sed 's/^0*//') # Remove leading zeros for URL

INPUT_FILE="$INPUT_DIR/day${PADDED_DAY}.txt"
PUZZLE_FILE="$PUZZLE_DIR/day${PADDED_DAY}.md"

# 4. Ensure Directories Exist
mkdir -p "$INPUT_DIR"
mkdir -p "$PUZZLE_DIR"

# ==============================================================================
# TASK 1: DOWNLOAD INPUT
# ==============================================================================

if [ -f "$INPUT_FILE" ]; then
    echo "Input for $YEAR Day $DAY already exists."
else
    echo "Downloading Input..."
    HTTP_CODE=$(curl -s -w "%{http_code}" --cookie "session=$SESSION" "https://adventofcode.com/$YEAR/day/$URL_DAY/input" -o "$INPUT_FILE")

    if [ "$HTTP_CODE" -ne 200 ]; then
        echo "Error: Input download failed (HTTP $HTTP_CODE)."
        echo "Check your session cookie or if the puzzle is unlocked."
        rm -f "$INPUT_FILE"
    else
        echo "Input saved to $INPUT_FILE"
    fi
fi

# ==============================================================================
# TASK 2: DOWNLOAD & CONVERT PUZZLE DESCRIPTION
# ==============================================================================

echo "Fetching Puzzle Description..."

# Fetch the raw HTML
HTML_CONTENT=$(curl -s --cookie "session=$SESSION" "https://adventofcode.com/$YEAR/day/$URL_DAY")

# Use embedded Python 3 to parse HTML -> Markdown
echo "$HTML_CONTENT" | python3 -c "
import sys, re

html = sys.stdin.read()

# Find all articles (Part 1 and Part 2 if available)
articles = re.findall(r'<article class=\"day-desc\">(.*?)</article>', html, re.DOTALL)

if not articles:
    print(\"Error: Could not find puzzle content. Check session cookie or day number.\")
    sys.exit(1)

markdown = \"\"

for art in articles:
    # Convert <h2> to Markdown headers
    art = re.sub(r'<h2.*?>(.*?)</h2>', r'## \1\n', art)
    
    # Convert <p> to double newlines (paragraphs)
    art = re.sub(r'<p>(.*?)</p>', r'\1\n\n', art, flags=re.DOTALL)
    
    # Convert Code Blocks <pre><code>...</code></pre>
    art = re.sub(r'<pre><code>(.*?)</code></pre>', r'\n\`\`\`\n\1\n\`\`\`\n', art, flags=re.DOTALL)
    
    # Convert Inline Code <code>...</code>
    art = re.sub(r'<code>(.*?)</code>', r'\`\1\`', art)
    
    # Convert Emphasis <em>...</em> to **bold**
    art = re.sub(r'<em.*?>(.*?)</em>', r'**\1**', art)
    
    # Convert Links <a href=...>...</a>
    art = re.sub(r'<a href=\"(.*?)\".*?>(.*?)</a>', r'[\2](\1)', art)
    
    # Convert Lists
    art = re.sub(r'<ul>', r'', art)
    art = re.sub(r'</ul>', r'', art)
    art = re.sub(r'<li>(.*?)</li>', r'- \1', art)

    # Clean up remaining tags
    art = re.sub(r'<.*?>', '', art)
    
    # Decode HTML entities
    art = art.replace('&lt;', '<').replace('&gt;', '>').replace('&amp;', '&').replace('&quot;', '\"')
    
    # Normalize newlines (max 2 consecutive newlines)
    art = re.sub(r'\n{3,}', '\n\n', art)
    
    markdown += art + \"\n---\n\n\"

print(markdown.strip())
" >"$PUZZLE_FILE"

# Check if the python script actually wrote content
if [ -s "$PUZZLE_FILE" ]; then
    echo "Puzzle description saved to $PUZZLE_FILE"
else
    echo "Error: Failed to parse puzzle description."
    rm -f "$PUZZLE_FILE"
fi
