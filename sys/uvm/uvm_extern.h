/*	$OpenBSD: uvm_extern.h,v 1.25 2001/11/06 00:24:55 art Exp $	*/
/*	$NetBSD: uvm_extern.h,v 1.43 2000/06/26 14:21:17 mrg Exp $	*/

/*
 *
 * Copyright (c) 1997 Charles D. Cranor and Washington University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Charles D. Cranor and
 *      Washington University.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * from: Id: uvm_extern.h,v 1.1.2.21 1998/02/07 01:16:53 chs Exp
 */

#ifndef _UVM_UVM_EXTERN_H_
#define _UVM_UVM_EXTERN_H_

/*
 * uvm_extern.h: this file defines the external interface to the VM system.
 *
 * this should be the only file included by non-VM parts of the kernel
 * which need access to VM services.   if you want to know the interface
 * to the MI VM layer without knowing the details, this is the file to
 * learn.
 *
 * NOTE: vm system calls are prototyped in syscallargs.h
 */

/*
 * defines
 */

/*
 * the following defines are for uvm_map and functions which call it.
 */

/* protections bits */
#define UVM_PROT_MASK	0x07	/* protection mask */
#define UVM_PROT_NONE	0x00	/* protection none */
#define UVM_PROT_ALL	0x07	/* everything */
#define UVM_PROT_READ	0x01	/* read */
#define UVM_PROT_WRITE  0x02	/* write */
#define UVM_PROT_EXEC	0x04	/* exec */

/* protection short codes */
#define UVM_PROT_R	0x01	/* read */
#define UVM_PROT_W	0x02	/* write */
#define UVM_PROT_RW	0x03    /* read-write */
#define UVM_PROT_X	0x04	/* exec */
#define UVM_PROT_RX	0x05	/* read-exec */
#define UVM_PROT_WX	0x06	/* write-exec */
#define UVM_PROT_RWX	0x07	/* read-write-exec */

/* 0x08: not used */

/* inherit codes */
#define UVM_INH_MASK	0x30	/* inherit mask */
#define UVM_INH_SHARE	0x00	/* "share" */
#define UVM_INH_COPY	0x10	/* "copy" */
#define UVM_INH_NONE	0x20	/* "none" */
#define UVM_INH_DONATE	0x30	/* "donate" << not used */

/* 0x40, 0x80: not used */

/* bits 0x700: max protection, 0x800: not used */

/* bits 0x7000: advice, 0x8000: not used */

typedef int		vm_prot_t;

/*
 *	Protection values, defined as bits within the vm_prot_t type
 *
 *   These are funky definitions from old CMU VM and are kept
 *   for compatibility reasons, one day they are going to die,
 *   just like everybody else.
 */

#define	VM_PROT_NONE	((vm_prot_t) 0x00)

#define VM_PROT_READ	((vm_prot_t) 0x01)	/* read permission */
#define VM_PROT_WRITE	((vm_prot_t) 0x02)	/* write permission */
#define VM_PROT_EXECUTE	((vm_prot_t) 0x04)	/* execute permission */

/*
 *	The default protection for newly-created virtual memory
 */

#define VM_PROT_DEFAULT	(VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)

/*
 *	The maximum privileges possible, for parameter checking.
 */

#define VM_PROT_ALL	(VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)

/*
 *	Enumeration of valid values for vm_inherit_t.
 */

#define	VM_INHERIT_SHARE	((vm_inherit_t)0)	/* share with child */
#define	VM_INHERIT_COPY		((vm_inherit_t)1)	/* copy into child */
#define	VM_INHERIT_NONE		((vm_inherit_t)2)	/* absent from child */
#define	VM_INHERIT_DONATE_COPY	((vm_inherit_t)3)	/* copy and delete */

#define VM_INHERIT_DEFAULT	VM_INHERIT_COPY

