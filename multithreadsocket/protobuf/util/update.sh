#!/bin/sh
uboot_file="/run/media/sda1/u-boot-imx6qsabresd_sd.imx"
kernel_file="/run/media/sda1/zImage"
dtb_file="/run/media/sda1/zImage-imx6q-sabresd.dtb"
rootfs_file="/run/media/sda1/rootfs.tar.bz2"
echo "update for instrument panel" > /usr/osmeter/a

sleep 3
echo "update for instrument panel"                 
if [ -e $rootfs_file ]                             
then                                               
echo "update rootfs start" > /usr/osmeter/a        
#echo "burn rootfs\n"                      
touch /usr/osmeter/b                       
mkdir -p /mnt/mmcblk3p2                    
mount -t ext3 /dev/mmcblk3p2 /mnt/mmcblk3p2
#tar -jxvf /run/media/sda1/rootfs.tar.bz2 -C /mnt/mmcblk3p2
tar -jxf $rootfs_file -C /mnt/mmcblk3p2                    
echo "update rootfs finish" > /usr/osmeter/a               
echo "update rootfs finish"                                
sync                                                       
else                                                       
echo "update rootfs finish" > /usr/osmeter/xc              
echo "update rootfs failed"                                
fi                                           
                                             
touch /usr/osmeter/d                         
echo "update finish" > /usr/osmeter/a   