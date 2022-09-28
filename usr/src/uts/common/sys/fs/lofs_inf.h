/*
 * Copyright (c) 1989, 1990, 1991, 1995, 1996 by Sun Microsystems, Inc.
 * All rights reserved.
 */

/*
 * Loopback mount info - one per mount
 */

#ifndef _SYS_FS_LOFS_INFO_H
#define	_SYS_FS_LOFS_INFO_H

#pragma ident	"@(#)lofs_info.h	1.12	97/08/12 SMI"

#ifdef	__cplusplus
extern "C" {
#endif

struct loinfo {
	struct vfs	*li_realvfs;	/* real vfs of mount */
	struct vfs	*li_mountvfs;	/* loopback vfs */
	struct vnode	*li_rootvp;	/* root vnode of this vfs */
	int		 li_mflag;	/* mount flags to inherit */
	int		 li_refct;	/* # outstanding vnodes */
	dev_t		li_rdev;	/* lofs rdev */
	struct lfsnode	*li_lfs;	/* list of other vfss */
};

/* inheritable mount flags - propagated from real vfs to loopback */
#define	INHERIT_VFS_FLAG	(VFS_RDONLY|VFS_NOSUID)

/*
 * lfsnodes are allocated as new real vfs's are encountered
 * when looking up things in a loopback name space
 * It contains a new vfs which is paired with the real vfs
 * so that vfs ops (fsstat) can get to the correct real vfs
 * given just a loopback vfs
 */
struct lfsnode {
	struct lfsnode *lfs_next;	/* next in loinfo list */
	int		lfs_refct;	/* # outstanding vnodes */
	struct vfs	*lfs_realvfs;	/* real vfs */
	struct vnode    *lfs_realrootvp; /* real root vp */
	struct vfs	lfs_vfs;	/* new loopback vfs */
};

#define	vtoli(VFSP)	((struct loinfo *)((VFSP)->vfs_data))

#ifdef _KERNEL
extern struct vfs *lo_realvfs(struct vfs *, struct vnode **);
extern int lofsinit(struct vfssw *, int);
extern void lofs_subrinit(void);

extern struct vnodeops lo_vnodeops;
extern kmutex_t lofs_ltable_lock;
extern kmutex_t lofs_minor_lock;
extern struct vfsops lo_vfsops;
extern struct mod_ops mod_fsops;

extern major_t lofs_major;
extern minor_t lofs_minor;
#endif /* _KERNEL */


#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_FS_LOFS_INFO_H */
