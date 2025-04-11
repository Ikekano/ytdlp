#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>

void waitForEnter() {
    std::cout << "[Press Enter to continue...]";
    std::cin.ignore(); std::cin.get();
}

void versionInfo() {
    system("clear");
    std::cout << "\nYT-DLP Version Information\n\n";
    system("yt-dlp --version");

    std::cout << "\nCheck for Update and Download Newest Version? [y/n]: ";
    std::string update;
    std::cin >> update;
    if (update == "y") {
        system("yt-dlp -U");
    }
    std::cout << "Returning to Main Menu...\n";
    sleep(3);
}

void downloadPanopto() {
    system("clear");
    std::string url, cookies, format;
    std::cout << "Enter Panopto URL: ";
    std::cin.ignore(); std::getline(std::cin, url);

    std::cout << "Enter Cookies.txt filename (with .txt): ";
    std::getline(std::cin, cookies);

    std::string listCmd = "yt-dlp --list-formats --cookies \"" + cookies + "\" \"" + url + "\"";
    system(listCmd.c_str());

    std::cout << "\nDesired Output Format: ";
    std::getline(std::cin, format);

    system("mkdir -p Downloads");
    std::string cmd = "yt-dlp -f " + format + " --cookies \"" + cookies + "\" --paths Downloads """ + url + """";
    system(cmd.c_str());
    std::cout << "\nReturning to Main Menu...\n";
    sleep(3);
}

void downloadTwitter(bool customFormat) {
    system("clear");
    std::string url, format;
    std::cout << "Enter Twitter/X URL: ";
    std::cin.ignore(); std::getline(std::cin, url);
    system("mkdir -p Downloads");

    if (!customFormat) {
        std::string cmd = "yt-dlp --remux-video mp4 --embed-thumbnail --paths Downloads """ + url + """";
        system(cmd.c_str());
    } else {
        std::string listCmd = "yt-dlp --list-formats \"" + url + "\"";
        system(listCmd.c_str());
        std::cout << "\nDesired Output Format: ";
        std::getline(std::cin, format);
        std::string cmd = "yt-dlp -f " + format + " --embed-thumbnail --paths Downloads """ + url + """";
        system(cmd.c_str());
    }
    std::cout << "\nReturning to Main Menu...\n";
    sleep(3);
}

void downloadYoutube(int mode) {
    system("clear");
    std::string url, format, quality, embed;
    std::cout << "Enter YouTube URL: ";
    std::cin.ignore(); std::getline(std::cin, url);
    system("mkdir -p Downloads");

    switch (mode) {
        case 1: // MP4
            system(("yt-dlp --remux-video mp4 --paths Downloads """ + url + """").c_str());
            break;
        case 2: // MP3
            std::cout << "Desired Audio Quality (0 best - 10 worst): ";
            std::getline(std::cin, quality);
            system(("yt-dlp -x --audio-format mp3 --audio-quality " + quality + " --paths Downloads """ + url + """").c_str());
            break;
        case 3: // Custom format
            std::cout << "Embed thumbnail? [y/n]: ";
            std::getline(std::cin, embed);
            system(("yt-dlp --list-formats \"" + url + "\"").c_str());
            std::cout << "\nDesired Output Format: ";
            std::getline(std::cin, format);
            if (embed == "y")
                system(("yt-dlp -f " + format + " --embed-thumbnail --paths Downloads """ + url + """").c_str());
            else
                system(("yt-dlp -f " + format + " --paths Downloads \"" + url + "\"").c_str());
            break;
        case 4: // Thumbnail
            std::cout << "Download Highest Quality Thumbnail? [y/n]: ";
            std::getline(std::cin, embed);
            if (embed == "y")
                system(("yt-dlp --skip-download --convert-thumbnails png --write-thumbnail --paths Downloads """ + url + """").c_str());
            else {
                system(("yt-dlp --list-thumbnails """ + url + """").c_str());
                std::cout << "[Ctrl+Click a link above to download manually from browser]" << std::endl;
                waitForEnter();
            }
            break;
    }
    std::cout << "\nReturning to Main Menu...\n";
    sleep(3);
}


int main() {
    while (true) {
        system("clear");
        std::cout << "\nYT-DLP Downloader\n\n";
        std::cout << "1. Download from YouTube\n";
        std::cout << "2. Download from Twitter/X\n";
        std::cout << "3. Download from Panopto\n";
        std::cout << "4. Version Info\n";
        std::cout << "5. Exit\n\n";
        std::cout << "Select Option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int ytChoice = 0;
                while (ytChoice < 1 || ytChoice > 5) {
                    system("clear");
                    std::cout << "\nYouTube Menu:\n\n1. Download MP4\n2. Download MP3\n3. Download With Chosen Format\n4. Download Thumbnail\n5. Main Menu\n\nSelect Option: ";
                    std::cin >> ytChoice;
                    if (ytChoice >= 1 && ytChoice <= 4)
                        downloadYoutube(ytChoice);
                    else if (ytChoice != 5)
                        std::cout << "Invalid option! Try again.\n";
			sleep(3);
                }
                break;
            }
            case 2: {
                int twChoice = 0;
                while (twChoice < 1 || twChoice > 3) {
                    system("clear");
                    std::cout << "\nTwitter/X Menu:\n\n1. Download MP4\n2. Download With Chosen Format\n3. Main Menu\n\nSelect Option: ";
                    std::cin >> twChoice;
                    if (twChoice == 1) downloadTwitter(false);
                    else if (twChoice == 2) downloadTwitter(true);
                    else if (twChoice != 3)
                        std::cout << "Invalid option! Try again.\n";
			sleep(3);
                }
                break;
            }
            case 3:
                downloadPanopto();
                break;
            case 4:
                versionInfo();
                break;
            case 5:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid option!\n";
                waitForEnter();
        }
    }
    return 0;
}
