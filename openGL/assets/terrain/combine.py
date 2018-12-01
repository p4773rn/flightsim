from PIL import Image
import numpy as np

'''alpha1 = Image.open('1.png')
w = alpha1.width
h = alpha1.height
alpha2 = Image.open('2.png')
alpha3 = Image.open('3.png')
alpha4 = Image.open('4.png')
combined = np.zeros((w, h, 4))'''

alpha = Image.open('alpha.png')
w = alpha.width
h = alpha.height
combined = np.zeros((w, h, 4))
for i in range(w):
	for j in range(h):
		r,g,b,a = alpha.getpixel((i,j))
		#print r,g,b,a
		combined[j][i][0] = r#alpha1.getpixel((i, j))
		combined[j][i][1] = g#alpha2.getpixel((i, j))
		combined[j][i][2] = b#alpha3.getpixel((i, j))
		combined[j][i][3] = 255.0 #alpha4.getpixel((i, j))
img = Image.fromarray(np.uint8(combined))
img = img.resize((128, 128), Image.ANTIALIAS)
img.save('amp.png')
img.close()
