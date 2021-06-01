#! /bin/sh
echo Test Syntaxe KO
for i in Tests/Syntaxe/OK/*.c; do
	./minicc $i -s
done