#pragma once

#include <stddef.h>
#include <stdlib.h>
#include "stdio.h"

#define LOG_DEBUG(fmt, ...) printf("[d] " fmt "\n", ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) printf("[i] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) printf("[w] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf("[e] " fmt "\n", ##__VA_ARGS__)
