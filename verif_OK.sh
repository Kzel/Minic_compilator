#! /bin/sh
echo Pour les test/Verif/OK
for i in Tests/Verif/OK/*.c;
do
	echo $i
	./minicc $i -v
done