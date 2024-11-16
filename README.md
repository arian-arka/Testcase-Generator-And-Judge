# A test case generator and judge

**Introduction**

This program can generate test case from a file or user input for a given executable
Also it can judge another executable with the correct inputs/outputs



## How it works
First you need to develop your code 
Then every program has its own inputs and prints (inputs-stdin-... and prints-stdout-... are abstracts, it can be any executable file or interpreter programming language)
This program creates 2 temp file for each test case. one for input and one for out put and runs your executable with the command written below
for example for a python program
````bash
python3 main.py < tempIn.txt > tempOut.txt
````
