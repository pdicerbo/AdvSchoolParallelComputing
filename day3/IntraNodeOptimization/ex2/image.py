import numpy as np
from PIL import Image
import ctypes as ct
import code
import argparse
from numpy.ctypeslib import ndpointer
from matplotlib import pyplot as plt


def argument():
    parser = argparse.ArgumentParser(description = '''
    Python to test keramagic improvement.
    ''',
    formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument(   '--image', '-i',
                                type = str,
                                required =False,
                                help = ''' Input image'''
                                )
  

    parser.add_argument(   '--operation', '-op',
                                type = int,
                                required = False,
                                default = 0,
                                choices=[0,1,2,3],
                                help = 'Number of operation')
  
    return parser.parse_args()


args = argument()
image = Image.open(args.image)
row,col = image.size
print("Image Dimension ",row,col)
image_array = np.asarray(image.getdata(),dtype='uint8')

#Channels table
#red_chanel    i=0
#green_chanel  i=1
#blu_chanel    i=2

new_row = row
new_col = col
channel = 3

red = image_array[:,0].copy()
green = image_array[:,1].copy()
blu = image_array[:,2].copy()

out_red = np.zeros(red.shape,dtype='uint8')
out_green = np.zeros((new_row*new_col),dtype='uint8')
out_blu = np.zeros((new_row*new_col),dtype='uint8')
out_Data = np.zeros((3),dtype='int32')

out_Data[0] = channel
out_Data[1] = row
out_Data[2] = col

lib = ct.cdll.LoadLibrary('./libimage.so')
lib.image_py.argtypes=([ct.c_int,
                        ct.c_int,
                        ct.c_int,
                        ndpointer(ct.c_ubyte, flags="aligned,C_CONTIGUOUS"),
                        ndpointer(ct.c_ubyte, flags="aligned,C_CONTIGUOUS"),
                        ndpointer(ct.c_ubyte, flags="aligned,C_CONTIGUOUS"),
                        ndpointer(ct.c_int, flags="aligned,C_CONTIGUOUS"),
                        ndpointer(ct.c_ubyte, flags="aligned,C_CONTIGUOUS"),
                        ndpointer(ct.c_ubyte, flags="aligned,C_CONTIGUOUS"),
                        ndpointer(ct.c_ubyte, flags="aligned,C_CONTIGUOUS")])

(lib.image_py( row,col,args.operation,red,green,blu,out_Data,out_red, out_green,out_blu))


if out_Data[0] == 3:
    image_array[:,0]=out_red.copy()
    image_array[:,1]=out_green.copy()   
    image_array[:,2]=out_blu.copy()
    img = image_array.reshape(out_Data[2],out_Data[1],out_Data[0]).astype(np.uint8)
    plt.imshow(img.astype(np.uint8),interpolation='none',aspect='auto')
    plt.show()

if out_Data[0] == 1:
    img = out_red.copy().reshape(out_Data[2],out_Data[1]).astype(np.uint8)
    plt.imshow(img.astype(np.uint8),interpolation='none',aspect='auto',cmap=plt.get_cmap('gray'))
    plt.show()