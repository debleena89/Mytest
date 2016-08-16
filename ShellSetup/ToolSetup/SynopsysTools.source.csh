setenv SYNOPSYS_HOME /home/acmu/tools/Synopsys
setenv LM_LICENSE_FILE ${LM_LICENSE_FILE}:27000@vlsilab-9
setenv SNPSLMD_LICENSE_FILE 27000@vlsilab-9

setenv SCL_HOME $SYNOPSYS_HOME/SCL
set path = ($SCL_HOME/amd64/bin $path)

setenv INSTALLER_HOME $SYNOPSYS_HOME/installer/3.1
set path = ($INSTALLER_HOME $path)

setenv VCS_HOME $SYNOPSYS_HOME/vcs
set path = ($VCS_HOME/amd64/bin $path)
alias vcs 'vcs -full64'

#setenv VCS_MX_HOME $SYNOPSYS_HOME/vcsMx
#set path = ($VCS_MX_HOME/amd64/bin $path)

setenv MG_HOME $SYNOPSYS_HOME/magellan
set path = ($MG_HOME/amd64/ctg/bin $path)

setenv DC_HOME $SYNOPSYS_HOME/dc
set path = ($DC_HOME/amd64/syn/bin $DC_HOME/amd64/bin $path)
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/usr/lib

setenv TMAX_HOME $SYNOPSYS_HOME/tmax
setenv SYNOPSYS $SYNOPSYS_HOME/dc
set path = ($TMAX_HOME/amd64/syn/bin $SYNOPSYS/bin $TMAX_HOME/amd64/bin $path)
