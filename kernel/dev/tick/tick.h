//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* one could reasonably ask why we need a tick device when we have pit.  
* pit is platform dependent while tick is not.
* we can have a slighly differnet implementation of tick on ARM and uses a device other than pit
*/
#ifndef _TICK_H
#define _TICK_H

#include <types.h>

struct device;

struct device* tick_attach(struct device* pit_device);
void tick_detach(struct device* dev);

#endif