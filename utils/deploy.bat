rem D:\user\Bertrand\Documents\GitHub\Peezer\utils\deploy.bat  "${ProjDirPath}/Debug/${ProjName}" /home/pi/projects/${ProjName}.a
ECHO OFF

ECHO Start deploy

rem Check for a valid filename
IF "%1"=="" (
	ECHO You have to provide a valid source file.
	EXIT /b
)

IF "%2"=="" (
	ECHO You have to provide a valid destination path.
	EXIT /b
)


 


SET PUTTYSCP_BIN="C:\Program Files\PuTTY\pscp.exe"
SET PUTTY_BIN="C:\Program Files\PuTTY\putty.exe"


SET RASPBERRYPI_ADDR=pidump4

SET USERNAME=pi
SET PASSWORD=raspberry 

SET CMD_FILENAME=commands.sh

rem Upload the file to raspberry pi
%PUTTYSCP_BIN% -scp -P 22 -pw %PASSWORD% "%1" %USERNAME%@%RASPBERRYPI_ADDR%:"%2"

rem Build a list of actions to do on the pi (chmod, execute GDB server)
rem if exist %~dp0%CMD_FILENAME% del %~dp0%CMD_FILENAME%
rem echo rm "%2" >> %~dp0%CMD_FILENAME%
rem echo chmod +x "%2" >> %~dp0%CMD_FILENAME%
rem echo gdbserver :3785 "%2" >> %~dp0%CMD_FILENAME%

rem Execute the action list on the raspberry pi
rem %PUTTY_BIN% -pw %PASSWORD% -m %~dp0%CMD_FILENAME% %USERNAME%@%RASPBERRYPI_ADDR% -p 22
 
rem exit /b %ERRORLEVEL%

