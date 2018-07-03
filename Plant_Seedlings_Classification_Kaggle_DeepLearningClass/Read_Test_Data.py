#!/bin/python3
import tensorflow as tf
import matplotlib.pyplot as plt

def Read_Test_TFRecords(files, resize):
	files = tf.train.match_filenames_once(files)
	filename_queue = tf.train.string_input_producer(files, shuffle = True)
	reader = tf.TFRecordReader()
	_, serialized_example = reader.read(filename_queue)
	features = tf.parse_single_example(serialized_example, features = \
		{'image': tf.FixedLenFeature([], tf.string), 'img_name': tf.FixedLenFeature([], tf.string)})
	image = tf.image.decode_png(features['image'], 3)
	image = tf.image.convert_image_dtype(image, dtype = tf.float32)
	image = tf.image.resize_images(image, [resize, resize])
	name = features['img_name']
	
	return image, name 

if __name__ == '__main__':
	image, name = Read_Test_TFRecords('Weed_InputData_Test*', 256)
	image_batch, name_batch = tf.train.batch([image, name], batch_size = 5)
	with tf.Session() as sess:
		sess.run([tf.global_variables_initializer(), tf.local_variables_initializer()])
		coord = tf.train.Coordinator()
		threads = tf.train.start_queue_runners(sess = sess, coord = coord)
		for i in range(3):
			a, b = sess.run([image_batch, name_batch])
			for j in b:
				print(j)
		coord.request_stop()
		coord.join(threads)
