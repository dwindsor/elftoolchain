#include <stdio.h>
#include <errno.h>
#include "../../../libelf/libelf.h"
#include <gelf.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define USDT_NOTE_SEC  ".notes"
#define USDT_NOTE_TYPE 3
#define USDT_NOTE_NAME "notes"

/*
 * From libbpf/src/usdt.c
 */
static int find_elf_sec_by_name(Elf *elf, const char *sec_name, GElf_Shdr *shdr, Elf_Scn **scn)
{
	Elf_Scn *sec = NULL;
	size_t shstrndx;

	if (elf_getshdrstrndx(elf, &shstrndx))
		return -EINVAL;

	fprintf(stderr, "Checkpoint 1\n");

	/* check if ELF is corrupted and avoid calling elf_strptr if yes */
	if (!elf_rawdata(elf_getscn(elf, shstrndx), NULL))
		return -EINVAL;

	fprintf(stderr, "Checkpoint 2\n");
	while ((sec = elf_nextscn(elf, sec)) != NULL) {
		char *name;

		if (!gelf_getshdr(sec, shdr))
			return -EINVAL;

		name = elf_strptr(elf, shstrndx, shdr->sh_name);
		fprintf(stderr, "find_elf_sec_by_name: sec=%s\n", name);
		if (name && strcmp(sec_name, name) == 0) {
			*scn = sec;
			return 0;
		}
	}

	return -ENOENT;
}

int main(int argc, char *argv[])
{
	Elf_Data *data;
	size_t off;
	size_t name_off;
	size_t desc_off;
	GElf_Nhdr nhdr;
	GElf_Shdr notes_shdr;
	Elf_Scn *notes_scn;
	int fd;
	int err;
	char elf_path[PATH_MAX] = "./samples/elf.notes.o";

	bzero(elf_path, PATH_MAX);
	if (argc >= 2)
	{
		strncpy(elf_path, argv[1], PATH_MAX);
	}

	fd = open(elf_path, O_RDONLY | O_CLOEXEC);
	if (fd < 0)
	{
		fprintf(stderr, "unable to open %s: %s\n", elf_path, strerror(errno));
		return 1;
	}

	/* Find the ELF header. */
	Elf *elf = elf_begin(fd, ELF_C_READ, NULL);
	if (!elf)
	{
		fprintf(stderr, "%s is not a valid ELF file\n", elf_path);
		return 1;
	}
	

	/* Find the NOTES section. */
	err = find_elf_sec_by_name(elf, USDT_NOTE_SEC, &notes_shdr, &notes_scn);
	if (err)
	{
		fprintf(stderr, "gelf-getnote: no USDT notes section (%s) found in '%s'\n", USDT_NOTE_SEC, elf_path);
		return err;
	}

	if (notes_shdr.sh_type == SHT_NOTE)
	{
		fprintf(stderr, "gelf-getnote: found a .notes section in %s\n", elf_path);
	}
	
	off = gelf_getnote(data, off, &nhdr, &name_off, &desc_off);
	int ret = 1;
	if (off > 0)
	{
		printf("Success!\n");
		ret = 0;
	}
	return ret;
}
