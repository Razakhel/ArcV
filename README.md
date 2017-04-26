# ArcV
Computer Vision &amp; Augmented Reality library.

## Summary

**ArcV** (pronounced "Ark V") stands for "**A**ugmented **R**eality and **C**omputer **V**ision [Library]" (this name may change in the future, not all that original as it is, I know).

Written in C++, it is primarily developed for learning purposes. It is intended to be optimized and to run relatively fast, even on low-end/old computers and smartphones.

This library is made for Linux, though it is planned to make it fully compatible Windows. It is also especially aimed to be running on Android.


# ArcV is...

### ... A **C**omputer **V**ision library

`Computer Vision` implies vision by... computer. That being said, how can there be vision without a bit of image processing?

| Objectives | Implemented |
| :--------: | :---------: |
| Creating windowing class (Windows, Linux/X) | _On standby_ |
| Implementing Matrix/Vector classes | **Yes** |
| Loading (reading) an image (JPEG/PNG/...) | **Yes** (PNG) |
| Saving (writing) an image | **Yes** (PNG) |
| Getting stream from webcam | No |
| Operations on image matrices (change color model, apply convolution, etc) | _In progress_ |
| Operations on colors (thresholding...) | No |
| Detecting basic features (lines, circles...) | No |
| Detecting advanced features (objects recognition, ORB algorithm) | No |

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
| Integration of 3D models into the scene (OpenGL/Vulkan) | No |

To be continued!

## External links

Links to Wikipedia:
- [Computer Vision](https://en.wikipedia.org/wiki/Computer_vision)
- [Augmented Reality](https://en.wikipedia.org/wiki/Augmented_reality)
