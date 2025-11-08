#include "saves.h"

bool SaveValue(int value)
{

    unsigned char *data = (unsigned char *)&value;
    unsigned int size = sizeof(int);

    bool success = SaveFileData(SAVE_DATA_FILE, data, size);

    return success;
}

int LoadValue(void)
{

    int dataSize = 0;
    unsigned char *fileData = LoadFileData(SAVE_DATA_FILE, &dataSize);

    if (fileData == NULL)
    {

        return 0;
    }

    if (dataSize < (int)sizeof(int))
    {

        UnloadFileData(fileData);
        return 0;
    }

    int value = *((int *)fileData);
    UnloadFileData(fileData);
    return value;
}
