#! /bin/sh
echo Pour les test/syataxe/KO
for i in Tests/Syntaxe/KO/*.c;
do
	echo $i
	./minicc $i -s
	echo "\n"
done