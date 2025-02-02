#include <windows.h>
#include <string>

#define ID_TEXTBOX 1
#define ID_BUTTON 2

// Function prototype
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void RunDownloadCommand(const std::string& link);

// Entry point for Windows GUI application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    HWND hwnd;
    MSG msg;

    // Hide the console window
    FreeConsole();

    // Define window properties
    wc.hInstance = hInstance;
    wc.lpszClassName = "YouTubeMP3Downloader";
    wc.lpfnWndProc = WindowProcedure;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // Create main window
    hwnd = CreateWindow("YouTubeMP3Downloader", "YouTube MP3 Downloader", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 500, 200, NULL, NULL, hInstance, NULL);

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

// Function to execute the PowerShell command silently
void RunDownloadCommand(const std::string& link) {
    std::string command = "powershell -WindowStyle Hidden -Command \""
        "IF (!(Test-Path 'Downloads')) { New-Item -ItemType Directory -Path 'Downloads' }; "
        "./yt-dlp.exe -x -q --audio-format 'mp3' --ffmpeg-location './ffmpeg/bin' --paths './Downloads' '" + link + "'\"";

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;
    
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;  // Hide the command window

    if (CreateProcess(NULL, (LPSTR)command.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
}

// Window Procedure: Handles GUI events
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hTextBox, hButton, hTitle;

    switch (msg) {
        case WM_CREATE:
            // Title label (centered at the top)
            hTitle = CreateWindow("STATIC", "YouTube MP3 Downloader", WS_VISIBLE | WS_CHILD | SS_CENTER,
                                  150, 10, 200, 20, hwnd, NULL, NULL, NULL);

            // Label: "YouTube Link:"
            CreateWindow("STATIC", "YouTube Link:", WS_VISIBLE | WS_CHILD,
                         20, 100, 100, 20, hwnd, NULL, NULL, NULL);

            // Text box for entering the YouTube link
            hTextBox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                    120, 100, 250, 20, hwnd, (HMENU)ID_TEXTBOX, NULL, NULL);

            // Download button
            hButton = CreateWindow("BUTTON", "Download", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                   380, 98, 80, 25, hwnd, (HMENU)ID_BUTTON, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON) { // If download button is clicked
                char link[256];
                GetWindowText(hTextBox, link, sizeof(link));

                if (strlen(link) > 0) {
                    RunDownloadCommand(link);
                } else {
                    MessageBox(hwnd, "Please enter a valid YouTube link.", "Error", MB_OK | MB_ICONERROR);
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
