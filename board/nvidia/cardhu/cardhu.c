/*
 *  (C) Copyright 2010-2013
 *  NVIDIA Corporation <www.nvidia.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gp_padctrl.h>
#include "pinmux-config-cardhu.h"
#include <i2c.h>

#define PMU_I2C_ADDRESS		0x2D
#define MAX_I2C_RETRY		3

/*
 * Routine: pinmux_init
 * Description: Do individual peripheral pinmux configs
 */
void pinmux_init(void)
{
	pinmux_config_pingrp_table(tegra3_pinmux_common,
		ARRAY_SIZE(tegra3_pinmux_common));

	pinmux_config_pingrp_table(unused_pins_lowpower,
		ARRAY_SIZE(unused_pins_lowpower));

	/* Initialize any non-default pad configs (APB_MISC_GP regs) */
	pinmux_config_drvgrp_table(cardhu_padctrl, ARRAY_SIZE(cardhu_padctrl));
}

#if defined(CONFIG_TEGRA_MMC)
/*
 * Do I2C/PMU writes to bring up SD card bus power
 *
 */
void board_sdmmc_voltage_init(void)
{
	struct udevice *dev;
	uchar reg, data_buffer[1];
	int ret;
	int i;

	ret = i2c_get_chip_for_busnum(0, PMU_I2C_ADDRESS, &dev);
	if (ret) {
		debug("%s: Cannot find PMIC I2C chip\n", __func__);
		return;
	}

	/* TPS659110: LDO5_REG = 3.3v, ACTIVE to SDMMC1 */
	data_buffer[0] = 0x65;
	reg = 0x32;

	for (i = 0; i < MAX_I2C_RETRY; ++i) {
		if (i2c_write(dev, reg, data_buffer, 1))
			udelay(100);
	}

	/* TPS659110: GPIO7_REG = PDEN, output a 1 to EN_3V3_SYS */
	data_buffer[0] = 0x09;
	reg = 0x67;

	for (i = 0; i < MAX_I2C_RETRY; ++i) {
		if (i2c_write(dev, reg, data_buffer, 1))
			udelay(100);
	}
}

/*
 * Routine: pin_mux_mmc
 * Description: setup the MMC muxes, power rails, etc.
 */
void pin_mux_mmc(void)
{
	/*
	 * NOTE: We don't do mmc-specific pin muxes here.
	 * They were done globally in pinmux_init().
	 */

	/* Bring up the SDIO1 power rail */
	board_sdmmc_voltage_init();
}
#endif	/* MMC */
