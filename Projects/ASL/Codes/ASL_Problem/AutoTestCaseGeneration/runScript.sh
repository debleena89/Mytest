echo 'hi'
#grep '#' test.c > outfile.c
#grep 'void' test.c >> outfile.c

#echo '{' >> outfile.c

#grep -w 'int' test.c >> outfile.c

#z=`cat outfile.c|wc -l`

foreach line in test.c
  echo $line
end


echo '__CPROVER_assume(a = 1);' >> outfile.c
echo '__CPROVER_assume(b = 2);' >> outfile.c
echo '__CPROVER_assume(c = 3);' >> outfile.c
echo '__CPROVER_assume(d = 4);' >> outfile.c




cbmc test.c>cbmclog


