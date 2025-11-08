#include "saves.h"

#if defined(PLATFORM_WEB)
#include <emscripten.h>
#include <emscripten/html5.h>

// wskaźnik na funkcję callback
static void (*InitGameCallback)(void) = NULL;

// wrapper dla Emscripten - wywołuje callback po sync
void EMSCRIPTEN_KEEPALIVE CallInitGame()
{
    if (InitGameCallback) InitGameCallback();
}

void InitFileSystemAsync(void (*callback)(void))
{
    InitGameCallback = callback;

    EM_ASM(
        FS.mkdir('/save');
        FS.mount(IDBFS, {}, '/save');
        FS.syncfs(true, function(err) {
            if(err) console.log("FS initial sync error:", err);
            dynCall('v', Module._CallInitGame);
        });
    );
}
#endif

bool SaveValue(int value)
{
    unsigned char *data = (unsigned char *)&value;
    unsigned int size = sizeof(int);

    bool success = SaveFileData(SAVE_DATA_FILE, data, size);

#if defined(PLATFORM_WEB)
    EM_ASM(
        FS.syncfs(false, function(err) {
            if(err) console.log("FS sync error:", err);
        });
    );
#endif

    return success;
}

int LoadValue(void)
{
    int dataSize = 0;

    unsigned char *fileData = LoadFileData(SAVE_DATA_FILE, &dataSize);

    if (fileData == NULL || dataSize < (int)sizeof(int))
    {
        if(fileData) UnloadFileData(fileData);
        return 0;
    }

    int value = *((int*)fileData);
    UnloadFileData(fileData);
    return value;
}
