#!/bin/python3
import tensorflow as tf
import matplotlib.pyplot as plt

def Read_TFRecords(files):
	files = tf.train.match_filenames_once(files)
	filename_queue = tf.train.string_input_producer(files, shuffle = True)
	reader = tf.TFRecordReader()
	_, serialized_example = reader.read(filename_queue)
	features = tf.parse_single_example(serialized_example, features = \
		{'image': tf.FixedLenFeature([], tf.string), 'label': tf.FixedLenFeature([], tf.int64)})
	image = tf.image.decode_png(features['image'])
	label = tf.cast(features['label'], tf.int32)
	return image, label, files

def Preprocess(image, label, resize):
	image = tf.image.convert_image_dtype(image, dtype = tf.float32)
	print('Preprocess 111')
	print('SHAPE: ', image)
	print('--------SHAPE: ', image.eval())
	v = image.eval()
	print('Preprocess 222')
	'''
	plt.imshow(v)
	plt.show()
	'''
	height = len(v)
	width = len(v[0])
	channels = len(v[0][0])
	print('Preprocess 333')
	image.set_shape([height, width, channels])
	#image = tf.image.convert_image_dtype(image, dtype = tf.float32)
	print('Preprocess 444')
	image = tf.image.resize_images(image, [resize, resize], method = 0)
	label = tf.one_hot(label, 12, 1, 0, dtype = tf.float32)
	print('Preprocess 555')
	return image, label

if __name__	== '__main__':
	files = 'Weed_InputData.tfrecords*'
	batch_size = 3000
	min_after_dequeue = 1000
	resize = 300
	capacity = min_after_dequeue + 3 * batch_size
	image, label, Files = Read_TFRecords(files)
	with tf.Session() as sess:
		sess.run([tf.global_variables_initializer(), tf.local_variables_initializer()])
		coord = tf.train.Coordinator()
		threads = tf.train.start_queue_runners(sess = sess, coord = coord)
		image_t, label_t = Preprocess(image, label, resize)
		image_batch, label_batch = tf.train.shuffle_batch([image_t, label_t], batch_size = batch_size, capacity = capacity, min_after_dequeue = min_after_dequeue)
		print(image_batch, label_batch)
		#a, b = sess.run([image_t, label_t])
		#a, b = sess.run([image_batch, label_batch])
		'''
		for i in range(5):
			print('------------------------')
			print(sess.run([image_batch, label_batch]))
		'''
		coord.request_stop()
		coord.join(threads)
