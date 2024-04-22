# 49 MEMORY MAPPINGS

## TOC

 1. [Overview](#491-overview)
 2. [Creating a Mapping: *mmap()*](#492-creating-a-mapping-mmap)
 3. [Unmapping a Mapped Region: *munmap()*](#493-unmapping-a-mapped-region-munmap)

 4. [File Mappings](#494-file-mappings)
    1. [Private File Mappings](#4941-private-file-mappings)
    2. [Shared File Mappings](#4942-shared-file-mappings)
    3. [Boundary Cases](#4943-boundary-cases)
    4. [Memory Protection and File Access Mode Interactions](#4944-memory-protection-and-file-access-mode-interactions)

 5. [Synchronizing a Mapped Region: *msync()*](#495-synchronizing-a-mapped-region-msync)
 6. [Additional *mmap()* Flags](#496-additional-mmap-flags)
 7. [Anonymous Mappings](#497-anonymous-mappings)
 8. [Remapping a Mapped Region: mremap()](#498-remapping-a-mapped-region-mremap)
 9. [`MMAP_NORESERVE` and Swap Space Overcommitting](#499-mmap_noreserve-and-swap-space-overcommitting)
10. [The `MAP_FIXED` Flag](#4910-the-map-fixed-flag)
11. [Nonlinear Mappings: *remap_file_pages()*](#4911-nonlinear-mappings-remap-file-pages)
12. [Summary](#4912-summary)


## 49.1 Overview

The *mmap()* system call creates a new *memory mapping* in the calling
process's virtual address space.  A mapping can be of two types:

* *File mapping*: A file mapping maps a region of a file directly into the
  calling process's virtual memory. Once a fils i mapped, its contents can be
  accessed by operations on the bytes in the corresponding memory region. The
  pages of the mapping are (automatically) loaded from the file as required.
  This type of mapping is also known as *file-based mapping* or *memory-mapped
  file*.

* *Anynomous mapping*: An anonymous mapping doesn't have a corresponding file.
  Instead, the pages of the mapping are initialzsed to 0.

The memory in one process's mapping may be shared with mappings in other
processes (i.e., the page-table entries of each process point tot he same pages
of RAM). This can occur in two ways:

* When two processes map the same region of a file, they share the same pages
  of physical memory.

* A child process created by *fork()* inherits copies of its parent's mappings,
  and these mappings refer to the same pages of physical memory as the
  corresponding mapping in the parent.

When two or more processes share the same pages, each process can potentially see
the changes to the page contents made by other processes, depending on whether the
mapping is *private* or *shared*:

* *private mapping* (`MAP_PRIVATE`): Modifications to the contents of the
  mapping are not visible to other processes and, for a file mapping, are not
  carried through to the underlying file. Although the pages of a private
  mapping are initially shared in the circumstances described above, changes ot
  the contents of the mapping are nevertheless private to each process. The
  kernel accomplishes this using the copy-on-write technique ([Section
  24.2.2](#noop)). This means that whenever a process attempts to modify the
  contents of a page, the kernel first creates a new, separate copy of that
  page for the process (and adjusts the process's' page tables). For this
  reason, a `MAP_PRIVATE` mapping is sometimes referred to as a *private*,
  *copy-on-write mapping*.

* *Shared mapping* (`MAP_SHARED`): Modifications to the contents of the mapping
  are visible to other processes that share the same mapping and, for a file
  mapping, are carried through to the underlying file.

The two mapping attribute described above (file versus anonymous and private vs
shared) can be combined in four deifferent ways, as summarized in [Table
49-1](#table-49-1).

### Table 49-1

                            |                                     Mappint Type                         |
Visibility of modifications |-------------------------------------------|------------------------------|
                            | File                                      | Anonymous                    |
----------------------------|-------------------------------------------|------------------------------|
Private                     | Initializing memory from contents of file | Memory allocation            |
----------------------------|-------------------------------------------|------------------------------|
Shared                      | Memory-mapped I/O; sharing memory between | Sharing memory between       |
                            | processes (IPC)                           | processes (IPC)              |

