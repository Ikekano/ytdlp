@echo off

::
::		Main Menu
::
:Menu
cls
echo.
echo YT-DLP Downloader 
echo.
echo 1. Download From Youtube
echo 2. Download From Twitter (WIP will Exit)
echo 3. Download From Panopto
echo 4. Exit 
echo.
SET /P M=Select Option: 
IF /I %M%==1 GOTO YTMenu
::IF /I %M%==2 GOTO TWMenu
IF /I %M%==2 GOTO EOF
IF /I %M%==3 GOTO PTMenu
IF /I %M%==4 GOTO EOF

echo.
echo Error: Invalid Input
echo.
echo [Press Any Key To Continue]
pause > nul
GOTO Menu


::
::		Main Panopto Menu
::
:PTMenu
cls
echo.
echo YT-DLP Panopto Downloader 
echo.
echo 1. Download Video in Specified Format
echo 2. Exit 
echo.
SET /P M=Select Option: 
IF /I %M%==1 GOTO PTDL
IF /I %M%==2 GOTO EOF

echo.
echo Error: Invalid Input
echo.
echo [Press Any Key To Continue]
pause > nul
GOTO PTMenu


::
:: 		Panopto Download
::
:PTDL
cls
echo.
echo YT-DLP Panopto Downloader 
echo.
echo In order to download from Panopto the browser cookies need to be in a text file using this extension:
echo Only the cookies from Panopto are required!!! and place the txt file in the ytdlp folder. 
echo (DO NOT UPLOAD THIS TEXT FILE ANYWHERE ON GIT OR THE INTERNET!!!)
echo.
echo.
SET /P URL=Enter URL: 
echo.
echo Enter Cookies.txt File Name (Include the ".txt" at the end)
echo.
SET /P Cookies=Enter Cookies File:
echo.
yt-dlp.exe --list-formats --cookies "%Cookies%" "%URL%"

echo.
SET /P F=Desired Output Format: 
echo.
IF not exist "%~dp0\Downloads" mkdir %~dp0\Downloads
yt-dlp.exe -f %F% --cookies "%Cookies%" --ffmpeg-location "%~dp0\ffmpeg\bin" --paths "%~dp0\Downloads" "%URL%"
echo.
echo Returning to Main Menu...
timeout /t 3 /nobreak > nul
GOTO PTMenu

::
::		Main Youtube Menu
::
:YTMenu
cls
echo.
echo YT-DLP Youtube Downloader 
echo.
echo 1. Download Video (MP4)
echo 2. Download Audio (MP3)
echo 3. Download Video in Specified Format
echo 4. Download Video Thumbnail
echo 5. Exit 
echo.
SET /P M=Select Option: 
IF /I %M%==1 GOTO YTDL4
IF /I %M%==2 GOTO YTDL3
IF /I %M%==3 GOTO YTDLC
IF /I %M%==4 GOTO YTDLT
IF /I %M%==5 GOTO EOF

echo.
echo Error: Invalid Input
echo.
echo [Press Any Key To Continue]
pause > nul
GOTO YTMenu

::
:: 		Download MP4
::
:YTDL4
cls
echo.
echo YT-DLP Youtube Downloader (MP4)
echo.
SET /P URL=Enter URL: 
echo.
IF not exist "%~dp0\Downloads" mkdir %~dp0\Downloads
yt-dlp.exe --remux-video "mp4" --embed-thumbnail --ffmpeg-location "%~dp0\ffmpeg\bin" --paths "%~dp0\Downloads" "%URL%" 
::--paths "%~dp0../Downloads"
echo.
echo Returning to Main Menu...
timeout /t 5 /nobreak > nul
GOTO YTMenu

::
:: 		Download MP3
::
:YTDL3
cls
echo.
echo YT-DLP Youtube Downloader (MP3)
echo.
SET /P URL=Enter URL: 
echo.
GOTO YTDL3P

:YTDL3P
cls
echo.
echo YT-DLP Youtube Downloader (MP3)
echo.
echo Enter URL: %URL% 
echo.
SET /P Q=Desired Audio Quality (0 Best - 10 Worst): 
 
