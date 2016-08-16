#!/bin/csh -f
# ShellSetup Installer

# Header
echo ---- ShellSetup Installer ----
echo Running on: `uname -sn`

# Set parameters
if ($#argv == 1) then
    set display = $1
else
    set display = `uname -n`:0.0
endif
echo DISPLAY: $display

# Clean
echo Cleaning old installation ...
\rm -f ~/.cshrc
cd `dirname $0`
if (`\pwd` != ~/ShellSetup) then
    \rm -rf ~/ShellSetup
endif

# Installation
echo Installing afresh ...
if (! -e ~/ShellSetup && `\pwd` != ~/ShellSetup) \
    cp -rp `dirname $0` ~/ShellSetup
cd
if (! -e ~/.cshrc) ln -s ~/ShellSetup/Cshrc.link.csh ~/.cshrc
if (! -e ~/.rhosts) cp ~/ShellSetup/ToolSetup/RHosts.link ~/.rhosts
\grep `uname -n` ~/.rhosts > /dev/null
if ($status) echo `uname -n` >> ~/.rhosts
if (! -e ~/.vimrc) ln -s ~/ShellSetup/ToolSetup/VimRc.link ~/.vimrc
if (! -e ~/ShellSetup/Setup.`whoami`.csh) then
    sed -e "s/#DISPLAY#/$display/g" ~/ShellSetup/Setup.whoami.csh > \
        ~/ShellSetup/Setup.`whoami`.csh
endif

# Exit
echo Installation complete. Source "~/.cshrc"
exit 0
