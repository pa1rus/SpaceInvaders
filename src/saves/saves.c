#include "saves.h"

bool SaveValue(unsigned int position, int value){

    bool success = false;
    int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(SAVE_DATA_FILE, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL){

        if (dataSize <= position * sizeof(int)){

            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                TraceLog(LOG_ERROR, "Error");

                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else{

            newFileData = fileData;
            newDataSize = dataSize;

            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(SAVE_DATA_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);
    }
    else{

        dataSize = (position + 1) * sizeof(int);
        fileData = (unsigned char *) RL_MALLOC(dataSize);
        int *dataPtr = (int *) fileData;
        dataPtr[position] = value;

        success = SaveFileData(SAVE_DATA_FILE, fileData, dataSize);
        UnloadFileData(fileData);
    }

    return success;
}

int LoadValue(unsigned int position){

    int value = 0;
    int dataSize = 0;
    unsigned char *fileData = LoadFileData(SAVE_DATA_FILE, &dataSize);

    if (fileData != NULL){

        if (dataSize < ((int) position * sizeof(int))) TraceLog(LOG_ERROR, "Error");
        else {

            int *dataPtr = (int *) fileData;
            value = dataPtr[position];
        }
    }

    UnloadFileData(fileData);

    return value;
}
