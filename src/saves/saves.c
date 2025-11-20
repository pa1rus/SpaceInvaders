#include "saves.h"

bool SaveValue(int value)
{
    #if defined(PLATFORM_WEB)
        EM_ASM_({ localStorage.setItem("highscore", $0); }, value);
        return true;
    #else
        unsigned char *data = (unsigned char *)&value;
        unsigned int size = sizeof(int);
        return SaveFileData("save/save.data", data, size);
    #endif
}

int LoadValue()
{
    #if defined(PLATFORM_WEB)
        int result = EM_ASM_INT({
            let v = localStorage.getItem("highscore");
            if (!v)
                return 0;
            return parseInt(v);
        });
        return result;
    #else
        int dataSize = 0;
        unsigned char *fileData = LoadFileData("save/save.data", &dataSize);
        if (fileData == NULL || dataSize < sizeof(int))
            return 0;

        int value = *((int *)fileData);
        UnloadFileData(fileData);
        return value;
    #endif
}
