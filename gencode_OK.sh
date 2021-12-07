#! /bin/sh
echo Pour les test/Gencode/OK
for i in Tests/Gencode/OK/*.c; 
do
	echo $i
	./minicc $i -o $i.s
done