:: Check if the input is numeric and in the range 0-10
set "valid=1"
for /f "delims=" %%A in ("%Q%") do (
    set "valid=0"
    for /f "delims=0123456789" %%B in ("%%A") do set "valid=1"
)

if "!valid!"=="0" if "!Q!" geq "0" if "!Q!" leq "10" (
    GOTO YTDL3C
) else (
    echo.
	echo Error: Invalid Input
	echo.
	echo [Press Any Key To Continue]
	pause > nul
    GOTO YTDL3P
)

:YTDL3C
echo.
IF not exist "%~dp0\Downloads" mkdir %~dp0\Downloads
yt-dlp.exe -x --audio-format "mp3" --audio-quality %Q% --ffmpeg-location "%~dp0\ffmpeg\bin" --paths "%~dp0\Downloads" "%URL%"
echo.
echo Returning to Main Menu...
timeout /t 3 /nobreak > nul
GOTO YTMenu

::
:: 		Download With Chosen Format
::
:YTDLC
cls
echo.
echo YT-DLP Youtube Downloader 
echo.
SET /P URL=Enter URL: 
echo.
GOTO YTDLCP

:YTDLCP
cls
echo.
echo YT-DLP Youtube Thumbnail Downloader 
echo.
echo Enter URL: %URL%
echo.
SET /P E=Embed Thumbnail to Video? [y/n]: 
echo.
IF %E%==y GOTO YTDLC1
IF %E%==n GOTO YTDLC2
echo.
echo Error: Invalid Input
echo.
echo [Press Any Key To Continue]
pause > nul
GOTO YTDLCP 

:YTDLC1
yt-dlp.exe --list-formats "%URL%"
echo.
SET /P F=Desired Output Format: 
echo.
IF not exist "%~dp0\Downloads" mkdir %~dp0\Downloads
yt-dlp.exe -f %F% --embed-thumbnail --ffmpeg-location "%~dp0\ffmpeg\bin" --paths "%~dp0\Downloads" "%URL%"
echo.
echo Returning to Main Menu...
timeout /t 3 /nobreak > nul
GOTO YTMenu

:YTDLC2
yt-dlp.exe --list-formats "%URL%"
echo.
SET /P F=Desired Output Format: 
echo.
IF not exist "%~dp0\Downloads" mkdir %~dp0\Downloads
yt-dlp.exe -f %F% --ffmpeg-location "%~dp0\ffmpeg\bin" --paths "%~dp0\Downloads" "%URL%"
echo.
echo Returning to Main Menu...
timeout /t 3 /nobreak > nul
GOTO YTMenu

::
:: 		Download Thumbnail
::
:YTDLT
cls
echo.
echo YT-DLP Youtube Thumbnail Downloader 
echo.
SET /P URL=Enter URL: 
echo.
GOTO YTDLTP

:YTDLTP
cls
echo.
echo YT-DLP Youtube Thumbnail Downloader 
echo.
echo Enter URL: %URL%
echo.
SET /P T=Download Highest Quality Thumbnail? [y/n]:
IF %T%==y GOTO YTDLT1
IF %T%==n GOTO YTDLT2
echo.
echo Error: Invalid Input
echo.
echo [Press Any Key To Continue]
pause > nul
GOTO YTDLTP 

:YTDLT1
echo.
IF not exist "%~dp0\Downloads" mkdir %~dp0\Downloads
yt-dlp.exe --skip-download --convert-thumbnails "png" --write-thumbnail --ffmpeg-location "%~dp0\ffmpeg\bin" --paths "%~dp0\Downloads" "%URL%"
echo.
echo Returning to Main Menu...
timeout /t 5 /nobreak>nul
GOTO YTMenu

:YTDLT2
echo.
yt-dlp.exe --list-thumbnails "%URL%"
echo.
echo [Ctrl + Left-Click] A Link Above to Select Specific Thumbnail to Manually Download from Web
echo.
echo [Press Any Key To Continue]
pause > nul
GOTO YTMenu
