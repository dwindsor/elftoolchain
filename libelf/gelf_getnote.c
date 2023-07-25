#include <libelf.h>
#include <gelf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "_libelf.h"

size_t gelf_getnote(Elf_Data *data, size_t offset, Elf_Note *result, size_t *name_offset, size_t *desc_offset) {


    fprintf(stderr, "in gelf_getnote\n");
    if (data == NULL)
    {
	    fprintf(stderr, "data is null\n");
	    return 0;
    }  

    struct _Libelf_Data *d;
    d = (struct _Libelf_Data *) data;

    Elf *e;
        Elf_Scn *scn;

        if (d == NULL || 
            (scn = d->d_scn) == NULL ||
            (e = scn->s_elf) == NULL) {
                return 0;
        }


    fprintf(stderr, "gelf_getnote: up in here\n");
    fprintf(stderr, "sizeof(GELf_Nhdr)=%ld\n", sizeof(GElf_Nhdr));
    fprintf(stderr, "d=%p\n", d);
    //fprintf(stderr, "sizeof(d)=%ld\n", sizeof(d->d_data));
    //fprintf(stderr, "d->d_size=%ld\n", d->d_data.d_size);

    //fprintf(stderr, "gelf_getnote: 1, data->d_size=%ld, sizeof(GElf_Nhdr)=%ld\n", data->d_size, sizeof(GElf_Nhdr));
    /* Read the NOTE header at the specified offset. */
    const Elf_Note *nhdr = (const Elf_Note *)((char *)d->d_data.d_buf + offset);

    /* Validate the NOTE header size. */
    if (offset > data->d_size || data->d_size - offset < sizeof(GElf_Nhdr))
    {
	    offset=0;
    }
    if (offset + sizeof(Elf_Note) > data->d_size)
    {
        return 0;
    }

	fprintf(stderr, "gelf_getnote: 3\n");
    /* Copy the NOTE header to the result. */
    memcpy(result, nhdr, sizeof(Elf_Note));

    /* Calculate the size of the entire NOTE entry. */
    size_t entry_size = sizeof(Elf_Note) + result->n_namesz + result->n_descsz;

    /* Validate the entire entry size. */
    if (offset + entry_size > data->d_size)
    {
        return 0;
    }

    fprintf(stderr, "gelf_getnote: 4\n");
    /* Set the name and descriptor offsets. */
    *name_offset = offset + sizeof(Elf_Note);
    *desc_offset = *name_offset + result->n_namesz;

    return offset;
}

