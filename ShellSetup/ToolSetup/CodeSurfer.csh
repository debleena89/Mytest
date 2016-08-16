setenv CSURF /home/ansuman/tools/codesurfer-2.2p0
set path = ($path $CSURF/csurf/bin) 
setenv LM_LICENSE_FILE /home/ansuman/tools/codesurfer-2.2p0/license.dat:$LM_LICENSE_FILE
$CSURF/csurf/bin/lmgrd -c $CSURF/license.dat
