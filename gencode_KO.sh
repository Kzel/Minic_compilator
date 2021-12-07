#! /bin/sh
echo Pour les test/Gencode/KO
for i in Tests/Gencode/KO/*.c; 
do
	echo $i
	./minicc $i -o $i.s
	echo "\n"
done