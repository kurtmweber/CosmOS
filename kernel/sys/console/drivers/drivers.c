//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/drivers/drivers.h>

console_driver_interface_t console_interfaces[CONSOLE_DRIVER_LAST];
console_driver console_active_driver;