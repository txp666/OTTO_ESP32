#ifndef __CONFIG_H
#define __CONFIG_H

/*=========================
   Application configuration
 *=========================*/







/*=========================
   Hardware Configuration
 *=========================*/



/* Screen */

#define CONFIG_SCREEN_BLK_PIN       46

#define CONFIG_SCREEN_HOR_RES       240
#define CONFIG_SCREEN_VER_RES       240
#define CONFIG_SCREEN_BUFFER_SIZE   (CONFIG_SCREEN_HOR_RES * CONFIG_SCREEN_VER_RES /2)

/* Battery */
#define CONFIG_BAT_DET_PIN          16
#define CONFIG_BAT_CHG_DET_PIN      17





#endif