/* advice: matches MADV_* from sys/mman.h */
#define UVM_ADV_NORMAL	0x0	/* 'normal' */
#define UVM_ADV_RANDOM	0x1	/* 'random' */
#define UVM_ADV_SEQUENTIAL 0x2	/* 'sequential' */
/* 0x3: will need, 0x4: dontneed */
#define UVM_ADV_MASK	0x7	/* mask */

/* mapping flags */
#define UVM_FLAG_FIXED   0x010000 /* find space */
#define UVM_FLAG_OVERLAY 0x020000 /* establish overlay */
#define UVM_FLAG_NOMERGE 0x040000 /* don't merge map entries */
#define UVM_FLAG_COPYONW 0x080000 /* set copy_on_write flag */
#define UVM_FLAG_AMAPPAD 0x100000 /* for bss: pad amap to reduce malloc() */
#define UVM_FLAG_TRYLOCK 0x200000 /* fail if we can not lock map */

/* macros to extract info */
#define UVM_PROTECTION(X)	((X) & UVM_PROT_MASK)
#define UVM_INHERIT(X)		(((X) & UVM_INH_MASK) >> 4)
#define UVM_MAXPROTECTION(X)	(((X) >> 8) & UVM_PROT_MASK)
#define UVM_ADVICE(X)		(((X) >> 12) & UVM_ADV_MASK)

#define UVM_MAPFLAG(PROT,MAXPROT,INH,ADVICE,FLAGS) \
	((MAXPROT << 8)|(PROT)|(INH)|((ADVICE) << 12)|(FLAGS))

/* magic offset value */
#define UVM_UNKNOWN_OFFSET ((voff_t) -1)
				/* offset not known(obj) or don't care(!obj) */

/*
 * the following defines are for uvm_km_kmemalloc's flags
 */
#define UVM_KMF_NOWAIT	0x1			/* matches M_NOWAIT */
#define UVM_KMF_VALLOC	0x2			/* allocate VA only */
#define UVM_KMF_TRYLOCK	UVM_FLAG_TRYLOCK	/* try locking only */

/*
 * the following defines the strategies for uvm_pagealloc_strat()
 */
#define	UVM_PGA_STRAT_NORMAL	0	/* high -> low free list walk */
#define	UVM_PGA_STRAT_ONLY	1	/* only specified free list */
#define	UVM_PGA_STRAT_FALLBACK	2	/* ONLY falls back on NORMAL */

/*
 * flags for uvm_pagealloc_strat()
 */
#define UVM_PGA_USERESERVE	0x0001	/* ok to use reserve pages */
#define	UVM_PGA_ZERO		0x0002	/* returned page must be zero'd */

/*
 * lockflags that control the locking behavior of various functions.
 */
#define	UVM_LK_ENTER	0x00000001	/* map locked on entry */
#define	UVM_LK_EXIT	0x00000002	/* leave map locked on exit */

/*
 * structures
 */

struct buf;
struct core;
struct mount;
struct pglist;
struct proc;
struct ucred;
struct uio;
struct uvm_object;
struct vm_anon;
struct vm_aref;
struct vm_map;
struct vmspace;
struct pmap;
struct vnode;
struct simplelock;

/*
 * uvmexp: global data structures that are exported to parts of the kernel
 * other than the vm system.
 */

struct uvmexp {
	/* vm_page constants */
	int pagesize;   /* size of a page (PAGE_SIZE): must be power of 2 */
	int pagemask;   /* page mask */
	int pageshift;  /* page shift */

	/* vm_page counters */
	int npages;     /* number of pages we manage */
	int free;       /* number of free pages */
	int active;     /* number of active pages */
	int inactive;   /* number of pages that we free'd but may want back */
	int paging;	/* number of pages in the process of being paged out */
	int wired;      /* number of wired pages */
	int zeropages;	/* number of zero'd pages */
	int reserve_pagedaemon; /* number of pages reserved for pagedaemon */
	int reserve_kernel; /* number of pages reserved for kernel */

