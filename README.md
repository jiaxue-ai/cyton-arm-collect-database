## use own dataset for deep filter banks 

Jia Xue

###   Introduction

This repository contains the code from [Deep-Reflectance-Code](https://github.com/zhanghang1989/Deep-Reflectance-Code.git), and [deep filter banks](https://github.com/mcimpoi/deep-fbanks.git), I used this algorithm to my dataset.

### Get Started

* The code relies on [vlfeat](http://www.vlfeat.org/), and [matconvnet](http://www.vlfeat.org/matconvnet), which should be downloaded and built before running the experiments. (Supprting the version [matconvnet-1.0-beta18](http://www.vlfeat.org/matconvnet/download/).)
You can use the following commend to download them:

		git clone --recurse-submodules https://github.com/mrxue1993/deep-filter-banks-wild-dataset.git
	
* Download the model [VGG-M](http://www.vlfeat.org/matconvnet/pretrained) to `data/models` (older models can also be updated using the `vl_simplenn_tidy` function).

* Download the following dataset to `data/`
    * Reflectance Disks [(reflectance)](https://goo.gl/6Kwg13)  
    * Flickr Material Database [(fmd)](http://people.csail.mit.edu/celiu/CVPR2010/FMD/) 
    * Describable Textures Dataset [(dtd)](http://www.robots.ox.ac.uk/~vgg/data/dtd)
    * Textures under varying Illumination [(kth)](http://www.nada.kth.se/cvap/databases/kth-tips/)

* `run_Experiments.m` reproducing general material recogniton results
