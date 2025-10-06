#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <windows.h>

typedef struct {
    const char* string;
    const char* message;
} StringMessagePair;

StringMessagePair stringMessages[] = {
    {"/AVIX-Config", "AVIX Client found"},
    {"trumpclientftw_bape", "Bape Client found"},
    {"dg82fo.pw", "Drek Client found"},
    {"nG@W", "Fusk Client found"},
    {"G0ttaDipMen.java", "Gucci Client found"},
    {"Harambe.png", "Harambe Client found"},
    {"czaarek99", "Incognito Client found"},
    {"+(M0G.V", "Hillary Rodham Clinton Client found"},
    {"dinkio", "Kurium Client found"},
    {"Sa_Vc", "Merge Client found"},
    {"5d@56", "Merge Client found"},
    {"TCNH$1", "Motion Blur Misplace Client found"},
    {"onetap.cc", "Onetap Client found"},
    {"bspkrs.IlIIIlIlIllIIlllIllIllIII", "Spooky Client found"},
    {"com/sun/jna/z/Main", "Vape 2.06 Client found"},
    {"/a.class:::0", "Time Changer Misplace Client found"},
    {"hi.a2", "Veiv Client found"},
    {"0SO1Lk2KASxzsd", "Zuiy Client found"},
    {"yCcADi", "Vape 2.47 Client found"},
    {"74.91.125.194", "Vape 2.50 Client found"},
    {"kc((k", "Premium Vape Client found"},
    {"JNativeHook", "Autoclicker found"},
    {"144.217.241.181", "Vape Lite client found"},
    {"/tcpnodelaymod/COM1", "Vea Client found"},
    {"wurst", "Wurst Client found"},
    {"meteor", "Meteor Client found"},
    {"aristois", "Aristois Client found"}
};

int is_directory(const char* path) {
    struct stat s;
    if (stat(path, &s) == 0 && (s.st_mode & S_IFDIR)) return 1;
    return 0;
}

int has_valid_extension(const char* filename) {
    const char* exts[] = {".txt", ".log", ".json", ".cfg"};
    const char* pos = strrchr(filename, '.');
    if (!pos) return 0;
    for (size_t i = 0; i < sizeof(exts)/sizeof(exts[0]); i++) {
        if (_stricmp(pos, exts[i]) == 0) return 1;
    }
    return 0;
}

void scan_file_for_patterns(const char* filepath) {
    FILE* f = fopen(filepath, "r");
    if (!f) return;
    char buf[8192];
    unsigned long lineNumber = 0;
    while (fgets(buf, sizeof(buf), f)) {
        lineNumber++;
        for (size_t i = 0; i < sizeof(stringMessages)/sizeof(stringMessages[0]); i++) {
            if (strstr(buf, stringMessages[i].string)) {
                printf("File %s line %lu: %s\n", filepath, lineNumber, stringMessages[i].message);
            }
        }
    }
    fclose(f);
}

void scan_dir_recursive(const char* dirpath) {
    DIR* dir = opendir(dirpath);
    if (!dir) return;
    struct dirent* entry;
    char path[1024];
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
        if (is_directory(path)) {
            scan_dir_recursive(path);
        } else if (has_valid_extension(entry->d_name)) {
            scan_file_for_patterns(path);
        }
    }
    closedir(dir);
}

void try_add_and_scan(const char* path) {
    if (!path) return;
    if (is_directory(path)) {
        printf("Scanning directory: %s\n", path);
        scan_dir_recursive(path);
    } else {
        printf("Scanning file: %s\n", path);
        scan_file_for_patterns(path);
    }
}

int main(int argc, char* argv[]) {
    size_t numStrings = sizeof(stringMessages) / sizeof(stringMessages[0]);
    printf("String Scanner - scanning for %zu patterns\n", numStrings);

    if (argc == 2) {
        try_add_and_scan(argv[1]);
    } else {
        FILE* f = fopen("Search results.txt", "r");
        if (f) {
            fclose(f);
            try_add_and_scan("Search results.txt");
        }
        const char* dirs[] = {
            "C:/Users/%USERNAME%/AppData/Roaming/.minecraft/mods",
            "C:/Users/%USERNAME%/AppData/Roaming/Curse/Minecraft/Instances",
            "C:/Users/%USERNAME%/.minecraft/mods",
            "."
        };
        char expanded[512];
        for (size_t i = 0; i < sizeof(dirs)/sizeof(dirs[0]); i++) {
            ExpandEnvironmentStringsA(dirs[i], expanded, sizeof(expanded));
            try_add_and_scan(expanded);
        }
    }

    printf("Done. Press Enter to exit...\n");
    getchar();
    return 0;
}