	/* pageout params */
	int freemin;    /* min number of free pages */
	int freetarg;   /* target number of free pages */
	int inactarg;   /* target number of inactive pages */
	int wiredmax;   /* max number of wired pages */

	/* swap */
	int nswapdev;	/* number of configured swap devices in system */
	int swpages;	/* number of PAGE_SIZE'ed swap pages */
	int swpguniq;	/* number of swap pages in use, not also in RAM */
	int swpginuse;	/* number of swap pages in use */
	int swpgonly;	/* number of swap pages in use, not also in RAM */
	int nswget;	/* number of times fault calls uvm_swap_get() */
	int nanon;	/* number total of anon's in system */
	int nanonneeded;/* number of anons currently needed */
	int nfreeanon;	/* number of free anon's */

	/* stat counters */
	int faults;		/* page fault count */
	int traps;		/* trap count */
	int intrs;		/* interrupt count */
	int swtch;		/* context switch count */
	int softs;		/* software interrupt count */
	int syscalls;		/* system calls */
	int pageins;		/* pagein operation count */
				/* pageouts are in pdpageouts below */
	int swapins;		/* swapins */
	int swapouts;		/* swapouts */
	int pgswapin;		/* pages swapped in */
	int pgswapout;		/* pages swapped out */
	int forks;  		/* forks */
	int forks_ppwait;	/* forks where parent waits */
	int forks_sharevm;	/* forks where vmspace is shared */
	int pga_zerohit;	/* pagealloc where zero wanted and zero
				   was available */
	int pga_zeromiss;	/* pagealloc where zero wanted and zero
				   not available */

	/* fault subcounters */
	int fltnoram;	/* number of times fault was out of ram */
	int fltnoanon;	/* number of times fault was out of anons */
	int fltpgwait;	/* number of times fault had to wait on a page */
	int fltpgrele;	/* number of times fault found a released page */
	int fltrelck;	/* number of times fault relock called */
	int fltrelckok;	/* number of times fault relock is a success */
	int fltanget;	/* number of times fault gets anon page */
	int fltanretry;	/* number of times fault retrys an anon get */
	int fltamcopy;	/* number of times fault clears "needs copy" */
	int fltnamap;	/* number of times fault maps a neighbor anon page */
	int fltnomap;	/* number of times fault maps a neighbor obj page */
	int fltlget;	/* number of times fault does a locked pgo_get */
	int fltget;	/* number of times fault does an unlocked get */
	int flt_anon;	/* number of times fault anon (case 1a) */
	int flt_acow;	/* number of times fault anon cow (case 1b) */
	int flt_obj;	/* number of times fault is on object page (2a) */
	int flt_prcopy;	/* number of times fault promotes with copy (2b) */
	int flt_przero;	/* number of times fault promotes with zerofill (2b) */

	/* daemon counters */
	int pdwoke;	/* number of times daemon woke up */
	int pdrevs;	/* number of times daemon rev'd clock hand */
	int pdswout;	/* number of times daemon called for swapout */
	int pdfreed;	/* number of pages daemon freed since boot */
	int pdscans;	/* number of pages daemon scaned since boot */
	int pdanscan;	/* number of anonymous pages scanned by daemon */
	int pdobscan;	/* number of object pages scanned by daemon */
	int pdreact;	/* number of pages daemon reactivated since boot */
	int pdbusy;	/* number of times daemon found a busy page */
	int pdpageouts;	/* number of times daemon started a pageout */
	int pdpending;	/* number of times daemon got a pending pagout */
	int pddeact;	/* number of pages daemon deactivates */
	
	/* kernel memory objects: managed by uvm_km_kmemalloc() only! */
	struct uvm_object *kmem_object;
	struct uvm_object *mb_object;
};

#ifdef _KERNEL

/*
 * the various kernel maps, owned by MD code
 */
