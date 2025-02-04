#include "util.h"
#include <windows.h>

bool isFileLocked(const QString& filePath) {
    HANDLE hFile = CreateFile(
        filePath.toStdWString().c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_SHARING_VIOLATION)
            return true;
        return false;
    }

    CloseHandle(hFile);
    return false;
}
