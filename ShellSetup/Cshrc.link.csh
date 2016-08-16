# .cshrc file to be linked into home

# Setups
set ssDebug = 0
set ssShellSetupDir = $HOME/ShellSetup
set ssSetupExt = source.csh
foreach ssSetupFile ($ssShellSetupDir/*.$ssSetupExt)
    if $ssDebug echo Sourcing $ssSetupFile ...
    source $ssSetupFile
end
