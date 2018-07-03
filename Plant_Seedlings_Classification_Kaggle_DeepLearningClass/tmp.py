#!/bin/python3

import os, random

path = '/mnt/windows_E/DeepLearningClass/TestData/'
p = os.listdir(path)
p.sort()
with open('TestData_filenames.txt', 'w') as fw:
	fw.write('file\n')
	for i in range(len(p)):
		name, suffix = p[i].split('.')
		fw.write(name + '\n')
'''
path0 = '/mnt/windows_E/DeepLearningClass/Data/'
path1 = './DataAugmentation/'
class0 = os.listdir(path0)
class1 = os.listdir(path1)
class0.sort()
class1.sort()
photos = []
for i in range(len(class0)):
	pics = os.listdir(path0 + class0[i])
	for j in range(len(pics)):
		photos.append(path0 + '#' + class0[i] + '#' + pics[j])
for i in range(len(class1)):
	pics = os.listdir(path1 + class1[i])
	for j in range(len(pics)):
		photos.append(path1 + '#' + class1[i] + '#' + pics[j])
random.shuffle(photos)
with open('Valid_Augmentation_120_.txt', 'w') as fw:
	fw.write('\n'.join(photos[:120]) + '\n')
'''
'''
path = '/mnt/windows_E/DeepLearningClass/Data/'
class_ = os.listdir(path)
fw = open('Valid_120.txt', 'w')
cc = 0
for i in range(len(class_)):
	img = os.listdir(path + class_[i])
	img.sort()
	cc += len(img)
	for j in range(5):
		fw.write(class_[i] + '#' + img[j] + '\n')
	c = 1
	while c < 6:
		fw.write(class_[i] + '#' + img[len(img) - c] + '\n')
		c += 1
fw.close()
print(cc)
'''
'''
import tensorflow as tf
import matplotlib.pyplot as plt
from PIL import Image

path = '/mnt/windows_E/DeepLearningClass/Data/Black-grass/'
files = os.listdir(path)
files.sort()
img = tf.gfile.FastGFile(path + files[0], 'rb')
#shape = Image.open(path + files[0])
iread = img.read()
img_mat = tf.image.decode_png(iread)
img_mat = tf.image.convert_image_dtype(img_mat, dtype = tf.float32)
img_mat = tf.image.resize_images(img_mat, [300, 300])
with tf.Session() as sess:
	sess.run(tf.global_variables_initializer())
	a = sess.run(img_mat)
	plt.imshow(a)
	plt.show()
'''
