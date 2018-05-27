Spin lock
Spin lock is one of the most common locks in Linux Kernel.In spin lock case, If a thread of execution 
attempts to acquire a spin lock while it is already held,the threads busy loops spins waiting for the lock to become available.If the lock is not already held,then thread can immediately acquire the lock and continue its execution.It is not a wise decision to hold a spin lock for a longer time.
   In case of semaphore, if a lock is already held by one thread and the other thread is requesting to enter the critical section has to wait.In this case,the second thread which is already waiting will go to sleep.In this case, two context switch is required to put off and on the thread.
Main aspect of why a spin lock should not hold for more time is the longer time you hold the lock,that much time another processor has to spin waiting for you to release it.This will lead a higher priority process has to wait for the cpu situation.This will increase kernel latency.
Spin locks are architecture dependent and implemented in assembly. The architecture-dependent code
is defined in <asm/spinlock.h>. The actual usable interfaces are defined in <linux/spinlock.h>.
spinlock_t my_lock;

unsigned long flags;
spin_lock_init(&my_lock);
spin_lock_irqsave(&my_lock, flags);/*Save current state of interrupts,disables them locally and obtain the given lock*//* critical region ... */
spin_unlock_irqrestore(&my_lock, flags);/*Unlock the lock held and return interrupt to previous state*/ When you are locking a critical section really means that you are locking the shared data and not the code.
Method list
spin_lock()
Acquires a lock
spin_lock_irq()
Disables local interrupts and acquires given lock
spin_lock_irqsave()
Saves current state of local interrupts, disables local interrupts, and acquires given lock.
spin_unlock()
Releases given lock.
spin_unlock_irq()
Releases given lock and enables local interrupts.
spin_unlock_irqrestore()
Releases given lock and restores local interrupts to given previous state.
spin_lock_init()
Dynamically initializes given spinlock_t.
spin_trylock()
Tries to acquire given lock; if unavailable, returns nonzero.
spin_is_locked()
Returns nonzero if the given lock is currently acquired, otherwise it returns zero.
Spin lock with bottom halves
spin_lock_bh() is used to obtain lock spin_unlock_bh() performs the inverse.
If data is shared between process context and bottom half , protect the data in process context by disabling bottom half and acquiring lock because bottom half can preempt process context code.
If data is shared between  bottom half interrupt context , protect the data in bottom half by disabling interrupt and acquiring lock because interrupt handler can preempt a bottom half.
If the data is shared between a single type of tasklet, then no need to protect data because same type tasklet do not ever run simultaneously.if the data is shared between two different tasklet, you need only a normal sip lock before accessing data in bottom half.No need to disable bottom half because tasklet never preempt another running tasklet on the same processor.
In case of softirq also , a normal lock is only required.Two softirq of same type can run simultaneously on multiple processors on the system.Softirq will not preempt another softirq so disabling bottom half is not required. 
When it is preferred?
Spin lock is preferred when
1.It is low overhead locking
2.When the lock time is short.
3.Need to lock from interrupt context.
Reader-Writer spin lock
Sometimes locks can be divided into reader and writer locks.Let's consider an example of list which is being searched and updated.The search and update can happen simultaneously.This impact the consistency of data read from the list.Multiple concurrent readers are safe as long as there are no writers.reader-writer spin lock provide separate reader/writer lock.Reader lock can be held by more number of threads simultaneously but writer lock can be held by only one thread at a time.Thus we can say that reader lock is shared and writer lock is exclusive.The main thing to be noted here is reader-writer lock always favors readers.Writer will get the lock only after all readers releases their lock.
There for a sufficient number of readers can starve the writers in the system.
Implementation is similar to spin lock.
initilized by

rwlock_t my_rwlock = RW_LOCK_UNLOCKED;
In reader lock case.
read_lock(&my_rwlock);
/* critical section (read only) ... */
read_unlock(&my_rwlock);
In writer lock case.
write_lock(&my_rwlock);
/* critical section (read and write) ... */
write_unlock(&my_lock);
In this case , the same thread can recursively obtain the same lock.This is safe.
Method list

read_lock()
Acquires given lock for reading
read_lock_irq()
Disables local interrupts and acquires given lock for reading
read_lock_irqsave()
Saves the current state of local interrupts, disables local interrupts, and acquires the given lock for reading
read_unlock()
Releases given lock for reading
read_unlock_irq()
Releases given lock and enables local interrupts
read_unlock_irqrestore()
Releases given lock and restores local interrupts to the given previous state
write_lock()
Acquires given lock for writing
write_lock_irq()
Disables local interrupts and acquires the given lock for writing
write_lock_irqsave()
Saves current state of local interrupts, disables local interrupts, and acquires the given lock for writing
write_unlock() 
Releases given lock
write_unlock_irq()
Releases given lock and enables local interrupts
write_unlock_irqrestore()
Releases given lock and restores local interrupts to given previous state
write_trylock()
Tries to acquire given lock for writing; if unavailable, returns nonzero
rw_lock_init()
Initializes given rwlock_t
rw_is_locked()
Returns nonzero if the given lock is currently acquired, or else it returns zero
