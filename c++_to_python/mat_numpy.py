# nはnumpy.ndarray
import numpy as np
import cv2 as cv
def mul(n):
    print("from python")
    print("received: " + str(n))
    print("end from python")
    #mat_array=cv.fromarray(n)
    cv.imshow("depth",n)
    np.save('depth',n)
    # ndarrayのタプルを返す
 #   return (2 * n, 3* n)
    return (n,n)