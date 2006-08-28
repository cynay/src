/*	$OpenBSD: cissreg.h,v 1.7 2006/08/28 02:56:33 mickey Exp $	*/

/*
 * Copyright (c) 2005,2006 Michael Shalayeff
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define	CISS_BIGBIT	0x80	/* texas radio and the big beat! */

#define	CISS_IDB	0x20
#define	CISS_IDB_CFG	0x01
#define	CISS_ISR	0x30
#define	CISS_IMR	0x34
#define	CISS_READYENAB	4
#define	CISS_READYENA	8
#define	CISS_INQ	0x40
#define	CISS_OUTQ	0x44
#define	CISS_CFG_BAR	0xb4
#define	CISS_CFG_OFF	0xb8

#define	CISS_DRVMAP_SIZE	(128 / 8)

#define	CISS_CMD_CTRL_GET	0x26
#define	CISS_CMD_CTRL_SET	0x27
/* sub-commands for GET/SET */
#define	CISS_CMS_CTRL_LDID	0x10
#define	CISS_CMS_CTRL_CTRL	0x11
#define	CISS_CMS_CTRL_LDSTAT	0x12
#define	CISS_CMS_CTRL_PDID	0x15
#define	CISS_CMS_CTRL_PDBLINK	0x16
#define	CISS_CMS_CTRL_PDBLSENS	0x17
#define	CISS_CMS_CTRL_LDIDEXT	0x18
#define	CISS_CMS_CTRL_FLUSH	0xc2
#define	CISS_CMS_CTRL_ACCEPT	0xe0

#define	CISS_CMD_LDMAP	0xc2
#define	CISS_CMD_PDMAP	0xc3

#define	ciss_bitset(d, v)	((v)[(d) >> 3] & (1 << ((d) & 7)))

struct ciss_softc;

struct ciss_config {
	u_int32_t	signature;
#define	CISS_SIGNATURE	(*(u_int32_t *)"CISS")
	u_int32_t	version;
	u_int32_t	methods;
#define	CISS_METH_READY	0x0001
#define	CISS_METH_SIMPL	0x0002
#define	CISS_METH_PERF	0x0004
#define	CISS_METH_EMQ	0x0008
	u_int32_t	amethod;
	u_int32_t	rmethod;
	u_int32_t	paddr_lim;
	u_int32_t	int_delay;
	u_int32_t	int_count;
	u_int32_t	maxcmd;
	u_int32_t	scsibus;
#define	CISS_BUS_U2	0x0001
#define	CISS_BUS_U3	0x0002
#define	CISS_BUS_FC1	0x0100
#define	CISS_BUS_FC2	0x0200
	u_int32_t	troff;
	u_int8_t	hostname[16];
	u_int32_t	heartbeat;
	u_int32_t	driverf;
#define	CISS_DRV_UATT	0x0001
#define	CISS_DRV_QINI	0x0002
#define	CISS_DRV_LCKINT	0x0004
#define	CISS_DRV_QTAGS	0x0008
#define	CISS_DRV_ALPHA	0x0010
#define	CISS_DRV_LUNS	0x0020
#define	CISS_DRV_MSGRQ	0x0080
#define	CISS_DRV_DBRD	0x0100
#define	CISS_DRV_PRF	0x0200
	u_int32_t	maxsg;
} __packed;

struct ciss_inquiry {
	u_int8_t	numld;
	u_int8_t	sign[4];
	u_int8_t	fw_running[4];
	u_int8_t	fw_stored[4];
	u_int8_t	hw_rev;
	u_int8_t	resv0[12];
	u_int16_t	pci_vendor;
	u_int16_t	pci_product;
	u_int8_t	resv1[10];
	u_int8_t	market_rev;
	u_int8_t	flags;
#define	CISS_INQ_WIDE	0x08
#define	CISS_INQ_BIGMAP	0x80
#define	CISS_INQ_BITS	"\020\04WIDE\010BIGMAP"
	u_int8_t	resv2[2];
	u_int8_t	nscsi_bus;
	u_int8_t	resv3[4];
	u_int8_t	clk[4];		/* unaligned dumbness */
	u_int8_t	buswidth;
	u_int8_t	disks[CISS_DRVMAP_SIZE];
	u_int8_t	extdisks[CISS_DRVMAP_SIZE];
	u_int8_t	nondisks[CISS_DRVMAP_SIZE];
} __packed;

struct ciss_ldmap {
	u_int32_t	size;
	u_int32_t	resv;
	struct {
		u_int32_t tgt;
		u_int32_t tgt2;
	} map[1];
} __packed;

