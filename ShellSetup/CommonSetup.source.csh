# Common C shell setups

# OS variable
if (! $?OS) setenv OS `uname -s`
if (`echo $OS | cut -c 1-9` == CYGWIN_NT || $OS == Windows_NT) setenv OS Cygwin

# Setting file creation mask
umask 022

# Enables file name completion with the ESC character
set filec

# If ignoreeof is set, you can't use CTRL-D to terminate shell
set ignoreeof

# History related setups
set history = 200
set savehist = 20

# Setting backspace key
stty erase 

if ($OS != "Cygwin") then
    # Set coredumpsize to maximum
    limit coredumpsize 0
    # Set stack size
    limit stacksize 33000
endif

# Set prompt
set prompt="[%B%M%b,%n:\!] "
## set prompt="[`hostname`,`whoami`:\!] "

# Others
set symlinks = ignore
set matchbeep = never
set autolist
set autologout = 0
set autoexpand
set histdup = prev
set ignoreeof = 2
set fignore = (.o)

# Environment variable setups
set path = (. /usr/local/bin /bin /usr/bin /sbin /usr/sbin /etc /usr/etc)
setenv MANPATH /usr/man:/usr/local/man
setenv LD_LIBRARY_PATH /lib:/usr/lib
setenv LM_LICENSE_FILE
setenv EDITOR vim
if ($OS == "Linux" || $OS == "Cygwin") then
    set path = ($path /usr/X11R6/bin)
else if ($OS == "SunOS") then
    setenv GNU_PATH /usr/local
    setenv OPENWINHOME /usr/openwin
    set path=($path $GNU_PATH/bin /usr/ccs/bin /usr/ucb \
            /usr/5bin $OPENWINHOME/bin)
    setenv MANPATH ${MANPATH}:$OPENWINHOME/man
    setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:$OPENWINHOME/lib:$OPENWINHOME/lib/X11
else
    echo "ERROR: Unknown OS "`uname -a`
    exit 1
endif

# Xterm setup
if ($TERM == "xterm") then
    alias setxprompt 'echo -n ]0\;$HOST : $cwd'
else
    alias setxprompt 'echo $HOST : $cwd'
endif
setxprompt

# Source common aliases
source $ssShellSetupDir/AliasSource.csh

# Personal setup
set ssSetupFile = $ssShellSetupDir/Setup.`whoami`.csh
if (-r $ssSetupFile) source $ssSetupFile

# Sourcing other tool setups
set ssToolSetupDir = $ssShellSetupDir/ToolSetup
if (-r $ssToolSetupDir) then
    if (-r $ssToolSetupDir/env.csh) then
        if $ssDebug echo Sourcing $ssToolSetupDir/env.csh
        source $ssToolSetupDir/env.csh
    endif
    foreach ssSetupFile ($ssToolSetupDir/*.$ssSetupExt)
        if (! -r $ssSetupFile.skipped) then
            if $ssDebug echo Sourcing $ssSetupFile ...
            source $ssSetupFile
        endif
    end
    Alias tools '\\
        foreach ssSetupFile ($ssToolSetupDir/*.$ssSetupExt) \
            if (-r $ssSetupFile.skipped) then \
                set ssSetupFile = "$ssSetupFile [skipped]" \
            endif \
            echo "$ssSetupFile" | sed -e "s/.*\///g" -e "s/\.$ssSetupExt//g" \
        end' (shows tools available and skipped)
    Alias skip '\\
        foreach ssSetupFile (\!*) \
            if (-r $ssToolSetupDir/$ssSetupFile.$ssSetupExt) \\
                touch $ssToolSetupDir/$ssSetupFile.$ssSetupExt.skipped \
        end \
        soc' (skips given tool setups)
    Alias unskip '\\
        foreach ssSetupFile (\!*) \
            if (-r $ssToolSetupDir/$ssSetupFile.$ssSetupExt.skipped) \\
                \rm -f $ssToolSetupDir/$ssSetupFile.$ssSetupExt.skipped; \
        end \
        soc' (restores given tool setups)
    Alias skipall '\\
        foreach ssSetupFile ($ssToolSetupDir/*.$ssSetupExt) \
            touch $ssSetupFile.skipped \
        end \
        soc' (skips all tool setups)
    Alias unskipall '\\
        foreach ssSetupSkipFile ($ssToolSetupDir/*.$ssSetupExt.skipped) \
            \rm -f $ssSetupSkipFile \
        end \
        soc' (restores all tool setups)
endif

