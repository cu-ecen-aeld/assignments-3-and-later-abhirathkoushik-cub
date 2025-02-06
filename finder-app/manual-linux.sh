#!/bin/bash
# Script outline to install and build kernel.
# Author: Siddhant Jajoo with modifications by Abhirath Koushik

set -e
set -u

OUTDIR=/tmp/aeld
KERNEL_REPO=git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
KERNEL_VERSION=v5.15.163
BUSYBOX_VERSION=1_33_1
FINDER_APP_DIR=$(realpath $(dirname $0))
ARCH=arm64
CROSS_COMPILE=aarch64-none-linux-gnu-

if [ $# -lt 1 ]
then
	echo "Using default directory ${OUTDIR} for output"
else
	OUTDIR=$1
	echo "Using passed directory ${OUTDIR} for output"
fi

mkdir -p ${OUTDIR}

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/linux-stable" ]; then
    #Clone only if the repository does not exist.
	echo "CLONING GIT LINUX STABLE VERSION ${KERNEL_VERSION} IN ${OUTDIR}"
	git clone ${KERNEL_REPO} --depth 1 --single-branch --branch ${KERNEL_VERSION}
fi
if [ ! -e ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ]; then
    cd linux-stable
    echo "Checking out version ${KERNEL_VERSION}"
    git checkout ${KERNEL_VERSION}

    # Set up environment
    export ARCH=arm64
    export CROSS_COMPILE=aarch64-none-linux-gnu-

    # Clean and configure kernel
    make mrproper
    make defconfig

    # Build VM Linux Target
    make -j4 all

    # Build Modules and Device Tree
    make modules
    make dtbs

fi

echo "Adding the Image in outdir"
cp ${OUTDIR}/linux-stable/arch/arm64/boot/Image ${OUTDIR}/

echo "Creating the staging directory for the root filesystem"
cd "$OUTDIR"
if [ -d "${OUTDIR}/rootfs" ]
then
	echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
    sudo rm  -rf ${OUTDIR}/rootfs
fi

# Create a new rootfs directory
mkdir -p ${OUTDIR}/rootfs
cd ${OUTDIR}/rootfs

mkdir -p bin dev etc home lib lib64 proc sbin sys tmp usr var
mkdir -p usr/bin usr/lib usr/sbin
mkdir -p var/log

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/busybox" ]
then
git clone git://busybox.net/busybox.git
    cd busybox
    git checkout ${BUSYBOX_VERSION}
    make distclean
    make defconfig
else
    cd busybox
fi

# After installing BusyBox to rootfs
sudo chown root:root ${OUTDIR}/busybox
sudo chmod 4755 ${OUTDIR}/busybox

make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE}
make CONFIG_PREFIX=${OUTDIR}/rootfs ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} install

echo "Library dependencies"
${CROSS_COMPILE}readelf -a ${OUTDIR}/rootfs/bin/busybox | grep "program interpreter"
${CROSS_COMPILE}readelf -a ${OUTDIR}/rootfs/bin/busybox | grep "Shared library"

SYSROOT_PATH=$(${CROSS_COMPILE}gcc -print-sysroot)

# Copy the Program Interpreter of the Cross Compiler Toolchain
cp ${SYSROOT_PATH}/lib/ld-linux-aarch64.so.1 ${OUTDIR}/rootfs/lib/

# Copy the Library Files of the Cross Compiler Toolchain
cp ${SYSROOT_PATH}/lib64/libm.so.6 ${OUTDIR}/rootfs/lib64/
cp ${SYSROOT_PATH}/lib64/libresolv.so.2 ${OUTDIR}/rootfs/lib64/
cp ${SYSROOT_PATH}/lib64/libc.so.6 ${OUTDIR}/rootfs/lib64/

# Creating the Device Nodes
sudo rm -f /dev/null
sudo mknod -m 666 /dev/null c 1 3

sudo rm -f /dev/console
sudo mknod -m 666 /dev/console c 5 1

# Clean and build the writer utility
cd ${FINDER_APP_DIR}
make clean
make cross CROSS_COMPILE=${CROSS_COMPILE}

# Copy the finder related scripts and executables to the /home directory on the target rootfs
cp writer ${OUTDIR}/rootfs/home/

cp finder.sh ${OUTDIR}/rootfs/home/
cp conf/assignment.txt ${OUTDIR}/rootfs/home/
cp conf/username.txt ${OUTDIR}/rootfs/home/
cp finder-test.sh ${OUTDIR}/rootfs/home/

# Copying the Autorun Qemu Script
cp autorun-qemu.sh ${OUTDIR}/rootfs/home/

# Chown the root directory
sudo chown -R root:root ${OUTDIR}/rootfs

# Create initramfs.cpio.gz
cd "$OUTDIR/rootfs"
find . | cpio -H newc -ov --owner root:root > ${OUTDIR}/initramfs.cpio
gzip -f ${OUTDIR}/initramfs.cpio
