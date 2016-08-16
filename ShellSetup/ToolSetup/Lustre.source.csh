if ($OS == "Linux") then
    setenv LUSTRE_INSTALL $UtilHome/lustre-v4-II.j-linux
    set path= ($path $LUSTRE_INSTALL/bin)
    setenv MANPATH ${MANPATH}:${LUSTRE_INSTALL}/man
endif
