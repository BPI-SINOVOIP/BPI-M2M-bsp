## **BPI-M2M-bsp**
Banana Pi M2M board bsp (u-boot 2011.09 & Kernel 3.4.39)


----------
**Prepare**

Get the docker image from [Sinovoip Docker Hub](https://hub.docker.com/r/sinovoip/bpi-build/) , Build the source code with this docker environment.

 **Build**

Get support boards, please run

    #./build.sh

Build a target board bsp packages, please run

`#./build.sh <board> 1`

Target download packages in SD/bpi-*/ after build. Please check the build.sh and Makefile for detail

**Install**

Get the image from [bpi](http://wiki.banana-pi.org/Banana_Pi_BPI-M2U#Image_Release) and download it to the SD card. After finish, insert the SD card to PC

    # ./build.sh <board> 6

Choose the type, enter the SD dev, and confirm yes, all the build packages will be installed to target SD card.
