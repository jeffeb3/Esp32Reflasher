
#include "FileFunctions.h"
#include <iostream>

SdCard::SdCard() : m_isValid(false) {
    if (SD_MMC.begin()) {
        uint8_t cardType = SD_MMC.cardType();

        if (cardType != CARD_NONE) {
            m_isValid = true;
        }
    }
}

SdCard::~SdCard() {
    if (m_isValid) {
        SD_MMC.end();
    }
    m_isValid = false;
}

const char *SdCard::cardType() {
    if (not isValid()) {
        return "NO CARD";
    }

    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_MMC) {
        return "MMC";
    } else if (cardType == CARD_SD) {
        return "SDSC";
    } else if (cardType == CARD_SDHC) {
        return "SDHC";
    } else {
        return "UNKNOWN";
    }
}

uint64_t SdCard::cardSize() {
    if (not isValid()) {
        return 0;
    }
    return SD_MMC.cardSize() / (1024 * 1024);
}

bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(),
                                        ending.length(), ending));
    } else {
        return false;
    }
}

bool endsWithCur(const char *name) {
    std::string filename(name);
    return hasEnding(name, "cur") or hasEnding(name, "CUR");
}

std::vector<std::string> SdCard::getCurFiles()
{
    if (not isValid()) {
        return std::vector<std::string>();
    }

    File root = SD_MMC.open("/");
    if (!root) {
        Serial.println("Failed to open directory");
        return std::vector<std::string>();
    }
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return std::vector<std::string>();
    }

    std::vector<std::string> curFiles;

    File file = root.openNextFile();
    while (file) {
        if (endsWithCur(file.name()))
        {
            curFiles.emplace_back(file.name());
        }
        file = root.openNextFile();
    }
    return curFiles;
}

void SdCard::listDir(const char *dirname) {
    if (not isValid()) {
        return;
    }

    Serial.printf("Listing directory: %s\n", dirname);

    File root = SD_MMC.open(dirname);
    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.print(file.name());
            Serial.print(" -- skipping");
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  CUR: ");
            Serial.print(endsWithCur(file.name()));
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void SdCard::rename(const char *from, const char *to) {
    if (not isValid()) {
        return;
    }
    Serial.printf("Renaming file %s to %s\n", from, to);
    if (SD_MMC.rename(from, to)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}
void SdCard::clearLog()
{
    SD_MMC.remove("/reflasherLog.txt");
}

void SdCard::log(const char *message) {
    File file = SD_MMC.open("/reflasherLog.txt", FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (not file.print(message)) {
        Serial.println("Append failed");
    }
}

