#!/bin/tcsh -f

# Command line parsing
if ($#argv != 1) then
    echo Error: No tool name specified ...
    echo "Usage: tool_setup <tool name>"
    exit 1
endif
set tool = $1
set toolSetupFile = ~/ShellSetup/ToolSetup/$tool.source.csh

# Check if you are creating new tool setup
if (! -r $toolSetupFile) then
    echo -n "Create new tool setup for $tool? (y/n): "
    set option = $<
    set option = `echo $option | cut -c 1`
    if ($option != Y && $option != y) exit 1
endif

# Edit the tool setup file
vi $toolSetupFile

# Check for skip file
if (-r $toolSetupFile.skipped) then
    echo Tool setup for $tool is skipped ...
    echo -n "Unskip the setup? (y/n) "
    set option = $<
    set option = `echo $option | cut -c 1`
    if ($option == Y || $option == y) then
        rm -f $toolSetupFile.skipped
        echo $tool is enabled.
    endif
endif

exit 0
