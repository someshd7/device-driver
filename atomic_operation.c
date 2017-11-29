An integer value also can become a shared resource.In such cases how we can provide a lock?In this situations,kernel provides an integer type called atomic_t.Header file for atomic operation is available in asm/atomic.h.
Signature for is atomic_t as follows.
typedef struct {
 int counter;
} atomic_t;
An atomic_t hold an integer value.But in some architecture it is working in a different way and because of that a full 32 bit may not be possible.For example in SPARC architecture, 0-7 bits are used for lock and 8-31 is used for data.So altogether we must consider only 24 bit for atomic_t.
How to initialize 
 static atomic_t my_counter = ATOMIC_INIT(1);
This can be used to initialize atomic_t before run time.
During run time, 
struct rt_atomic { atomic_t test; };
 struct rt_atomic *p;
 p = kmalloc(sizeof(*p), GFP_KERNEL);
 if (!k)
  return -ENOMEM;
 atomic_set(&p->counter, 0);
A proper implicit or explicit memory barrier is needed before the value set with the operation
is guaranteed to be readable with atomic_read from another thread.
#define atomic_read(v) ((v)->counter)
reads the counter value currently visible to the calling thread.
Method listing
ATOMIC_INIT(int i)
Initialize an atomic_t to i
int atomic_read(atomic_t *v)
Atomically read the integer value of v
void atomic_set(atomic_t *v, int i)
Atomically set v = i
void atomic_add(int i, atomic_t *v)
Atomically add i to v
void atomic_sub(int i, atomic_t *v)
Atomically subtract i from v
void atomic_inc(atomic_t *v)
Atomically add one to v
void atomic_dec(atomic_t *v)
Atomically subtract one from v
int atomic_sub_and_test(int i, atomic_t *v)
Atomically subtract i from v and return true if the result is zero; otherwise false
int atomic_add_negative(int i, atomic_t *v)
Atomically add i to v and return true if the result is negative; otherwise false
int atomic_dec_and_test(atomic_t *v)
Atomically decrement v by one and return true if zero; false otherwise
int atomic_inc_and_test(atomic_t *v)
Atomically increment v by one and return true if the result is zero; false otherwise
Atomic Bitwise Operations
Kernel provides a set of functions which operates on bit level. These are architecture specific and defined in <asm/bitops.h>.Bitwise operations operate on direct memory address.Bit 0 is the LSB (least significant bit ) and 31 is MSB(most significant bit )

unsigned long word = 0;
set_bit(0, &word);       /* bit zero is now set (atomically) */
set_bit(1, &word);       /* bit one is now set (atomically) */
printk("%ul\n", word);   /* will print "3" */
clear_bit(1, &word);     /* bit one is now unset (atomically) */
change_bit(0, &word);    /* bit zero is flipped; now it is unset (atomically) */
/* atomically sets bit zero and returns the previous value (zero) */
if (test_and_set_bit(0, &word)) {
        /* never true     */
}
/* the following is legal; you can mix atomic bit instructions with normal C */
word = 7;
Method Listing

void set_bit(int nr, void *addr)
Atomically set the nr-th bit starting from addr
void clear_bit(int nr, void *addr)
Atomically clear the nr-th bit starting from addr
void change_bit(int nr, void *addr)
Atomically flip the value of the nr-th bit starting from addr
int test_and_set_bit(int nr, void *addr)
Atomically set the nr-th bit starting from addr and return the previous value
int test_and_clear_bit(int nr, void *addr)
Atomically clear the nr-th bit starting from addr and return the previous value
int test_and_change_bit(int nr, void *addr)
Atomically flip the nr-th bit starting from addr and return the previous value
int test_bit(int nr, void *addr)
Atomically return the value of the nr-th bit starting from addr
