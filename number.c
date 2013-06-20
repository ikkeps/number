#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/mutex.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DRIVER_NAME "number"

// All we need to store in file private_data is one char, so here is the macros
#define CHAR_IN_VOID_PTR(p) (*((char*)&(p)))

dev_t device_number;

struct cdev device;

static int number_open(struct inode *inod, struct file *filp){
  CHAR_IN_VOID_PTR(filp->private_data) = (char)MINOR(inod->i_rdev);
  return 0;
};

static ssize_t number_read(struct file * filp, char __user * buf, size_t size, loff_t * offset){
  size_t n;
  char c = CHAR_IN_VOID_PTR(filp->private_data);

  if (!access_ok(ACCESS_WRITE, buf, size)) return -EINVAL;

  for(n=0; n<size; n++)
    __put_user(c, buf + n);

  return size;
};

static struct file_operations number_fops = {
  .owner = THIS_MODULE,
  .open = number_open,
  .read = number_read
};

static __init int number_init(void)
{
  int result;

  result = alloc_chrdev_region(&device_number, 0, 256, DRIVER_NAME);
  if (result < 0 ) return result;

  cdev_init(&device, &number_fops);
  device.owner = THIS_MODULE;
  result = cdev_add(&device, device_number, 256);
  if (result < 0)
    unregister_chrdev_region(device_number, 256);

  return result;
};

static __exit void number_exit(void)
{
  cdev_del(&device);
  unregister_chrdev_region(device_number, 256);
};

module_init(number_init);
module_exit(number_exit);
