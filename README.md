<h1 align="center">
    ArcV
</h1>

<h4 align="center">Computer Vision &amp; Augmented Reality library</h4>

---

## Summary

**ArcV** (pronounced "Ark V") stands for "**A**ugmented **R**eality and **C**omputer **V**ision [Library]" (this name may change in the future, not all that original as it is, I know).

Written in C++, it is primarily developed for learning purposes. It is intended to be optimized and to run relatively fast, even on low-end/old computers and smartphones.

This library is available for Linux and Windows. It is also especially aimed to be running on Android.

If you want to get started, [head to the wiki](https://github.com/Razakhel/ArcV/wiki)!

# ArcV is...

### ... A **C**omputer **V**ision library

`Computer Vision` implies vision by... computer. That being said, how can there be vision without a bit of image processing?

| Objectives | Implemented |
| :--------: | :---------: |
| Implementing Matrix/Vector classes | **Yes** |
| Loading (reading) an image (JPEG/PNG/TGA/BMP/BPG) | **Yes** (PNG) |
| Saving (writing) an image | **Yes** (PNG) |
| Changing colorspace | **Yes** |
| Convolution filtering | **Yes** |
| Operations on images (rotation, flip, thresholding) | **Yes** |
| Detecting basic features (corners, lines, circles...) | _In progress_ |
| Detecting advanced features (object recognition, ORB algorithm) | No |
| Handle windowing | **Yes** |
| Getting stream from webcam | **Yes** (Linux) |

May be decided to be added:
- Facial recognition
- Motion analysis/tracking
- Optical flow
- Image restoration
- Scene reconstruction

### ... An **A**ugmented **R**eality library

The principle of `Augmented Reality` here is to place virtual elements over a picture or a video stream, this superposition having to be as realistic as possible.

This part will not be begun before a while: though being the most interesting, it makes less sense to move onto AR while CV isn't advanced enough.

| Objectives | Implemented |
| :---------: | :---------: |
| Fiducial markers recognition | No |
| Pose estimation | No |
| 3D models integration (OpenGL/Vulkan) | No |

To be continued!

## External links

Links to Wikipedia:
- [Computer Vision](https://en.wikipedia.org/wiki/Computer_vision)
- [Convolution](https://en.wikipedia.org/wiki/Kernel_(image_processing))
- [Edge detection](https://en.wikipedia.org/wiki/Edge_detection)
  - [Canny detector](https://en.wikipedia.org/wiki/Canny_edge_detector)
  - [Sobel operator](https://en.wikipedia.org/wiki/Sobel_operator)
- [Corner detection](https://en.wikipedia.org/wiki/Corner_detection)
  - [Harris operator](https://en.wikipedia.org/wiki/Corner_detection#The_Harris_.26_Stephens_.2F_Plessey_.2F_Shi.E2.80.93Tomasi_corner_detection_algorithms)
- [Augmented Reality](https://en.wikipedia.org/wiki/Augmented_reality)
  - [Fiducial marker](https://en.wikipedia.org/wiki/Fiducial_marker)
