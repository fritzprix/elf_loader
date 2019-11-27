# elf_loader

> simple ELF printer implementation to understand ELF format

## How To Build

```shell
$make all
```

> then executable 'elf_parser' is created under Debug/ directory.

## How To Use

```shell
$./elf_parser ELF_FILENAME
```

## Output Example
```
===============       ELF32 Header       ======================
======= Entry Address                        :      80256 =====
======= Bytes offset of Program Header       :         52 =====
======= Bytes size of Program Header Table   :          1 =====
======= Number of Entries in Program Header  :          8 =====
======= Bytes offset of Section Header Table :    2130088 =====
======= Bytes size of Section Header Table   :         40 =====
======= Number of entries in Section Header  :         37 =====
======= String Table Index                   :         34 =====
machine code : 40
Loadable Image Size : 389894 

==========       ELF32 Section    @               308  ==========
==========       Section Name      :          .interp  ==========
==========       Section Base Addr :            65844  ==========
==========       Section Size      :               24  ==========
==========       Section align     :                1  ==========
==========       Section Type      :     SHT_PROGBITS  ==========
==========       Section Attributes:  ALLOC | 
==========       Section Entry Size:                0  ==========

==========       ELF32 Section    @               332  ==========
==========       Section Name      :            .hash  ==========
==========       Section Base Addr :            65868  ==========
==========       Section Size      :             1700  ==========
==========       Section align     :                4  ==========
==========       Section Type      :         SHT_HASH  ==========
==========       Section Attributes:  ALLOC | 
==========       Section Entry Size:                4  ==========

==========       ELF32 Section    @              2032  ==========
==========       Section Name      :          .dynsym  ==========
==========       Section Base Addr :            67568  ==========
==========       Section Size      :             3616  ==========
==========       Section align     :                4  ==========
==========       Section Type      :       SHT_DYNSYM  ==========
==========       Section Attributes:  ALLOC | 
==========       Section Entry Size:               16  ==========
...
```

## LICENSE

BSD-3
