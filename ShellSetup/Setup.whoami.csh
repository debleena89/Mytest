# Personal setup

# Display
# set HOSTMC = host_machine
setenv | grep DISPLAY
if ($status != 0) then
    if ($?HOSTMC) then
        echo Setting the display to $HOSTMC ...
        setenv DISPLAY ${HOSTMC}:0.0
    else
        setenv DISPLAY #DISPLAY#
        echo Setting the display to $DISPLAY ...
    endif
endif

# CVS Root
# setenv CVSROOT /u/amitroy/CvsRoot

# Personal aliases
# alias cdt 'cd ~/Temp/Others'
# alias ssbackup 'pu ~; tar -cvz --exclude="ShellSetup/Setup.`whoami`\.csh" --exclude="CVS" --exclude="*.source.csh.skipped" -f ~/MySoftwareArea/Backups/EnvSetup/Shell/ShellSetup.tgz ShellSetup; po'

# Carbon transactor - dummy CARBON_HOME setup
# alias nocarbon 'setenv CARBON_HOME `\pwd | sed -e "s:/\(Carbon[^ /]*Xtor\).*:/\1/CarbonHomeTemplate:"`; echo \$CARBON_HOME=$CARBON_HOME; setenv SYSTEMC_HOME /software/download2/amitroy/utils/systemc/2.1.v1; echo \$SYSTEMC_HOME=$SYSTEMC_HOME'
# Carbon USB Transactor related aliases
# alias makehl8  'make clean; make VHM=no HostIntf=LINK UtmiBusWidth=8 '
# alias makehl16 'make clean; make VHM=no HostIntf=LINK UtmiBusWidth=16'
# alias makehp8  'make clean; make VHM=no HostIntf=PHY  UtmiBusWidth=8 '
# alias makehp16 'make clean; make VHM=no HostIntf=PHY  UtmiBusWidth=16'
