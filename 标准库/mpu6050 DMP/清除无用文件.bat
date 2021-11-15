del *.bak /s
del *.ddk /s
del *.edk /s
del *.lst /s
del *.lnp /s
del *.mpf /s
del *.mpj /s
del *.obj /s
del *.omf /s
del *.uvopt /s       ren 旧版本的删除
del *.saved_uv4 /s   ren 旧版本的删除
ren del *.uvoptx /s  ren 不允许删除JLINK的设置，新版本是这个
del *.plg /s
del *.rpt /s
del *.tmp /s
del *.__i /s
del *.crf /s
del *.o /s
del *.d /s
del *.axf /s
del *.tra /s
del *.dep /s           
del JLinkLog.txt /s
del JLinkSettings.ini /s
del *.iex /s
del *.htm /s
del *.sct /s
del *.map /s
del *.scvd /s

del *."%username%" /s      ren删除登录本机的账户缓存文件，有%%就用ren
del *."%computername%" /s  ren删除用户名缓存文件，有%%就用ren

rd /q /s DebugConfig
rd /q /s RTE

exit
