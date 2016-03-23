BlobDetective
=============
BlobDetective is a blob tracker that sends OSC messages.

Author: Alexandre Quessy <alexandre@quessy.net>
Date: 2016

Technologies: C++, OpenCV, liblo

Installation instructions
-------------------------
See INSTALL.txt

Usage and options
-----------------
The print the help message::

    $ ./blobdetective -h
    Usage: blobdetective [options]
     -h,--help Print help and exit
     -v,--verbose Set output to verbose
     -l List options and exit
     -d [value] Set the video device identifier. /dev/video0
     -o [name] [value] Sets an option value

To list the options::

    $ ./blobdetective -l
    -o blobColor 0
    -o filterByArea true
    -o filterByCircularity false
    -o filterByColor true
    -o filterByConvexity false
    -o filterByInertia true
    -o identifier "default"
    -o maxArea 10000
    -o maxThreshold 50
    -o minArea 1500
    -o minCircularity 0.2
    -o minConvexity 0.95
    -o minDistBetweenBlobs 10
    -o minInertiaRatio 0.1
    -o minRepeatability 2
    -o minThreshold 0
    -o osc_send_host "127.0.0.1"
    -o osc_send_port 31340
    -o thresholdStep 10
    -o verbose false
    -o video_device_id "0"
    -o video_height 480
    -o video_width 640
