#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/sched.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DRIVER_NAME "number"

static dev_t device_number;

static struct cdev device;

static int number_open(struct inode *inod, struct file *filp){
  char *page = (char *)__get_free_page(GFP_KERNEL);
  if (page < 0) return -ENOMEM;
  memset(page, (char)MINOR(inod->i_rdev), PAGE_SIZE);
  filp->private_data = page;
  return 0;
};

static int number_release(struct inode *inod, struct file *filp){
  free_page((unsigned long)filp->private_data);
  return 0;
};

static ssize_t number_read(struct file *filp, char __user *buf, size_t size, loff_t *offset){
  size_t written, to_write, not_written;
  char *page = filp->private_data;

  if (size == 0) return 0;

  if (!access_ok(ACCESS_WRITE, buf, size)) return -EFAULT;

  written = 0;
  while(written < size){
    to_write = min(size - written, PAGE_SIZE);
    not_written = __copy_to_user(buf + written, page, to_write);
    written += to_write - not_written;
    // If any pending signal - just return bytes written
    if(signal_pending(current)) break;
    // Be nice to others
    cond_resched();
  }

  return written;
};

static ssize_t number_write(struct file *filp, const char __user *buf, size_t size, loff_t *offset){
  return size;
};

static struct file_operations number_fops = {
  .owner = THIS_MODULE,
  .open = number_open,
  .release = number_release,
  .read = number_read,
  .write = number_write,
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
