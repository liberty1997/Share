#!/bin/python3
import tensorflow as tf
import os

def _int64_feature(v):
	return tf.train.Feature(int64_list = tf.train.Int64List(value = [v]))
	
def _bytes_feature(v):
	return tf.train.Feature(bytes_list = tf.train.BytesList(value = [v]))

def Write_TFRecords(path, output):
	img = os.listdir(path)
	img.sort()
	writer = tf.python_io.TFRecordWriter(output)
	for i in range(len(img)):
		print(i, img[i])
		image = tf.gfile.FastGFile(path + img[i], 'rb')
		img_str = image.read()
		height, width, channels = tf.image.decode_png(img_str).eval().shape
		example = tf.train.Example(features = tf.train.Features(feature = \
		{'image': _bytes_feature(img_str), 'img_name': _bytes_feature(bytes(img[i], encoding = 'utf8'))}))
		writer.write(example.SerializeToString())
	writer.close()

if __name__ == '__main__':
	path = '/mnt/windows_E/DeepLearningClass/TestData/'
	output = './Weed_InputData_Final_Test.tfrecords'
	with tf.Session() as sess:
		sess.run(tf.global_variables_initializer())
		Write_TFRecords(path, output)