extern struct vm_map *exec_map;
extern struct vm_map *kernel_map;
extern struct vm_map *kmem_map;
extern struct vm_map *mb_map;
extern struct vm_map *phys_map;

extern struct uvmexp uvmexp;

/*
 * macros
 */

/* zalloc zeros memory, alloc does not */
#define uvm_km_zalloc(MAP,SIZE) uvm_km_alloc1(MAP,SIZE,TRUE)
#define uvm_km_alloc(MAP,SIZE)  uvm_km_alloc1(MAP,SIZE,FALSE)

#endif /* _KERNEL */

/*
 * typedefs 
 */

typedef unsigned int  uvm_flag_t;
typedef int vm_fault_t;

#ifdef _KERNEL

/* uvm_aobj.c */
struct uvm_object	*uao_create __P((vsize_t, int));
void			uao_detach __P((struct uvm_object *));
void			uao_detach_locked __P((struct uvm_object *));
void			uao_reference __P((struct uvm_object *));
void			uao_reference_locked __P((struct uvm_object *));

/* uvm_fault.c */
int			uvm_fault __P((vm_map_t, vaddr_t, 
				vm_fault_t, vm_prot_t));
				/* handle a page fault */

/* uvm_glue.c */
#if defined(KGDB)
void			uvm_chgkprot __P((caddr_t, size_t, int));
#endif
void			uvm_fork __P((struct proc *, struct proc *, boolean_t,
			    void *, size_t));
void			uvm_exit __P((struct proc *));
void			uvm_init_limits __P((struct proc *));
boolean_t		uvm_kernacc __P((caddr_t, size_t, int));
__dead void		uvm_scheduler __P((void));
void			uvm_swapin __P((struct proc *));
boolean_t		uvm_useracc __P((caddr_t, size_t, int));
int			uvm_vslock __P((struct proc *, caddr_t, size_t,
			    vm_prot_t));
void			uvm_vsunlock __P((struct proc *, caddr_t, size_t));


/* uvm_init.c */
void			uvm_init __P((void));	
				/* init the uvm system */

/* uvm_io.c */
int			uvm_io __P((vm_map_t, struct uio *));

/* uvm_km.c */
vaddr_t			uvm_km_alloc1 __P((vm_map_t, vsize_t, boolean_t));
void			uvm_km_free __P((vm_map_t, vaddr_t, vsize_t));
void			uvm_km_free_wakeup __P((vm_map_t, vaddr_t,
						vsize_t));
vaddr_t			uvm_km_kmemalloc __P((vm_map_t, struct uvm_object *,
						vsize_t, int));
struct vm_map		*uvm_km_suballoc __P((vm_map_t, vaddr_t *,
				vaddr_t *, vsize_t, int,
				boolean_t, vm_map_t));
vaddr_t			uvm_km_valloc __P((vm_map_t, vsize_t));
vaddr_t			uvm_km_valloc_wait __P((vm_map_t, vsize_t));
vaddr_t			uvm_km_alloc_poolpage1 __P((vm_map_t,
				struct uvm_object *, boolean_t));
void			uvm_km_free_poolpage1 __P((vm_map_t, vaddr_t));

#define	uvm_km_alloc_poolpage(waitok)	uvm_km_alloc_poolpage1(kmem_map, \
						uvmexp.kmem_object, (waitok))
#define	uvm_km_free_poolpage(addr)	uvm_km_free_poolpage1(kmem_map, (addr))

/* uvm_map.c */
int			uvm_map __P((vm_map_t, vaddr_t *, vsize_t,
				struct uvm_object *, voff_t, uvm_flag_t));
int			uvm_map_pageable __P((vm_map_t, vaddr_t, 
				vaddr_t, boolean_t, int));
int			uvm_map_pageable_all __P((vm_map_t, int, vsize_t));
boolean_t		uvm_map_checkprot __P((vm_map_t, vaddr_t,
				vaddr_t, vm_prot_t));
