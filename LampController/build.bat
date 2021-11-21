@echo off
rem Compile build file generator, if needed.
if  not exist PrintBuildInfo.class javac PrintBuildInfo.java

rem Compile build file.
java PrintBuildInfo %1 %2 %3 %4 %5 %6 %7 %8 %9 > src\BuildInfo.java

rem Run java compiler
ant package

