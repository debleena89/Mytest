# Aliases

# UnAlias all
unalias -a

# General Aliases
alias Alias 'alias \!:1 \!:2; set HELP\!:1 = ": \!:3*"'
Alias help 'echo "-alias--:-----------------description------------------"; \
          set | grep HELP | sort | sed -e "s/HELP//g" -e "s/[()]//g"' \
          (shows ShellSetup help)

# System setup related Aliases
Alias soc 'source ~/.cshrc' (sources ~/.cshrc file)
Alias s 'vi $ssShellSetupDir/Setup.`whoami`.csh' (opens your setup file)

# Directory and file access Aliases
Alias cd 'chdir \!*; setxprompt; echo $cwd >! ~/.cwd' (change directory)
Alias pu 'pushd \!*; setxprompt; echo $cwd >! ~/.cwd' (push directory)
Alias po 'popd \!*; setxprompt; echo $cwd >! ~/.cwd' (pop directory)
Alias ccd 'cd `cat ~/.cwd`' (change to most recent visited directory)
Alias md mkdir (make directory)
Alias rm 'rm -i' (delete file or directory)
Alias mv 'mv -i' (move file or directory)
Alias l 'ls -al' (list file or directory)
Alias ll "ls -l \!* | more" (list file or directory)
Alias c clear (clear screen)
Alias h history (shows history of commands)
Alias q exit (exit or quit)

# Xterm Aliases
alias xterm_default '\\
        set xcolor = white; \\
        set xbgcolor = black; \\
        set xgeom = 80x40'
alias xterm_setup '\\
        if (\!:0 != \!:$) \\
            set xcolor = \!:$'
alias xterm_Alias 'xterm -bg $xbgcolor -fg $xcolor -geom $xgeom -sl 1000 -sb \\
            -fn '"'"'-b&h-lucidatypewriter-medium-r-normal-sans-12-*'"'"' \\
            -bd $xcolor -cr $xcolor -ms $xcolor &'
Alias x 'xterm_default; xterm_setup \!*; xterm_Alias' \
          (opens new xterm with given text color)
Alias sx 'xterm_default; set xgeom = 80x20; xterm_setup \!*; xterm_Alias' \
          (opens small xterm with given text color)
Alias rx 'xterm_default; set xcolor = black; set xbgcolor = white; \\
          xterm_setup \!*; xterm_Alias' (opens new xterm in reverse background)
Alias X 'x \!*; exit' (opens new xterm and exit current xterm)

# Other common Aliases
Alias date 'date +"%d %B, %Y [%H:%M:%S]"' (shows date)
Alias cstat 'cvs status \!* | grep Status | grep -v Up-to-date' \
          (shows CVS status)
Alias ds 'echo "Disk Status:"; echo "==========="; \df -k .' (shows disk status)
Alias gcc_path 'set path = (\!:1/bin $path); \\
          setenv LD_LIBRARY_PATH \!:1/lib:$LD_LIBRARY_PATH' \
          (sets GCC path to given directory)
Alias ph 'echo ""; grep -i "\!*" ~janet/ext.txt | more; echo ""' \
          (searches telephone)
Alias tel ph (searches telephone)
Alias ips 'ypcat hosts' (shows ip address vs machine names)
alias mcm 'make clean; make;' 
alias mc 'make clean' 
alias backup 'rsync -azv /home/ansuman/ ansuman@192.168.72.26:~/'
