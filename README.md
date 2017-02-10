# vice-redpill v0.1

This is the initial version of the vice-redpill that exposes 
the main RAM of each of the 11 machines of the VICE emulator.

### Download
git clone -b vice-3.0-redpill --depth 1 https://github.com/mathfigure/vice-emu

### Build & Install (linux/native)
cd vice-emu/vice  
./autogen.sh  

 # option 1 - install to your system directories  
./configure LIBS=-lrt --enable-gnomeui3  
make  
sudo make install  

 # option 2 - install to your home directory  
./configure LIBS=-lrt --enable-gnomeui3 --prefix=$HOME/vice-redpill  
make  
make install  
PATH=$PATH:~/vice-redpill/bin
 
### Run
machine| /dev/shm | size
-------|----------|-----
x64    |C64_RAM   | 64K
x64sc  |C64_RAM   | 64K
xscpu64|C64_RAM   | 64K
vsid   |C64_RAM   | 64K
x64dtv |C64DTV_RAM|  2M
x128   |C128_RAM  |256K
xvic   |VIC20_RAM | 64K
xplus4 |PLUS4_RAM | 64K
xpet   |PET_RAM   |128K
xcbm2  |CBM2_RAM  |  1M
xcbm5x0|CBM2_RAM  |  1M

### Tests
hexedit /dev/shm/C64_RAM  
cat /dev/shm/C64_RAM | od -Ax -t x1 -v | less  
watch -d -n0 "cat /dev/shm/C64_RAM | od -Ax -vtx1 -w40 -j 1024 -N 1000"  
ffplay /dev/shm/C64_RAM -f rawvideo -video_size 256x256 -pixel_format gray -framerate 50 -loop 0  
gst-launch-1.0 multifilesrc location=/dev/shm/C64_RAM ! videoparse width=256 height=256 framerate=50 format=25 ! videoconvert dither=0 ! videoscale method=0 ! agingtv ! ximagesink
