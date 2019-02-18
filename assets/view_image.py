from PIL import Image, ImageTk 
import PIL
import tkinter as tk 
import sys

root = tk.Tk()

tkims = []


for filename in sys.argv[1:]:
    img = Image.open(filename)
    if filename.endswith('test.png'):
        resample = PIL.Image.BILINEAR
    else:
        resample = PIL.Image.NEAREST
    img = img.resize((300,300), resample)
    tkimage = ImageTk.PhotoImage(img)
    tkims.append(tkimage)
    tk.Label(root, image=tkimage).pack(side = "bottom", fill = "both", expand = "yes")

root.mainloop()
