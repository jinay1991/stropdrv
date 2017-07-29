#ifndef __CHARDRV_IOCTLS_H__
#define __CHARDRV_IOCTLS_H__
/* -------------
 * IOCTL
 * -------------*/
#include <linux/ioctl.h>

#define MAGIC_CODE 'l'

#define IOCTL_STR_TO_LOWER _IO(MAGIC_CODE, 0)
#define IOCTL_STR_TO_UPPER _IO(MAGIC_CODE, 1)
#define IOCTL_STR_TO_CAMEL _IO(MAGIC_CODE, 2)

#endif
