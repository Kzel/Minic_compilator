#! /bin/sh
echo Pour les test/syataxe/OK
for i in Tests/Syntaxe/OK/*.c;
do
	echo $i
	./minicc $i -s
done