struct ciss_flush {
	u_int16_t	flush;
#define	CISS_FLUSH_ENABLE	0
#define	CISS_FLUSH_DISABLE	1
	u_int16_t	resv[255];
} __packed;

struct ciss_blink {
	u_int32_t	duration;	/* x100ms */
	u_int32_t	elapsed;	/* only for sense */
	u_int8_t	pdtab[256];
#define	CISS_BLINK_ALL	1
#define	CISS_BLINK_TIMED 2
	u_int8_t	res[248];
} __packed;

struct ciss_ldid {
	u_int16_t	blksize;
	u_int16_t	nblocks[2];	/* UNALIGNED! */
	u_int8_t	params[16];
	u_int8_t	type;
#define	CISS_LD_RAID0	0
#define	CISS_LD_RAID4	1
#define	CISS_LD_RAID1	2
#define	CISS_LD_RAID5	3
#define	CISS_LD_RAID51	4
#define	CISS_LD_RAIDADG	5
	u_int8_t	res0;
	u_int8_t	bios_dis;
	u_int8_t	res1;
	u_int32_t	id;
	u_int8_t	label[64];
	u_int64_t	nbigblocks;
	u_int8_t	res2[410];
} __packed;

struct ciss_ldstat {
	u_int8_t	stat;
#define	CISS_LD_OK	0
#define	CISS_LD_FAILED	1
#define	CISS_LD_UNCONF	2
#define	CISS_LD_DEGRAD	3
#define	CISS_LD_RBLDRD	4	/* ready for rebuild */
#define	CISS_LD_REBLD	5
#define	CISS_LD_PDINV	6	/* wrong phys drive replaced */
#define	CISS_LD_PDUNC	7	/* phys drive is not connected proper */
#define	CISS_LD_EXPND	10	/* expanding */
#define	CISS_LD_NORDY	11	/* volume is not ready */
#define	CISS_LD_QEXPND	12	/* queued for expansion */
	u_int8_t	failed[4];	/* failed map */
	u_int8_t	res0[416];
	u_int8_t	prog[4];	/* blocks left to rebuild/expand */
	u_int8_t	rebuild;	/* drive that is rebuilding */
	u_int16_t	remapcnt[32];	/* count of re3mapped blocks for pds */
	u_int8_t	replaced[4];	/* replaced drives map */
	u_int8_t	spare[4];	/* used spares map */
	u_int8_t	sparestat;	/* spare status */
#define	CISS_LD_CONF	0x01	/* spare configured */
#define	CISS_LD_RBLD	0x02	/* spare is used and rebuilding */
#define	CISS_LD_DONE	0x04	/* spare rebuild done */
#define	CISS_LD_FAIL	0x08	/* at least one spare drive has failed */
#define	CISS_LD_USED	0x10	/* at least one spare drive is used */
#define	CISS_LD_AVAIL	0x20	/* at least one spare is available */
	u_int8_t	sparemap[32];	/* spare->pd replacement map */
	u_int8_t	replok[4];	/* replaced failed map */
	u_int8_t	readyok;	/* ready to become ok */
	u_int8_t	memfail;	/* cache mem failure */
	u_int8_t	expfail;	/* expansion failure */
	u_int8_t	rebldfail;	/* rebuild failure */
#define	CISS_LD_RBLD_READ	0x01	/* read faild */
#define	CISS_LD_RBLD_WRITE	0x02	/* write fail */
	u_int8_t	bigfailed[16];	/* bigmap vers of same of the above */
	u_int8_t	bigremapcnt[256];
	u_int8_t	bigreplaced[16];
	u_int8_t	bigspare[16];
	u_int8_t	bigsparemap[128];
	u_int8_t	bigreplok[16];
	u_int8_t	bigrebuild;	/* big-number rebuilding driveno */
} __packed;

struct ciss_pdid {
	u_int8_t	bus;
	u_int8_t	target;
	u_int16_t	blksz;
	u_int32_t	nblocks;
	u_int32_t	resblks;
	u_int8_t	model[40];
	u_int8_t	serial[40];
	u_int8_t	revision[8];
	u_int8_t	bits;
	u_int8_t	res0[2];
	u_int8_t	present;
#define	CISS_PD_PRESENT	0x01
#define	CISS_PD_NONDSK	0x02
#define	CISS_PD_WIDE	0x04
#define	CISS_PD_SYNC	0x08
#define	CISS_PD_NARROW	0x10
#define	CISS_PD_W2NARR	0x20	/* wide downgrade to narrow */
#define	CISS_PD_ULTRA	0x40
#define	CISS_PD_ULTRA2	0x80
	u_int8_t	config;
#define	CISS_PD_SMART	0x01
#define	CISS_PD_SMERRR	0x02
#define	CISS_PD_SMERRE	0x04
#define	CISS_PD_SMERRD	0x08
#define	CISS_PD_EXT	0x10
#define	CISS_PD_CONF	0x20
#define	CISS_PD_SPARE	0x40
#define	CISS_PD_CASAVE	0x80
	u_int8_t	res1;
	u_int8_t	cache;
#define	CISS_PD_CACHE	0x01
#define	CISS_PD_CASAFE	0x01
	u_int8_t	res2[5];
	u_int8_t	connector[2];
	u_int8_t	res3;
	u_int8_t	bay;
	u_int16_t	rpm;
	u_int8_t	type;
	u_int8_t	res4[393];
} __packed;

