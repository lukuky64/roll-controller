#ifndef SD_HPP
#define SD_HPP

#include <SD.h>

class Sd
{
public:
    Sd(size_t bufferSize = 512);
    ~Sd();

    bool init(int CS, int MOSI, int MISO, int SCK, String StartMsg);

    bool writeToBuffer(String dataString);
    void flushBuffer();
    bool isInitialized();

    String createUniqueLogFile();

private:
    File dataFile;
    String fileName;
    bool isFileOpen;
    String buffer;
    size_t maxBufferSize;
};

#endif