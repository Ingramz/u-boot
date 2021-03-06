/*
 * Copyright (C) 2013 Google, Inc
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <cros_ec.h>
#include <asm/gpio.h>

int arch_early_init_r(void)
{
	if (cros_ec_board_init())
		return -1;

	return 0;
}

void setup_pch_gpios(u32 gpiobase, const struct pch_gpio_map *gpio)
{
	return;
}
