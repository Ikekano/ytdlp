@echo off

:Menu
cls
echo.
echo YT-DLP Youtube Downloader 
echo.
echo 1. Download Video (MP4)
echo 2. Download Audio (MP3)
echo 3. Download in Specified Format
echo 4. Download Thumbnail
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
GOTO MENU 

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
timeout /t 10 /nobreak
GOTO Menu

:YTDL3
cls
echo.
echo YT-DLP Youtube Downloader (MP3)
echo.
SET /P URL=Enter URL: 
echo.
SET /P Q=Desired Audio Quality (0 Best - 10 Worst): 
echo.
IF not exist "%~dp0\Downloads" mkdir %~dp0\Downloads
yt-dlp.exe -x --audio-format "mp3" --embed-thumbnail --audio-quality %Q% --ffmpeg-location "%~dp0\ffmpeg\bin" --paths "%~dp0\Downloads" "%URL%"
timeout /t 5 /nobreak
GOTO Menu

:YTDLC
cls
echo.
echo YT-DLP Youtube Downloader 
echo.
SET /P URL=Enter URL: 
echo.
yt-dlp.exe --list-formats "%URL%"
echo.
SET /P F=Desired Output Format: 
echo.
IF not exist "%~dp0\Downloads" mkdir %~dp0\Downloads
yt-dlp.exe -f %F% --embed-thumbnail --ffmpeg-location "%~dp0\ffmpeg\bin" --paths "%~dp0\Downloads" "%URL%"
timeout /t 5 /nobreak
GOTO Menu

:YTDLT
cls
echo.
echo YT-DLP Youtube Thumbnail Downloader 
echo.
SET /P URL=Enter URL: 
echo.
IF not exist "%~dp0\Downloads" mkdir %~dp0\Downloads
yt-dlp.exe --skip-download --list-thumbnails --convert-thumbnails "png" --write-thumbnail --ffmpeg-location "%~dp0\ffmpeg\bin" --paths "%~dp0\Downloads" "%URL%"
echo.
echo [Press Any Key To Continue]
pause > nul
GOTO Menu
