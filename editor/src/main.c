#include "../../engine/src/core/asserts.h"
#include "../../engine/src/core/logger.h"

#include "../../engine/src/platforms/includes/platform.h"

int main(void) {
    FATAL("A test message: %f", 3.14f);
    ERROR("A test message: %f", 3.14f);
    WARNING("A test message: %f", 3.14f);
    INFO("A test message: %f", 3.14f);
    DEBUG("A test message: %f", 3.14f);
    TRACE("A test message: %f", 3.14f);

    platformState_t state;

    if (platformStartup(&state, "Graphical Engine Editor", 100, 100, 1280, 720)) {
        while(TRUE) {
            platformPumpMessages(&state);
        }
    }

    platformShutdown(&state);

    return 0;
}
