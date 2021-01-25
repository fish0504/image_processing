# nはnumpy.ndarray
import numpy as np
import cv2 as cv

print("from python")
#n=cv.imread("/home/kawahara/dex-net-withoutdocker/gqcnn/data/examples/single_object/primesense/color_0.png")
n=np.load("/home/kawahara/programs/image_input/use_intelRealsense/Pydepth.npy")

print("received matrix ")# + str(n))
print("end from python")

cv.imshow("depth",n)
cv.waitKey()

