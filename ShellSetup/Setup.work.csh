# Personal setup

# Display
# set HOSTMC = host_machine
setenv | grep DISPLAY
if ($status != 0) then
    if ($?HOSTMC) then
        echo Setting the display to $HOSTMC ...
        setenv DISPLAY ${HOSTMC}:0.0
    else
        setenv DISPLAY 0.0
        echo Setting the display to $DISPLAY ...
    endif
endif

# CVS Root
setenv CVSROOT :pserver:ansuman@192.192.164.79:/software/CvsRoot

# Personal aliases
# alias cdt 'cd ~/Temp/Others'
# alias ssbackup 'pu ~; tar -cvz --exclude="ShellSetup/Setup.`whoami`\.csh" --exclude="CVS" --exclude="*.source.csh.skipped" -f ~/MySoftwareArea/Backups/EnvSetup/Shell/ShellSetup.tgz ShellSetup; po'

# Carbon transactor - dummy CARBON_HOME setup
alias nocarbon 'setenv CARBON_HOME `\pwd | sed -e "s:/\(Carbon[^ /]*Xtor\).*:/\1/CarbonHomeTemplate:"`; echo \$CARBON_HOME=$CARBON_HOME;'
