# 关于UEFI/GPT/ESP
---
##### **（本文的所有操作均在VirtualBox/Archlinux上实现）**

> 目前常用的主板分为UEFI和BIOS，硬盘的分区分为MBR和GPT，装Linux时有以下几种组合：
1. MBR+BIOS(推荐)
2. GPT+UEFI(推荐)
3. GPT+BIOS
4. MBR+UEFI

---
#### 1. MBR+BIOS
这种情况的装机方案已在之前的文章中写到，这里不再赘述。[附上链接](https://github.com/liberty1997/Share/blob/master/Archlinux_installation_guide.md)
#### 2. GPT+UEFI
> [UEFI](https://wiki.archlinux.org/index.php/Unified_Extensible_Firmware_Interface)：一种主板的新技术，据说比BIOS好。我不清楚不了解不说了。</br>
[GPT](https://wiki.archlinux.org/index.php/Partitioning)：硬盘的一种新的分区方案，旨在替换MBR

给一块新硬盘安装linux时，在MBR下，需要将启动区的boot flag激活；在UEFI下，实际上没有bootable flag，而是需要一个ESP分区——EFI System Partition。创建ESP的方法很简单，只需要设置此分区的两个属性：Size & Partition Type。[链接：创建ESP](https://wiki.archlinux.org/index.php/EFI_System_Partition)

GRUB的安装参照[链接](https://wiki.archlinux.org/index.php/GRUB)中“UEFI Systems”小节。

*在GTP+UEFI下，只要把ESP区设置好，GRUB配置好，其他步骤就和MBR+BIOS装Linux一样一样的了*
#### 3. GPT+BIOS
有些电脑的主板不支持UEFI，但是还想用GPT分区，也是可以的。**只是需要另设一个叫做"BIOS boot Partition"的分区**，有关此分区的作用及创建方法请看[链接](https://wiki.archlinux.org/index.php/GRUB)中"BIOS systems / GUID Partition Table (GPT) specific instructions"小节的讲解。
#### 4. MBR+UEFI(可能有些UEFI firmware不支持)
这种没试过，既然有了UEFI，还是享用GPT分区吧。

---
> Attentions
* UEFI启动，必须有ESP分区
* GPT+BIOS，分区方案尝试过ESP+BIOS boot Partition+/  ， / + BIOS boot Partition + /boot + /home ， 这两种都行 。 即BIOS boot Partition分区的位置可以不在磁盘开头（但此区必须有！），ESP分区可有可无



</br>


> References
* [Arch Wiki](https://wiki.archlinux.org/)

---
<p align="center"><a href="https://github.com/liberty1997">&copy;Liberty</a></p>
