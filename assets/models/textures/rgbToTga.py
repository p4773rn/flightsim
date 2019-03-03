#code taken from https://pillow.readthedocs.io/en/stable/reference/Image.html
from PIL import Image
import glob
import os

for infile in glob.glob("*.rgb"):
	f, ext = os.path.splitext(infile)
	img = Image.open(infile)
	img.save(f + ".tga")