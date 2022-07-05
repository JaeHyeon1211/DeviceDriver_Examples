#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define DEBUG 1
#define IMX_GPIO_NR(bank, nr)       (((bank) - 1) * 32 + (nr))

int led[] = {
    IMX_GPIO_NR(1, 16),   //16
    IMX_GPIO_NR(1, 17),   //17
    IMX_GPIO_NR(1, 18),   //18
    IMX_GPIO_NR(1, 19),   //19
};

static int led_init(void)
{
	int ret = 0;
    int i;

    for (i = 0; i < ARRAY_SIZE(led); i++) {
        ret = gpio_request(led[i], "gpio led");
        if(ret<0){
            printk("#### FAILED Request gpio %d. error : %d \n", led[i], ret);
        }
    }
    return ret;
	
}

static void led_write(unsigned long data)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(led); i++){
        gpio_direction_output(led[i], (data >> i ) & 0x01);
	}
#if DEBUG
    printk("#### %s, data = %ld\n", __FUNCTION__, data);
#endif
}

static void led_exit(void)
{
	int i;
    for (i = 0; i < ARRAY_SIZE(led); i++){
        gpio_free(led[i]);
    }
}

static int led_on(void)
{
	led_init();
	led_write(0x0f);
	return 0;
}
static void led_off(void)
{
	led_write(0x00);
	led_exit();
}

module_init(led_on);
module_exit(led_off);
MODULE_AUTHOR("kcci");
MODULE_DESCRIPTION("test module");
MODULE_LICENSE("Dual BSD/GPL");

