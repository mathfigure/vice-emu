/*
 * redpill.c - Real-time monitor & logging subsystem.
 *
 * Written by
 *  mathfigure <mathfigure@gmail.com>
 *
 * Redpill for VICE, copyright (c) mathfigure.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define WHITE "\x1b[1;37m"
#define GREEN "\x1b[1;32m"
#define RED "\x1b[1;31m"
#define NOCOLOR "\x1b[0m"
#endif

#include "redpill.h"

extern void  redpill_mem_init(void);

void redpill_init()
{
    redpill_mem_init();
}

void redpill_quit(const char *msg)
{
#ifdef WIN32
    MessageBox(NULL, msg, "REDPILL FAIL", MB_OK);
#else
    printf(RED "FAIL.\n" NOCOLOR);
    perror(msg);
#endif
    exit(EXIT_FAILURE);
}

void* shared_new(int size, const char *sid)
{
#ifdef WIN32
    HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, sid);
    if (hFileMap == NULL)
        redpill_quit("Shared memory failed!");
    return MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0,0,0);
#else
    printf(">Allocating shared memory: " WHITE "%s" NOCOLOR " = %d bytes, ",sid,size);
    int fd = shm_open(sid, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1)
        redpill_quit("shm_open() failed!");
    ftruncate(fd, size);
    void *ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED)
        redpill_quit("mmap() failed!");
    close(fd);
    printf(GREEN "OK\n" NOCOLOR);
    return ptr;
#endif
}
