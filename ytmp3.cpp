#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>

#define ID_TEXTBOX 1
#define ID_BUTTON 2
#define ID_STATUS 3

// Function prototypes
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void RunDownloadCommand(const std::string& link, HWND hStatus);
void ReadConfig(std::string &audioQuality, std::string &downloadPath, bool &hidePowerShell);
void SetStatusMessage(HWND hStatus, const std::string &message);

// Global config variables
std::string configAudioQuality = "";
std::string configDownloadPath = "";
bool configHidePowerShell = true; // Default to hidden

// Entry point for Windows GUI application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    HWND hwnd;
    MSG msg;

    // Hide the console window
    FreeConsole();

    // Read config file
    ReadConfig(configAudioQuality, configDownloadPath, configHidePowerShell);

    // Define window properties
    wc.hInstance = hInstance;
    wc.lpszClassName = "YouTubeMP3Downloader";
    wc.lpfnWndProc = WindowProcedure;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // Create main window
    hwnd = CreateWindow("YouTubeMP3Downloader", "YouTube MP3 Downloader", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 500, 250, NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

// Function to read configuration file
void ReadConfig(std::string &audioQuality, std::string &downloadPath, bool &hidePowerShell) {
    std::ifstream configFile("ytmp3-config.ini");
    if (!configFile) return; // If file doesn't exist, use defaults

    std::string line;
    while (std::getline(configFile, line)) {
        if (line.empty() || line[0] == ';') continue; // Ignore comments & empty lines

        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // Trim whitespace
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (key == "quality" && !value.empty()) {
            long q = strtol(value.c_str(), NULL, 10); // Use strtol instead of stoi
            if (q >= 0 && q <= 10) {
                std::ostringstream oss;
                oss << "--audio-quality " << q;
                audioQuality = oss.str();
            }
        } else if (key == "downloads" && !value.empty()) {
            downloadPath = value;
        } else if (key == "hidepowershell") {
            if (value == "true") hidePowerShell = true;
            else if (value == "false") hidePowerShell = false;
        }
    }
}

// Function to execute the PowerShell command with optional visibility
void RunDownloadCommand(const std::string& link, HWND hStatus) {
    std::string command = "powershell -Command \"";

    // Handle download path
    if (configDownloadPath.empty()) {
        command += "IF (!(Test-Path 'Downloads')) { New-Item -ItemType Directory -Path 'Downloads' }; ";
        command += "./yt-dlp.exe -x --audio-format 'mp3' --ffmpeg-location './ffmpeg/bin' --paths './Downloads' ";
    } else {
        command += "./yt-dlp.exe -x --audio-format 'mp3' --ffmpeg-location './ffmpeg/bin' --paths '" + configDownloadPath + "' ";
    }

    // Handle audio quality
    if (!configAudioQuality.empty()) {
        command += configAudioQuality + " ";
    }

    command += "'" + link + "'\"";

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;
    
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = configHidePowerShell ? SW_HIDE : SW_SHOWDEFAULT;

    DWORD creationFlags = configHidePowerShell ? CREATE_NO_WINDOW : 0; // Hide or show PowerShell

    if (CreateProcess(NULL, (LPSTR)command.c_str(), NULL, NULL, FALSE, creationFlags, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        SetStatusMessage(hStatus, "Download started...");
    } else {
        SetStatusMessage(hStatus, "Error: Failed to start download.");
    }
}

// Function to set status message and auto-clear after 5 seconds
void SetStatusMessage(HWND hStatus, const std::string &message) {
    SetWindowText(hStatus, message.c_str());

    std::thread([hStatus]() {
        Sleep(5000); // Wait 5 seconds
        SetWindowText(hStatus, ""); // Clear message
    }).detach();
}

// Window Procedure: Handles GUI events
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hTextBox, hButton, hTitle, hStatus;

    switch (msg) {
        case WM_CREATE:
            // Title label (centered at the top)
            hTitle = CreateWindow("STATIC", "YouTube MP3 Downloader", WS_VISIBLE | WS_CHILD | SS_CENTER,
                                  100, 10, 300, 20, hwnd, NULL, NULL, NULL);

            // Status message (between title and input)
            hStatus = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_CENTER,
                                   100, 60, 300, 40, hwnd, (HMENU)ID_STATUS, NULL, NULL);

            // Label: "YouTube Link:"
            CreateWindow("STATIC", "YouTube Link:", WS_VISIBLE | WS_CHILD,
                         20, 145, 100, 20, hwnd, NULL, NULL, NULL);

            // Text box for entering the YouTube link
            hTextBox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                    120, 145, 250, 20, hwnd, (HMENU)ID_TEXTBOX, NULL, NULL);

            // Download button
            hButton = CreateWindow("BUTTON", "Download", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                   380, 144, 80, 25, hwnd, (HMENU)ID_BUTTON, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON) { // If download button is clicked
                char link[256];
                GetWindowText(hTextBox, link, sizeof(link));

                if (strlen(link) > 0) {
                    RunDownloadCommand(link, hStatus);
                } else {
                    SetStatusMessage(hStatus, "Please enter a valid YouTube link.");
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

