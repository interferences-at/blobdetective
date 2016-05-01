#!/bin/bash
# VIDEO_DEVICE=/dev/video0
VIDEO_DEVICE=/dev/video1

gst-launch-1.0 v4l2src device=${VIDEO_DEVICE} ! autovideosink
