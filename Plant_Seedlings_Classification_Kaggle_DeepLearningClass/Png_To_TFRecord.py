#!/bin/python3
import tensorflow as tf
import os
import random

def _int64_feature(v):
	return tf.train.Feature(int64_list = tf.train.Int64List(value = [v]))
	
def _bytes_feature(v):
	return tf.train.Feature(bytes_list = tf.train.BytesList(value = [v]))

def Write_TFRecords(path, output):
	class0 = os.listdir(path[0])
	class1 = os.listdir(path[1])
	class0.sort()
	class1.sort()
	photos = []
	label_ = {}
	for i in range(len(class0)):
		label_[class0[i]] = i
		pics = os.listdir(path[0] + class0[i] + '/')
		for j in range(len(pics)):
			photos.append(path[0] + '#' + class0[i] + '#' + pics[j])
	for i in range(len(class1)):
		pics = os.listdir(path[1] + class1[i] + '/')
		for j in range(len(pics)):
			photos.append(path[1] + '#' + class1[i] + '#' + pics[j])
	random.shuffle(photos)
	with open('Valid_Augmentation_120.txt') as f:
		con = f.read()
	valid = con.splitlines()
	photos = list(set(photos) - set(valid))
	with open('HALF.txt') as f:
		con = f.read()
	h = con.splitlines()
	photos = list(set(photos) - set(h))
	#half = len(photos) // 2
	fw = open('HALF2.txt', 'w')
	writer = tf.python_io.TFRecordWriter(output)
	#for i in range(half):
	for i in range(len(photos)):
		fw.write(photos[i] + '\n')
		p, label, pic = photos[i].split('#')
		print(i, p, label, pic)
		pic_ = p + label + '/' + pic
		image = tf.gfile.FastGFile(pic_, 'rb')
		img_str = image.read()
		height, width, channels = tf.image.decode_png(img_str).eval().shape
		example = tf.train.Example(features = tf.train.Features(feature = \
			{'image': _bytes_feature(img_str), 'label': _int64_feature(label_[label]), \
			'name': _bytes_feature(bytes(pic, encoding = 'utf8'))}))
		writer.write(example.SerializeToString())
	writer.close()
	fw.close()

if __name__ == '__main__':
	path = ['/mnt/windows_E/DeepLearningClass/Data/', './DataAugmentation/']
	output = './Weed_InputData_Training_Augmentation.tfrecords02'
	with tf.Session() as sess:
		sess.run(tf.global_variables_initializer())
		Write_TFRecords(path, output)
