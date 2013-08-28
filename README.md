hqx
===

PHP extension wrapping the hqx library. 

You need to download, build and install hqx from https://code.google.com/p/hqx/ 
in order to build this.

Install instructions for ubuntu:

    wget https://hqx.googlecode.com/files/hqx-1.1.tar.gz
    tar zxvf hqx-1.1.tar.gz
    cd hqx-1.1
    ./configure --prefix=/usr
    make
    sudo make install

You'll also need some headers:

    sudo apt-get install php5-dev libgd-dev
    
Then checkout this repository and do:

    phpize
    ./configure --with-gd=/usr
    make
    sudo make install
    
You might have to update your php configuration to load the module.
