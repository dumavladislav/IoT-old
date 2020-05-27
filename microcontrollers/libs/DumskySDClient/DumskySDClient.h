#include <SPI.h>
#include "SD.h"

namespace Dumsky {    


class SDClient {

public:

    SDClient();
    void init();
    String generateFileHeader();
    void writeLogString(String logString);

private:
    int filesCounter = 0;
    int recordsCounter = 0;
    int recordsInFileCounter = 0;

    const int maxRecordsInFile = 10000;

    File fileCounterFile;

    const String deviceId = "GPS1";
};


}