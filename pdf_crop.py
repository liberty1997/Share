# -*- coding: UTF-8 -*-

def my_crop(fileIn, fileOut, widthB, widthE, heightB, heightE):
	r"""
	widthB, widthE --- 宽的起始与结束
	heightB, heightE --- 高的起始与结束
	Uh! This func only works well on some files.
	"""
	try:
		from PyPDF2 import PdfFileReader
		from PyPDF2 import PdfFileWriter
		from PyPDF2.pdf import PageObject
	except Exception as e:
		print(e)
		print('Sorry, you need to install PyPDF2 first!')
		print('You can run command `pip install PyPDF2 -i https://pypi.douban.com/simple` as root or administrator to handle this exception.')
		return
    
	# 创建PdfFileWriter实例
	output = PdfFileWriter()
	# 需要裁剪的PDF
	try:
		file1 = PdfFileReader(fileIn)
	except Exception as e:
		print(e)
		print('Please check the path to file '+fileIn+' !')
		return

	# 裁剪完的文件名
	outputStream = open(fileOut, 'wb')

	# PDF的页数
	pdfPages = file1.getNumPages()

	for i in range(pdfPages):
		# 默认每张的大小都一样，得到最后一张的尺寸：
		# 左下 右下 左上 右上
		if i == pdfPages-1 :
			print('LowerLeft:', file1.getPage(i).mediaBox.getLowerLeft())
			print('LowerRight:', file1.getPage(i).mediaBox.getLowerRight())
			print('UpperLeft:', file1.getPage(i).mediaBox.getUpperLeft())
			print('UpperRight:', file1.getPage(i).mediaBox.getUpperRight())
			print('Pages:', i+1)
		# 自己设定四个角的坐标，值为二元的tuple---(x,y)
		# 四个角是一个矩形，所以请注意坐标值之间的对应关系，否则没有裁剪作用
		file1.getPage(i).mediaBox.setLowerLeft((widthB,heightB))
		file1.getPage(i).mediaBox.setLowerRight((widthE,heightB))
		file1.getPage(i).mediaBox.setUpperLeft((widthB,heightE))
		file1.getPage(i).mediaBox.setUpperRight((widthE,heightE))
	
		# 将此页加入output
		output.addPage(file1.getPage(i))
		
		# 写入
		output.write(outputStream)
		outputStream.close()
		print('Input File:', fileIn)
		print('Output File:', fileOut)
		print('Success!')

'''
References:
	http://pythonhosted.org/PyPDF2/index.html
	http://www.docin.com/p-918745378.html
	https://github.com/mstamy2/PyPDF2

Contact:
	GitHub: https://github.com/liberty1997
	E-mail: liberty1997@yeah.net
'''
if __name__ == '__main__':
	my_crop('机器学习_周志华.pdf', '机器学习_周志华_CROP.pdf', 59, 524, 20, 600)
