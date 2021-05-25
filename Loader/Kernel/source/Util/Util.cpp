#include "Common.hpp"
#include "Util/Util.hpp"
#include "kproc.hpp"

int MountNullFS(char* where, char* what, int flags)
{
    struct mntarg* ma = NULL;

    ma = mount_argf(ma, "fstype", "%s", "nullfs");
    ma = mount_argf(ma, "fspath", "%s", where);
    ma = mount_argf(ma, "target", "%s", what);

    if (ma == NULL) {
    	klog("Something is wrong, ma value is null after argument\n");
    	return 50;
    }

    return kern_mount(ma, flags);
}

void MountDir(thread* td, char* Sandbox, char* what, int flags)
{
    if(!td)
    {
        klog("Thread was NULL...\n");
        return;
    }

	char s_fulldir[0x200];
	snprintf(s_fulldir, sizeof(s_fulldir), "%s%s", Sandbox, what);

	klog("Mount: %s -> %s\n", s_fulldir, what);
	kern_mkdir(td, s_fulldir, 0, 0777);
	MountNullFS(s_fulldir, what, flags);
}

void UnMountDir(thread* td, char* Sandbox, char* what, int flags)
{
    if(!td)
    {
        klog("Thread was NULL...\n");
        return;
    }

	char s_fulldir[0x200];
	snprintf(s_fulldir, sizeof(s_fulldir), "%s%s", Sandbox, what);

	klog("Un-Mount: %s -> %s\n", s_fulldir, what);

	sys_unmount(s_fulldir, flags);
	kern_rmdir(td, s_fulldir, 0);
}

char* strrchr(const char *cp, int ch)
{
    char *save;
    char c;

    for (save = (char *) 0; (c = *cp); cp++) {
	if (c == ch)
	    save = (char *) cp;
    }

    return save;
}

char* strchr(const char *s, int c)
{
  do {
    if (*s == c)
      {
        return (char*)s;
      }
  } while (*s++);
  return (0);
}

char* basename(const char *filename)
{
  char *p = strrchr(filename, '/');
  return p ? p + 1 : (char *)filename;
}

void Jailbreak(proc* proc, Backup_Jail* jail)
{
    if(proc)
    {
        ucred* cred = proc->p_ucred;
        filedesc* fd = proc->p_fd;

        if(!cred || !fd)
            return;

        if(jail)
        {
            jail->cr_prison = cred->cr_prison;
            jail->cr_uid = cred->cr_uid;
            jail->cr_ruid = cred->cr_ruid;
            jail->cr_rgid = cred->cr_rgid;
            jail->cr_groups = cred->cr_groups[0];

            jail->fd_jdir = fd->fd_jdir;
            jail->fd_rdir = fd->fd_rdir;
        }
        
        cred->cr_prison = *(prison**)prison0;

        cred->cr_uid = 0;
        cred->cr_ruid = 0;
        cred->cr_rgid = 0;
        cred->cr_groups[0] = 0;

    
        fd->fd_jdir = *(vnode**)rootvnode;
        fd->fd_rdir = *(vnode**)rootvnode;
    }
}

void RestoreJail(proc* proc, Backup_Jail jail)
{
    if(proc)
    {
        ucred* cred = proc->p_ucred;
        filedesc* fd = proc->p_fd;

        if(!cred || !fd)
            return;

        cred->cr_prison = jail.cr_prison;
        cred->cr_uid = jail.cr_uid;
        cred->cr_ruid = jail.cr_ruid;
        cred->cr_rgid = jail.cr_rgid;
        cred->cr_groups[0] = jail.cr_groups;

        fd->fd_jdir = jail.fd_jdir;
        fd->fd_rdir = jail.fd_rdir;
    }
}

int klog(char* fmt, ...)
{
    char buffer[0x400] = { 0 };
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
    va_end(args);

    char buffer2[0x400] = { 0 };
    sprintf(buffer2, "[Orbis Toolbox] %s\n", buffer);

    #if defined(SOFTWARE_VERSION_505) || defined(SOFTWARE_VERSION_NA)
    
    struct thread *td = curthread();

	struct sys_read_args {
		uint64_t unk;
		uint64_t msg;
		uint64_t unk2;
	};

    struct sysent* sysents = sysvec->sv_table;

    auto sys_read = (int(*)(struct thread * td, struct sys_read_args * uap))sysents[601].sy_call;

    td->td_retval[0] = 0;

	struct sys_read_args uap;
	uap.unk = 7;
	uap.msg = (uint64_t)buffer2;
	uap.unk2 = 0;

	int errorno = sys_read(td, &uap);

	if(errorno)
		return -errorno;

    return (int)td->td_retval[0];

    #endif

    #if defined(SOFTWARE_VERSION_672) || defined(SOFTWARE_VERSION_702) || defined(SOFTWARE_VERSION_755)

    kprintf(buffer2);

    return 0;

    #endif
}

int sys_unmount(const char *dir, int flags)
{
	struct sys_unmount_args {
		uint64_t dir;
		uint64_t flags;
		uint64_t nbyte;
	};

    sysent* sysents = sysvec->sv_table;

    auto sys_unmount = (int(*)(thread * td, sys_unmount_args * uap))sysents[22].sy_call;

	thread *td = curthread();

    td->td_retval[0] = 0;

	sys_unmount_args uap;
	uap.dir = (uint64_t)dir;
	uap.flags = flags;

	int errorno = sys_unmount(td, &uap);
	if(errorno)
		return -errorno;

	return (int)td->td_retval[0];
}