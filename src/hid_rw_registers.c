#include <stdint.h>
#include <stdio.h>
#include <linux/hidraw.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

void set_feature(int fd, uint8_t report_id, uint8_t *data, int length)
{
    int ret;
    uint8_t buffer[length + 1];
    memcpy(buffer + 1, data, length);

    buffer[0] = report_id;

    ret = ioctl(fd, HIDIOCSFEATURE(length + 1), buffer);

    printf("set_feature: %d\n", ret);
}

void write_register(int fd, uint8_t bank, uint8_t address, uint8_t value)
{
    uint8_t buffer[] = {address, bank, value};

    set_feature(fd, 0x42, buffer, 3);
}

uint8_t get_feature(int fd, uint8_t report_id, uint8_t *data, int* length, int max_length)
{
    uint8_t buffer[max_length];

    memset(buffer, 0xcc, max_length);

    buffer[0] = report_id;

    int res = ioctl(fd, HIDIOCGFEATURE(max_length), buffer);

    if(res < 0) {
        printf("get_feature: error\n");
        return res;
    }
    else {
        *length = res;
        memcpy(data, buffer, res);
        return res;
    }
}

uint8_t read_register(int fd, uint8_t bank, uint8_t address)
{
    uint8_t buffer[4];
    int len = 0;
    write_register(fd, bank | 0x10, address, 0);

    get_feature(fd, 0x42, buffer, &len, 4);

    return buffer[3];
}

int main(int argc, char **argv)
{
    char * path = argv[1];
    printf("PATH: %s\n", path);
    printf("argc: %d\n", argc);

    int fd = open(path, O_RDWR|O_NONBLOCK);

    if(fd < 0)
    {
        printf("error main\n");
    }

    write_register(fd, 6, 0x74, 0x64);
    uint8_t read_value = read_register(fd, 6, 0x74);

    printf("main: return value: 0x%02x", read_value);

    return 0;

}
