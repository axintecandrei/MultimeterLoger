/*
 * config.h
 *
 *  Created on: 14 feb. 2019
 *      Author: uidp7521
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include "stdint.h"

/*Config options*/
#define CFG_AVAILABLE     (1)
#define CFG_notAVAILABLE  (0)

#define CFG_DAS_USB_AVAILABLE (1)
#define CFG_DAS_UART_AVAILABLE (2)

#define CFG_ADC_REG_CONV     (1)
#define CFG_DEBUG_FMSTR      (0)
#define CFG_FMSTR_ON         (0)
#define CFG_ACQ_ON           (1)
#define CFG_DAS_USB_UART     (CFG_DAS_USB_AVAILABLE)
#endif /* CONFIG_H_ */
