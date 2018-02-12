# Archlinux安装教程(MBR+BIOS)
---

#### 1. 分区
分区工具: 推荐使用cfdisk</br>
若是装纯Arch，分区完成后要对自选的某个分区进行Bootable操作('Toggle bootable flag of the current partition')，不然装好后无法进入系统。
#### 2. 安装文件系统
假如分区是(假定只有一块硬盘sda)

sda1|sda2|sda3|sda4
---|---|---|---
/|/boot|swap|/home
```shell
mkfs.ext4 /dev/sda1
mkfs.ext4 /dev/sda2
mkfs.ext4 /dev/sda4
```
#### 3. 挂载

```shell
mount /dev/sda1 /mnt
cd /mnt
mkdir boot
mkdir home
mount /dev/sda2 boot
mount /dev/sda4 home
mkswap /dev/sda3
swapon /dev/sda3
```
#### 4. 更新源
找到 /etc/pacman.d/mirrorlist ，删除此文件里的内容，然后添加国内源

以下为本人常用国内源
```
Server = http://mirrors.tuna.tsinghua.edu.cn/archlinux/$repo/os/$arch
Server = http://mirrors.zju.edu.cn/archlinux/$repo/os/$arch
Server = http://mirrors.ustc.edu.cn/archlinux/$repo/os/$arch
Server = http://ftp.yzu.edu.tw/Linux/archlinux/$repo/os/$arch
Server = http://shadow.ind.ntou.edu.tw/archlinux/$repo/os/$arch
Server = http://run.hit.edu.cn/archlinux/$repo/os/$arch
Server = http://mirror.metrocast.net/archlinux/$repo/os/$arch
Server = http://arch.serverspace.co.uk/arch/$repo/os/$arch
Server = http://mirror.nus.edu.sg/archlinux/$repo/os/$arch
Server = http://mirror3.gaw.solutions/arch/$repo/os/$arch
Server = http://mirror2.gaw.solutions/arch/$repo/os/$arch
Server = http://mirrors.cug.edu.cn/archlinux/$repo/os/$arch
Server = http://mirrors.cug6.edu.cn/archlinux/$repo/os/$arch
Server = http://mirrors.hust.edu.cn/archlinux/$repo/os/$arch
Server = http://mirrors.neusoft.edu.cn/archlinux/$repo/os/$arch
Server = http://mirrors.163.com/archlinux/$repo/os/$arch
Server = http://mirror.its.dal.ca/archlinux/$repo/os/$arch
Server = http://archlinux.cs.nctu.edu.tw/$repo/os/$arch
Server = http://mirrors.cqu.edu.cn/archlinux/$repo/os/$arch
Server = http://mirror.rackspace.com/archlinux/$repo/os/$arch
Server = http://arch-mirror.wtako.net/$repo/os/$arch
Server = http://mirror.lzu.edu.cn/archlinux/$repo/os/$arch
Server = http://mirrors.cat.pdx.edu/archlinux/$repo/os/$arch
Server = http://mirror.bjtu.edu.cn/archlinux/$repo/os/$arch
```
#### 5. 安装系统
`pacstrap /mnt base base-devel`
#### 6. 生成fstab
`genfstab -U /mnt >> /mnt/etc/fstab`
#### 7. chroot
`arch-chroot /mnt /bin/bash`
#### 8. 配置locale
```
vi /etc/locale.gen
# 将以下行开头的井号去掉
# en_US.UTF-8 UTF-8
# en_US ISO-8859-1
# zh_CN.GB18030 GB18030
# zh_CN.GBK GBK
# zh_CN.UTF-8 UTF-8
# zh_CN GB2312
# zh_HK.UTF-8 UTF-8
# zh_HK BIG5-HKSCS
# zh_SG.UTF-8 UTF-8
# zh_SG.GBK GBK
# zh_SG GB2312
# zh_TW.EUC-TW EUC-TW
# zh_TW.UTF-8 UTF-8
# zh_TW BIG5
locale-gen
echo LANG=en_US.UTF-8 >> /etc/locale.conf
```
#### 9. 配置时区
`ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime`
#### 10. 设置主机名
`echo 主机名 >> /etc/hostname`
#### 11. 生成ramdisk
`mkinitcpio -p linux`
#### 12. 配置用户
```shell
# 更改root密码
passwd
# 添加普通用户
useradd -m -g users -G wheel -s /bin/bash 用户名
passwd 用户名
```
#### 13. 安装GRUB
```shell
pacman -S grub # 如果是双系统，还需安装os-prober以检测win
grub-install --target=i386-pc /dev/sda
grub-mkconfig -o /boot/grub/grub.cfg
```
#### 14. 进入新系统
```
exit
umount /mnt/{home,boot}
cd ~
umount /mnt
reboot
```
#### 15. 安装驱动
```shell
# 进入新系统如果连着网线还没网，先执行dhcpcd
pacman -S alsa-utils mesa xf86-video-intel xf86-input-libinput
```
#### 16. END

现在Arch已经装好，剩下的桌面/软件等后续按需安装即可。

---

#### Attentions
* 如果是装双系统，安装前注意看分区类型，mbr分区形式要求主分区+扩展分区不超过4个
* 硬盘有RAID的，要先停止raid才能安装: cat /proc/mdstat;mdadm -S [设备号]   , [参见链接](http://blog.sina.com.cn/s/blog_406127500101e1nh.html)
* 单系统一定要记得Bootable
* 有个别时候，因为磁盘的原因或是win镜像文件的原因，os-prober会检测不到win系统，这时需要自己修改/boot/grub/grub.cfg中有关os-prober一节的配置，即手动添加win启动的一些配置信息(参见此md文件同目录下的grub.cfg)
---

#### References
* [Arch Wiki Installation guide](https://wiki.archlinux.org/index.php/Installation_guide)
* [Archlinux简明安装教程](https://my.oschina.net/codeaxe/blog/127533/?fromerr=7E0KEw31)
* [Arch Wiki General recommendations](https://wiki.archlinux.org/index.php/General_recommendations)
* [List of applications](https://wiki.archlinux.org/index.php/List_of_applications)

---

<p align="center"><a href="https://github.com/liberty1997">&copy;Liberty</a></p>
