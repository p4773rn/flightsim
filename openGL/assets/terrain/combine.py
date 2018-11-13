from PIL import Image
import numpy as np

alpha1 = Image.open('1.png')
w = alpha1.width
h = alpha1.height
alpha2 = Image.open('2.png')
alpha3 = Image.open('3.png')
alpha4 = Image.open('4.png')
combined = np.zeros((w, h, 4))

for i in range(w):
	for j in range(h):
		combined[j][i][0] = alpha1.getpixel((i, j))
		combined[j][i][1] = alpha2.getpixel((i, j))
		combined[j][i][2] = alpha3.getpixel((i, j))
		combined[j][i][3] = alpha4.getpixel((i, j))
img = Image.fromarray(np.uint8(combined))
img.save('alpha.png')
