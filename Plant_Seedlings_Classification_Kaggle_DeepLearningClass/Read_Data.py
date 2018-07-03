#!/bin/python3
import tensorflow as tf
import matplotlib.pyplot as plt

def Read_Test_TFRecords(files, resize):
	files = tf.train.match_filenames_once(files)
	filename_queue = tf.train.string_input_producer(files, shuffle = False)
	reader = tf.TFRecordReader()
	_, serialized_example = reader.read(filename_queue)
	features = tf.parse_single_example(serialized_example, features = \
		{'image': tf.FixedLenFeature([], tf.string), 'img_name': tf.FixedLenFeature([], tf.string)})
	image = tf.image.decode_png(features['image'], 3)
	image = tf.image.convert_image_dtype(image, dtype = tf.float32)
	image = tf.image.resize_images(image, [resize, resize])
	name = features['img_name']
	
	return image, name

def Read_TFRecords(files):
	files = tf.train.match_filenames_once(files)
	filename_queue = tf.train.string_input_producer(files, shuffle = True)
	reader = tf.TFRecordReader()
	_, serialized_example = reader.read(filename_queue)
	features = tf.parse_single_example(serialized_example, features = \
		{'image': tf.FixedLenFeature([], tf.string), 'label': tf.FixedLenFeature([], tf.int64)})
	'''
	features = tf.parse_single_example(serialized_example, features = \
		{'image': tf.FixedLenFeature([], tf.string), 'label': tf.FixedLenFeature([1], tf.int64), \
		'height': tf.FixedLenFeature([1], tf.int64), 'width': tf.FixedLenFeature([1], tf.int64), \
		'channels': tf.FixedLenFeature([1], tf.int64)})
	decode_png时用RGB模式(数字3)
	之前是想从tfrecord中读取height/width/channels然后用reshape，
	但是总是有错
	'''
	image = tf.image.decode_png(features['image'], 3)
	label = tf.cast(features['label'], tf.int32)

	return image, label

def Preprocess(image, label, resize):
	image = tf.image.convert_image_dtype(image, dtype = tf.float32)
	image = tf.image.resize_images(image, [resize, resize])
	label = tf.one_hot(label, 12, 1., 0., dtype = tf.float32)

	return image, label

if __name__ == '__main__':
	batch_size = 10
	min_after_dequeue = 1000
	capacity = min_after_dequeue + 3 * batch_size
	num_threads = 2
	image, label = Read_TFRecords('Weed_InputData_Valid*')
	image_p, label_p = Preprocess(image, label, 256)
	image_batch, label_batch = tf.train.batch([image_p, label_p], batch_size = batch_size)
	#image_batch, label_batch = tf.train.shuffle_batch([image_p, label_p], batch_size = batch_size, capacity = capacity, min_after_dequeue = min_after_dequeue, num_threads = num_threads)
	with tf.Session() as sess:
		sess.run([tf.global_variables_initializer(), tf.local_variables_initializer()])
		coord = tf.train.Coordinator()
		threads = tf.train.start_queue_runners(sess = sess, coord = coord)
		for i in range(12):
			'''
			a, b = sess.run([image_batch, label_batch])
			plt.imshow(a[0])
			plt.show()
			'''
			a, b = sess.run([image_batch, label_batch])
			print(b)
		coord.request_stop()
		coord.join(threads)
