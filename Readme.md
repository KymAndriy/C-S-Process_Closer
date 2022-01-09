## Compile app for Windows 
Start vcvars32.bat to setup Windows environment and compiler path
vcvars32.bat is (approximately) at-> E:\Visual Studio 19\Down\VC\Auxiliary\Build\vcvars32.bat
Install Gnu Make - http://gnuwin32.sourceforge.net/packages/make.htm


## Task 1, Variant 13
### Client/Server application **to* close processes* on remote Host.
- client gets processes list and sends them by server request.
- server will request to close one of the processes on client's host.
- client has to reply with success or failure.

Tested at Ubuntu 20.04.03 LTS with g++ 9.3.0 and Windows 10 21H1 19043.1288 with Visual Studio 2019 16.11.4

## WBS
![Work Breakdown Structure](/wbs.png)
