/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the license.txt file.
 */

/** @cond To make doxygen skip this file */

/** @file
 *  This header contains defines with respect persistent storage that are specific to
 *  persistent storage implementation and application use case.
 */
#ifndef PSTORAGE_PL_H__
#define PSTORAGE_PL_H__

#include <stdint.h>

#define PSTORAGE_FLASH_PAGE_SIZE    ((uint16_t)NRF_FICR->CODEPAGESIZE)   /**< Size of one flash page. */
#define PSTORAGE_FLASH_EMPTY_MASK    0xFFFFFFFF                          /**< Bit mask that defines an empty address in flash. */

#define PSTORAGE_FLASH_PAGE_END                                      \
	((NRF_UICR->BOOTLOADERADDR != PSTORAGE_FLASH_EMPTY_MASK)     \
	 ? (NRF_UICR->BOOTLOADERADDR / PSTORAGE_FLASH_PAGE_SIZE)     \
	 : NRF_FICR->CODESIZE)

#define PSTORAGE_MAX_APPLICATIONS   2                                                           /**< Maximum number of applications that can be registered with the module, configurable based on system requirements. */
// minimum block size is 4 bytes, even if PSTORAGE_MIN_BLOCK_SIZE is set to lower than 0x4
#define PSTORAGE_MIN_BLOCK_SIZE     0x0004                                                      /**< Minimum size of block that can be registered with the module. Should be configured based on system requirements, recommendation is not have this value to be at least size of word. */

#define PSTORAGE_DATA_START_ADDR    ((PSTORAGE_FLASH_PAGE_END - PSTORAGE_MAX_APPLICATIONS - 1) \
		* PSTORAGE_FLASH_PAGE_SIZE)                                 /**< Start address for persistent data, configurable according to system requirements. */
#define PSTORAGE_DATA_END_ADDR      ((PSTORAGE_FLASH_PAGE_END - 1) * PSTORAGE_FLASH_PAGE_SIZE)  /**< End address for persistent data, configurable according to system requirements. */
#define PSTORAGE_SWAP_ADDR          PSTORAGE_DATA_END_ADDR                                      /**< Top-most page is used as swap area for clear and update. */

#define PSTORAGE_MAX_BLOCK_SIZE     PSTORAGE_FLASH_PAGE_SIZE                                    /**< Maximum size of block that can be registered with the module. Should be configured based on system requirements. And should be greater than or equal to the minimum size. */
#define PSTORAGE_CMD_QUEUE_SIZE     10                                                          /**< Maximum number of flash access commands that can be maintained by the module for all applications. Configurable. */


/** Abstracts persistently memory block identifier. */
typedef uint32_t pstorage_block_t;

typedef struct
{
	uint32_t            module_id;      /**< Module ID.*/
	pstorage_block_t    block_id;       /**< Block ID.*/
} pstorage_handle_t;

typedef uint16_t pstorage_size_t;      /** Size of length and offset fields. */

/**@brief Handles Flash Access Result Events. To be called in the system event dispatcher of the application. */
void pstorage_sys_event_handler (uint32_t sys_evt);

#endif // PSTORAGE_PL_H__

/** @} */
/** @endcond */
