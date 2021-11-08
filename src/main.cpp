
#include "FileFunctions.h"

#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"

std::string getFinalName(const std::string& curFilename) {
    // Get a lowercase version, just to make sure we aren't dealing with
    // upper/lowercase problems.
    std::string lowerCurFilename(curFilename);
    std::transform(lowerCurFilename.begin(), lowerCurFilename.end(),
                   lowerCurFilename.begin(),
                   [](unsigned char ch) { return std::tolower(ch); });

    // Handle two special cases.
    if (lowerCurFilename.compare("/config.ini.cur") == 0) {
        return "/config.ini";
    }
    if (lowerCurFilename.compare("/tft35.cur") == 0) {
        return "/TFT35.zip";
    }

    if (curFilename.size() <= 3) {
        // What? What file is this?
        return curFilename;
    }

    // Otherwise, we need to just rename the end to .bin.
    return curFilename.substr(0, curFilename.size() - 3) + "bin";
}

void setup() {
    // Some of the stuff coming over this line isn't from me. So this needs to
    // stay at 115200, or it will have junk in between.
    Serial.begin(115200);
}

void doubleLog(SdCard& sdcard, const char* message) {
    Serial.print(message);
    sdcard.log(message);
}

bool cardIn = false;
void processCard() {
    SdCard sdcard;
    if (sdcard.isValid() and not cardIn) {
        // This is a fresh insert.
        sdcard.clearLog();
        doubleLog(sdcard, "SD Card Inserted\n");
        cardIn = true;
    } else if (not sdcard.isValid() and cardIn) {
        Serial.println("SD Card Removed");
        cardIn = false;
        return;
    } else {
        // Nothing to do. No state change
        return;
    }

    doubleLog(sdcard, "SD_MMC Card Type: ");
    doubleLog(sdcard, sdcard.cardType());
    doubleLog(sdcard, "\n");

    Serial.printf("SD_MMC Card Size: %lluMB\n", sdcard.cardSize());

    sdcard.listDir("/");

    const auto curFiles = sdcard.getCurFiles();

    if (not curFiles.empty()) {
        doubleLog(sdcard, "Renaming cur files:\n");
        for (const auto& filename : curFiles) {
            std::string toName = getFinalName(filename);
            doubleLog(sdcard, "Renaming: ");
            doubleLog(sdcard, filename.c_str());
            doubleLog(sdcard, " to: ");
            doubleLog(sdcard, toName.c_str());
            doubleLog(sdcard, "\n");
            sdcard.rename(filename.c_str(), toName.c_str());
        }
    }
    else
    {
        doubleLog(sdcard, "No files to move.\n");
    }
}

void loop() {
    processCard();
    delay(1000);
}

