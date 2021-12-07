#! /bin/sh
echo Pour les test/Verif/KO
for i in Tests/Verif/KO/*.c;
do
	echo $i
	./minicc $i -v
	echo "\n"
done