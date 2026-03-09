#include <iostream>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>
#include <ctime>

using namespace std;

// function to sort an array of string of some size.
void sort(string n[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (n[i] > n[j]) {
                string temp = n[i];
                n[i] = n[j];
                n[j] = temp;
            }
        }
    }
}

// function to get permission from mode
string formatPermissions(mode_t mode) {
    string perms = "---------";

    if (mode & S_IRUSR) perms[0] = 'r';
    if (mode & S_IWUSR) perms[1] = 'w';
    if (mode & S_IXUSR) perms[2] = 'x';

    if (mode & S_IRGRP) perms[3] = 'r';
    if (mode & S_IWGRP) perms[4] = 'w';
    if (mode & S_IXGRP) perms[5] = 'x';

    if (mode & S_IROTH) perms[6] = 'r';
    if (mode & S_IWOTH) perms[7] = 'w';
    if (mode & S_IXOTH) perms[8] = 'x';

    return perms;
}

// function to return file type label
string fileType(mode_t mode) {
    if (S_ISDIR(mode)) return "[DIR]     ";
    if (S_ISLNK(mode)) return "[SYMLNK]  ";
    if (S_ISREG(mode)) return "[FILE]    ";
    return "[OTHER]   ";
}

// function to format time as Month day hour:min
string fomratTime(time_t t) {
    char buffer[100];
    struct tm *timeinfo = localtime(&t);
    strftime(buffer, sizeof(buffer), "%b %d %H:%M", timeinfo);
    return string(buffer);
}

int main() {
    char cwd[256];
    string names[256];
    string user_cmd;

    while (1) {
        getcwd(cwd, sizeof(cwd));
        cout << endl << cwd << "> ";

        getline(cin, user_cmd);

        if (user_cmd == "exit")
            break;

        if (user_cmd == "ls" || user_cmd == "ls -l" || user_cmd == "ls -al") {
            DIR *dir = opendir(cwd);
            if (!dir) {
                cout << "Error opening directory " << cwd << endl;
                return 1;
            }

            struct dirent *dir_entry;
            int i = 0;

            while ((dir_entry = readdir(dir)) != 0) {
                names[i++] = dir_entry->d_name;
            }

            closedir(dir);
            sort(names, i);

            bool showAll = (user_cmd == "ls -al");
            bool longFormat = (user_cmd == "ls -l" || user_cmd == "ls -al");

            if (!longFormat) {
                int p = 0;

                for (int j = 0; j < i; j++) {
                    if (!showAll && names[j][0] == '.')
                        continue;

                    p += names[j].length();
                    if (p > 80) {
                        cout << endl;
                        p = 0;
                    }

                    cout << names[j] << "  ";
                }
                cout << endl;
            } else {
                for (int j = 0; j < i; j++) {
                    if (!showAll && names[j][0] == '.')
                        continue;

                    string fullPath = string(cwd) + "/" + names[j];
                    struct stat sb;

                    if (lstat(fullPath.c_str(), &sb) == -1) {
                        cout << "Error reading file info for " << names[j] << endl;
                        continue;
                    }

                    struct passwd *pw = getpwuid(sb.st_uid);
                    string owner = (pw != NULL) ? pw->pw_name : to_string(sb.st_uid);

                    cout << fileType(sb.st_mode)
                         << formatPermissions(sb.st_mode) << " "
                         << setw(10) << owner << " "
                         << setw(8) << sb.st_size << " "
                         << fomratTime(sb.st_mtime) << " "
                         << names[j] << endl;
                }
            }
        }
    }

    return 0;
}