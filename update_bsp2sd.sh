sudo gunzip -c SD/bpi-m2m/100MB/BPI-M2M-LCD7-8k.img.gz | dd of=$1 bs=1024 seek=8
sync
cd SD/bpi-m2m
sudo bpi-update -d $1
