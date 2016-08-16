echo 'hi'
grep '#' test.c > outfile.c
grep 'void' test.c >> outfile.c

echo '{' >> outfile.c;

grep -w 'int' test.c >> outfile.c


 l=`cat outfile.c| wc -l`
echo $l
