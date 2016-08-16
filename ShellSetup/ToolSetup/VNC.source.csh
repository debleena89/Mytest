if ($OS == "SunOS" ) then
    set path = ( $path $UtilHome/vnc/vnc-4.0-sparc_solaris_2.5 )
endif
Alias vncstart 'vncserver -geometry 1600x900 -depth 24' \
          (starts vncserver in available display)
