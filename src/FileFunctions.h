
#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"
#include <vector>


class SdCard
{
public:
    SdCard();
    ~SdCard();

    // Always check this.
    inline bool isValid() { return m_isValid; };

    uint64_t cardSize();

    // Return the card type.
    const char* cardType();

    void listDir(const char* dirname);

    void rename(const char* from, const char* to);

    // Clear the log file. Delete it.
    void clearLog();
    void log(const char* message);

    std::vector<std::string> getCurFiles();

private:
    bool m_isValid;
};

