#!/bin/python3
# Author: Liberty
# GitHub: liberty1997
# E-mail: liberty1997@yeah.net

import sys

def Zhuan(InputFile):
	with open(InputFile, encoding='gbk') as fileIn:
		content = fileIn.read()

	with open(InputFile.split('.')[0] + '_utf8.' + InputFile.split('.')[1], 'w', encoding='utf-8') as fileOut:
		fileOut.write(content)


if __name__ == '__main__':
	if len(sys.argv) == 1:
		print('''Function: transform GBK text file into UTF-8 text file \
				 \nUsage: {} file1 file2 file3 ... \
				 \nExample: {} book.txt stu.csv '''
				 .format(sys.argv[0], sys.argv[0]))
	for i in range(1,len(sys.argv)):
		Zhuan(sys.argv[i])
		print('Done.')
