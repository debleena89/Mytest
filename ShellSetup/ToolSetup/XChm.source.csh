if ($OS == "Linux") then
    setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:$UtilHome/wxGTK-2.8.7/lib
    setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:$UtilHome/chmlib-0.39/lib
    alias xchm $UtilHome/xchm-1.13/bin/xchm
endif
