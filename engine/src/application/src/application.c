#include "../includes/application.h"
#include "../../game_types.h"

#include "../../core/logger.h"

#include "../../platforms/includes/platform.h"

typedef struct {
    game_t* gameInstance;
    b8 isRunning;
    b8 isSuspended;
    platformState_t platform;
    i16 width;
    i16 height;
    f64 lastTime;
} applicationState_t;

static b8 initialized = FALSE;
static applicationState_t appState;

b8 applicationCreate(game_t* gameInstance) {
    if (initialized) {
        ERROR("application_create called more than once.");
        return FALSE;
    }

    appState.gameInstance = gameInstance;

    /* Initialize subsystems. */
    initializeLogging();

    /* Remove this. */
    FATAL("A test message: %f", 3.14f);
    ERROR("A test message: %f", 3.14f);
    WARNING("A test message: %f", 3.14f);
    INFO("A test message: %f", 3.14f);
    DEBUG("A test message: %f", 3.14f);
    TRACE("A test message: %f", 3.14f);

    appState.isRunning = TRUE;
    appState.isSuspended = FALSE;

    if (!platformStartup(&appState.platform, gameInstance->appConfig.name,
                         gameInstance->appConfig.startPositionX,
                         gameInstance->appConfig.startPositionY,
                         gameInstance->appConfig.startWidth,
                         gameInstance->appConfig.startHeight)) {
        return FALSE;
    }

    /* Initialize the game. */
    if (!appState.gameInstance->initialize(appState.gameInstance)) {
        FATAL("Game failed to initialize.");
        return FALSE;
    }

    appState.gameInstance->onResize(appState.gameInstance,
                                    appState.width, appState.height);

    initialized = TRUE;

    return TRUE;
}

b8 applicationRun() {
    while (appState.isRunning) {
        if (!platformPumpMessages(&appState.platform)) {
            appState.isRunning = FALSE;
        }

        if (!appState.isSuspended) {
            if (!appState.gameInstance->update(appState.gameInstance, (f32)0)) {
                FATAL("Game update failed, shutting down.");
                appState.isRunning = FALSE;
                break;
            }

            /* Call the game's render routine. */
            if (!appState.gameInstance->render(appState.gameInstance, (f32)0)) {
                FATAL("Game render failed, shutting down.");
                appState.isRunning = FALSE;
                break;
            }
        }
    }

    appState.isRunning = FALSE;

    platformShutdown(&appState.platform);

    return TRUE;
}
