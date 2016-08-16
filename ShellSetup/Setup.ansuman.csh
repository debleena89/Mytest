# Personal setup

# Display
# set HOSTMC = host_machine
setenv | grep DISPLAY
if ($status != 0) then
    if ($?HOSTMC) then
        echo Setting the display to $HOSTMC ...
        setenv DISPLAY ${HOSTMC}:0.0
    else
        setenv DISPLAY acmuws4:0.0
        echo Setting the display to $DISPLAY ...
    endif
endif
