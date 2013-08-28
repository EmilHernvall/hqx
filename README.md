hqx
===

PHP extension wrapping the hqx library. 

Installing
----------

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

Usage
-----

This library declares three functions: image_scale_hqx2, image_scale_hqx3 and 
image_scale_hqx4. All of them accepts a gd image resource and returns another
one for the scaled image.

Sample usage:
    
    <?php
    $src = imagecreatefrompng($filename);
    $final = image_scale_hqx4($src);
    header('Content-type: image/png');
    imagepng($final);
    imagedestroy($src);
    imagedestroy($final);
    ?>
