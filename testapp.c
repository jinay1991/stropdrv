#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define DEVICE_NAME "/dev/chardrv"

#include "chardrv_ioctls.h"

int main()
{
	int fd = open(DEVICE_NAME, O_RDWR);
	if (fd < 0)
	{
		printf("[ERR]: can not open %s\n", DEVICE_NAME);
		return -1;
	}

	const char write_buffer[256] = "Sending Buffer";

	printf("[INFO]: Writing Buffer: %s\n", write_buffer);

	int ret = write(fd, write_buffer, 256);
	if (ret < 0)
	{
		printf("[ERR]: write operation failed for %s, ret %d\n", DEVICE_NAME, ret);
		close(fd);
		return -1;
	}

	ret = ioctl(fd, IOCTL_STR_TO_LOWER, NULL);
	if (ret < 0)
	{
		printf("[ERR]: ioctl operation failed for %s, ret %d\n", DEVICE_NAME, ret);
		close(fd);
		return -1;
	}

	char read_buffer[256] = {0};

	ret = read(fd, read_buffer, 256);
	if (ret < 0)
	{
		printf("[ERR]: read operation failed for %s, ret %d\n", DEVICE_NAME, ret);
		close(fd);
		return -1;
	}


	printf("[INFO]: Read Buffer: %s\n", read_buffer);

	close(fd);
	return 0;
}


