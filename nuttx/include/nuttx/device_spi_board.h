/**
 * Copyright (c) 2016 Google, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __INCLUDE_NUTTX_DEVICE_SPI_BOARD_H
#define __INCLUDE_NUTTX_DEVICE_SPI_BOARD_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <nuttx/util.h>
#include <nuttx/device.h>
#include <nuttx/device_spi.h>
#include <nuttx/greybus/types.h>

#define DEVICE_TYPE_SPI_BOARD_HW "spi_board"

struct spi_board_device_cfg {
    /** chip name */
    uint8_t name[32];
    /** max speed be set in device */
    uint32_t max_speed_hz;
    /** type be set in device */
    uint8_t device_type;
    /** mode be set in device */
    uint16_t mode;
    /** bit per word be set in device */
    uint8_t bpw;
    /** external chip-select pin */
    uint8_t ext_cs;
    /** default cs pin state when using external chip-select pin */
    uint8_t init_cs_state;
};

struct spi_board_init_data {
    /** number of spi slave device */
    uint8_t num;
    /** maximum frequency supported in controlelr */
    uint32_t max_freq;
    /** minimum frequency supported in controlelr */
    uint32_t min_freq;
    /** maximum divider supported in controlelr */
    uint16_t max_div;
    /** bit pre-work supported in controlelr */
    uint32_t bpw_mask;
    /** TX FIFO depth supported in controller */
    uint32_t tx_depth;
    /** RX FIFO depth supported in controller */
    uint32_t rx_depth;
    /** configuration of spi slave device */
    struct spi_board_device_cfg *devices;
    /** using normal GPIO pin instead of internal chip-select */
    bool using_gpio;
};

/**
 * SPI board device driver operations
 */
struct device_spi_board_type_ops {
    /** Get number of SPI device. */
    int (*get_device_num)(struct device *dev, uint8_t *num);
    /** Get SPI specific chip configured information. */
    int (*get_device_cfg)(struct device *dev, uint8_t cs,
                          struct spi_board_device_cfg *dev_cfg);
    /** Check whether using normal gpio instead of internal chip-select */
    int (*is_using_gpio_cs)(struct device *dev, bool *using_gpio);
};

/**
 * @brief Get number of SPI device
 *
 * @param dev Pointer to structure of device.
 * @param num number of device.
 * @return 0 on success, negative errno on error.
 */
static inline int device_spi_board_get_device_num(struct device *dev,
                                                  uint8_t *num)
{
    DEVICE_DRIVER_ASSERT_OPS(dev);

    if (!device_is_open(dev)) {
        return -ENODEV;
    }

    if (DEVICE_DRIVER_GET_OPS(dev, spi_board)->get_device_num)
        return DEVICE_DRIVER_GET_OPS(dev,
                                spi_board)->get_device_num(dev, num);

    return -ENOSYS;
}

/**
 * @brief Get SPI specific chip configured information.
 *
 * @param dev Pointer to structure of device.
 * @param cs the specific chip number.
 * @param dev_cfg pointer to the spi_board_device_cfg structure to receive the
 *                configuration that be set in chip.
 * @return 0 on success, negative errno on error.
 */
static inline int device_spi_board_get_device_cfg(struct device *dev,
                              uint8_t cs, struct spi_board_device_cfg *dev_cfg)
{
    DEVICE_DRIVER_ASSERT_OPS(dev);

    if (!device_is_open(dev)) {
        return -ENODEV;
    }

    if (DEVICE_DRIVER_GET_OPS(dev, spi_board)->get_device_cfg)
        return DEVICE_DRIVER_GET_OPS(dev, spi_board)->get_device_cfg(dev, cs,
                                                                     dev_cfg);
    return -ENOSYS;
}

/**
 * @brief Check whether using normal gpio instead of internal chip-select
 *
 * @param dev Pointer to structure of device.
 * @param using_gpio whether using normal gpio or not.
 * @return 0 on success, negative errno on error.
 */
static inline int device_spi_board_is_using_gpio_cs(struct device *dev,
                                                  bool *using_gpio)
{
    DEVICE_DRIVER_ASSERT_OPS(dev);

    if (!device_is_open(dev)) {
        return -ENODEV;
    }

    if (DEVICE_DRIVER_GET_OPS(dev, spi_board)->is_using_gpio_cs)
        return DEVICE_DRIVER_GET_OPS(dev,
                                spi_board)->is_using_gpio_cs(dev, using_gpio);

    return -ENOSYS;
}

#endif
