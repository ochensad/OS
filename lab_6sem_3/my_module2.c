#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init_task.h>

#include <linux/fs_struct.h>
#include <linux/path.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Narandaev Damir");

static int __init md_init(void)
{
  struct task_struct *task = &init_task;
  do
  {
    printk(KERN_INFO "+ taskp - %llu, task - %s, pid - %d, parent - %s, ppid - %d, state - %d, fl - %d, prio - %d, mnt - %s, policy - %d\n", 
      task,
      task->comm,
      task->pid,
      task->parent->comm,
      task->parent->pid,
      task->__state,
      task->flags,
      task->prio,
      task->fs->root.dentry->d_iname,
      task->policy
    );
  } while ((task = next_task(task)) != &init_task);
  printk(KERN_INFO "+ taskp - %llu, task - %s, pid - %d, parent - %s, ppid - %d, state - %d, fl - %d, prio - %d, mnt - %s, policy - %d\n", 
    current,
    current->comm,
    current->pid,
    current->parent->comm,
    current->parent->pid,
    current->__state,
    current->flags,
    current->prio,
    current->fs->root.dentry->d_iname,
    current->policy
  );
  return 0;
}

static void __exit md_exit(void) {}

module_init(md_init);
module_exit(md_exit);