int			uvm_map_protect __P((vm_map_t, vaddr_t, 
				vaddr_t, vm_prot_t, boolean_t));
struct vmspace		*uvmspace_alloc __P((vaddr_t, vaddr_t,
				boolean_t));
void			uvmspace_init __P((struct vmspace *, struct pmap *,
				vaddr_t, vaddr_t, boolean_t));
void			uvmspace_exec __P((struct proc *));
struct vmspace		*uvmspace_fork __P((struct vmspace *));
void			uvmspace_free __P((struct vmspace *));
void			uvmspace_share __P((struct proc *, struct proc *));
void			uvmspace_unshare __P((struct proc *));


/* uvm_meter.c */
void			uvm_meter __P((void));
int			uvm_sysctl __P((int *, u_int, void *, size_t *, 
				void *, size_t, struct proc *));
void			uvm_total __P((struct vmtotal *));

/* uvm_mmap.c */
int			uvm_mmap __P((vm_map_t, vaddr_t *, vsize_t,
				vm_prot_t, vm_prot_t, int, 
				caddr_t, voff_t, vsize_t));

/* uvm_page.c */
struct vm_page		*uvm_pagealloc_strat __P((struct uvm_object *,
				voff_t, struct vm_anon *, int, int, int));
#define	uvm_pagealloc(obj, off, anon, flags) \
	    uvm_pagealloc_strat((obj), (off), (anon), (flags), \
				UVM_PGA_STRAT_NORMAL, 0)
vaddr_t			uvm_pagealloc_contig __P((vaddr_t, vaddr_t,
				vaddr_t, vaddr_t));
void			uvm_pagerealloc __P((struct vm_page *, 
					     struct uvm_object *, voff_t));
/* Actually, uvm_page_physload takes PF#s which need their own type */
void			uvm_page_physload __P((paddr_t, paddr_t,
					       paddr_t, paddr_t, int));
void			uvm_setpagesize __P((void));

/* uvm_pdaemon.c */
void			uvm_pageout __P((void));

/* uvm_pglist.c */
int			uvm_pglistalloc __P((psize_t, paddr_t,
				paddr_t, paddr_t, paddr_t,
				struct pglist *, int, int)); 
void			uvm_pglistfree __P((struct pglist *));

/* uvm_swap.c */
void			uvm_swap_init __P((void));

/* uvm_unix.c */
int			uvm_coredump __P((struct proc *, struct vnode *, 
				struct ucred *, struct core *));
int			uvm_grow __P((struct proc *, vaddr_t));

/* uvm_user.c */
int			uvm_deallocate __P((vm_map_t, vaddr_t, vsize_t));

/* uvm_vnode.c */
void			uvm_vnp_setsize __P((struct vnode *, voff_t));
void			uvm_vnp_sync __P((struct mount *));
void 			uvm_vnp_terminate __P((struct vnode *));
				/* terminate a uvm/uvn object */
boolean_t		uvm_vnp_uncache __P((struct vnode *));
struct uvm_object	*uvn_attach __P((void *, vm_prot_t));

/* kern_malloc.c */
void			kmeminit_nkmempages __P((void));
void			kmeminit __P((void));
extern int		nkmempages;

void		swstrategy __P((struct buf *));

/* Machine dependent portion */
void		vmapbuf __P((struct buf *, vsize_t));
void		vunmapbuf __P((struct buf *, vsize_t));
void		pagemove __P((caddr_t, caddr_t, size_t));
void		cpu_fork __P((struct proc *, struct proc *, void *, size_t));
#ifndef	cpu_swapin
void		cpu_swapin __P((struct proc *));
#endif
#ifndef	cpu_swapout
void		cpu_swapout __P((struct proc *));
#endif

#endif /* _KERNEL */
#endif /* _UVM_UVM_EXTERN_H_ */