struct ciss_cmd {
	u_int8_t	resv0;	/* 00 */
	u_int8_t	sgin;	/* 01: #sg in the cmd */
	u_int16_t	sglen;	/* 02: #sg total */
	u_int32_t	id;	/* 04: cmd id << 2 and status bits */
#define	CISS_CMD_ERR	0x02
	u_int32_t	id_hi;	/* 08: not used */
	u_int32_t	tgt;	/* 0c: tgt:bus:mode or lun:mode */
#define	CISS_CMD_MODE_PERIPH	0x00000000
#define	CISS_CMD_MODE_LD	0x40000000
#define	CISS_CMD_TGT_MASK	0x40ffffff
#define	CISS_CMD_BUS_MASK	0x3f000000
#define	CISS_CMD_BUS_SHIFT	24
	u_int32_t	tgt2;	/* 10: scsi-3 address bytes */

	u_int8_t	cdblen;	/* 14: valid length of cdb */
	u_int8_t	flags;	/* 15 */
#define	CISS_CDB_CMD	0x00
#define	CISS_CDB_MSG	0x01
#define	CISS_CDB_NOTAG	0x00
#define	CISS_CDB_SIMPL	0x20
#define	CISS_CDB_QHEAD	0x28
#define	CISS_CDB_ORDR	0x30
#define	CISS_CDB_AUTO	0x38
#define	CISS_CDB_IN	0x80
#define	CISS_CDB_OUT	0x40
	u_int16_t	tmo;	/* 16: timeout in seconds */
#define	CISS_MAX_CDB	12
	u_int8_t	cdb[16];/* 18 */

	u_int64_t	err_pa;	/* 28: pa(struct ciss_error *) */
	u_int32_t	err_len;/* 30 */

	struct {		/* 34 */
		u_int32_t	addr_lo;
		u_int32_t	addr_hi;
		u_int32_t	len;
		u_int32_t	flags;
#define	CISS_SG_EXT	0x0001
	} sgl[1];
} __packed;

struct ciss_error {
	u_int8_t	scsi_stat;	/* SCSI_OK etc */
	u_int8_t	senselen;
	u_int16_t	cmd_stat;
#define	CISS_ERR_OK	0
#define	CISS_ERR_TGTST	1	/* target status */
#define	CISS_ERR_UNRUN	2
#define	CISS_ERR_OVRUN	3
#define	CISS_ERR_INVCMD	4
#define	CISS_ERR_PROTE	5
#define	CISS_ERR_HWERR	6
#define	CISS_ERR_CLOSS	7
#define	CISS_ERR_ABRT	8
#define	CISS_ERR_FABRT	9
#define	CISS_ERR_UABRT	10
#define	CISS_ERR_TMO	11
#define	CISS_ERR_NABRT	12
	u_int32_t	resid;
	u_int8_t	err_type[4];
	u_int32_t	err_info;
	u_int8_t	sense[32];
} __packed;

struct ciss_ccb {
	TAILQ_ENTRY(ciss_ccb)	ccb_link;
	struct ciss_softc	*ccb_sc;
	paddr_t			ccb_cmdpa;
	enum {
		CISS_CCB_FREE	= 0x01,
		CISS_CCB_READY	= 0x02,
		CISS_CCB_ONQ	= 0x04,
		CISS_CCB_PREQ	= 0x08,
		CISS_CCB_POLL	= 0x10,
		CISS_CCB_FAIL	= 0x80
#define	CISS_CCB_BITS	"\020\01FREE\02READY\03ONQ\04PREQ\05POLL\010FAIL"
	} ccb_state;

	struct scsi_xfer	*ccb_xs;
	size_t			ccb_len;
	void			*ccb_data;
	bus_dmamap_t		ccb_dmamap;

	struct ciss_error	ccb_err;
	struct ciss_cmd		ccb_cmd;	/* followed by sgl */
};

typedef TAILQ_HEAD(ciss_queue_head, ciss_ccb)     ciss_queue_head;

