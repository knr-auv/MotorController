#include <stdbool.h>
typedef enum{
    ERROR_IMU,
    ERROR_BARO,
    ERROR_TEST,
    ERROR_COUNT
} error_e;

bool ERRORStatus[ERROR_COUNT];

void ERRORHandle(void);