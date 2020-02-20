This program is a UNIX shell program. It can be compiled by running "make" and then ran by running ./m

This program implements all unix commands using execvp(). These can be tested out by running the program
This program has a history feature, which can be tested out by using !!. This echoes out the most recent command and runs it instantly
Input and output streams can be redirected in this program. An example would be running "ls > out.txt". This would display the contents of the ls command in out.txt

This program supports simple pipelining. an example would be running "ls | less" pipelines the input of ls to be outputted by the less command
