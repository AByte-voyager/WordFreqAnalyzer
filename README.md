# Word Frequency Analyzer

## Overview
This project is a **Word Frequency Analyzer** that processes text files to compute the frequency of words. It consists of a **C program** to analyze word frequencies, a **Bash script** to automate the execution, an **SQL database** to store the results, and a **Python script** to query and display the word frequencies stored in the database.

## Features
- **C program** to read text from files, count word frequencies, and save the results in an SQL format.
- **Bash script** to automate the process of compiling the C program and executing it for multiple files.
- **SQL database** to store word frequencies and associate them with the filenames.
- **Python script** to query the SQL database and display word frequencies in a readable format.

## Project Structure
- `Word Frequency Analyzer`
  - `word_freq.c`          : C program to analyze word frequencies
  - `freq_count`           : Bash script to automate C program execution
  - `freq_table.sql`       : SQL script to create the word frequency table
  - `word_freq.py`         : Python script to query and display results

## Requirements
- **C Compiler** (e.g., `gcc`)
- **SQLite3** for SQL database handling
- **Python3** with SQLite support

## Usage
The Bash script automates the entire process. To run the script, use:
./freq_count <file1> <file2> ... <fileN>
