@echo off
SetLocal EnableDelayedExpansion
(set PATH=D:\Qt\qt5\qtbase\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=D:\Qt\qt5\qtbase\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=D:\Qt\qt5\qtbase\plugins
)
D:\Qt\qt5\qtbase\bin\uic.exe %*
EndLocal
