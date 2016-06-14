BlobDetective
=============
BlobDetective is a blob tracker that sends OSC messages.

Author: Alexandre Quessy <alexandre@quessy.net>
		Cédric Arlen-Pouliot <cedric_arlen@hotmail.com>
Date: 2016

Technologies: C++, OpenCV, liblo

Installation instructions
-------------------------
See INSTALL.txt

Usage and options
-----------------
Colors are inverted, so 0 means white, not black, and 255 means black.

The print help message::

    $ ./blobdetective -h
    Usage: blobdetective [options]
     -h,--help Print help and exit
     -v,--verbose Set output to verbose
     -l List options and exit
     -d [value] Set the video device identifier. /dev/video0
     -o [name] [value] Sets an option value

List of the options::

    $ ./blobdetective -l   
    -o identifier "default"
	-o osc_send_host "127.0.0.1"
    -o osc_send_port 31340
    -o verbose (true - false)
    -o video_device_id "0"
    -o video_height 480
    -o video_width 640
	-o trackLightBlob (0 - 1)
	-o threshold (0 - 255)
    -o filterByArea (0 - 1)
	-o minArea (0 - 20000)
	-o maxArea (0 - 20000)
    -o filterByCircularity (0 - 1)
	-o minCircularity (0 - 100)
	-o maxCircularity (0 - 100)
    -o filterByConvexity (0 - 1)
	-o minConvexity (0 - 100)
	-o maxConvexity (0 - 100)
    -o filterByInertia (0 - 1)
	-o minInertiaRatio (0 - 100)
	-o maxInertiaRatio (0 - 100)



    
