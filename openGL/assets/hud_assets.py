from PIL import Image, ImageDraw, ImageColor, ImageFont
import math

white = ImageColor.getrgb('#ffffff')
black = ImageColor.getrgb('#000000')


size = 200
nums = range(0, 200, 20)


###############
# speedometer #
###############
im = Image.new('RGBA', (size, size), (0, 0, 0, 0))

font = ImageFont.truetype("arial.ttf", 20)
draw = ImageDraw.Draw(im)
draw.ellipse([0, 0, size, size], black)
draw.ellipse([size*0.05, size*0.05, size*0.95, size*0.95], white)


for num in nums:
    angle = num / nums[-1] * math.pi * 1.8 - math.pi / 2
    
    r1, r2, r3 = size*0.15, size*0.25, size*0.35

    cos_a, sin_a = math.cos(angle), math.sin(angle)
    
    x1, y1 = cos_a*r1, sin_a*r1
    x2, y2 = cos_a*r2, sin_a*r2
    x3, y3 = cos_a*r3, sin_a*r3
    x1 += size*0.5
    y1 += size*0.5
    x2 += size*0.5
    y2 += size*0.5

    w, h = draw.textsize(str(num), font)
    x3 += size*0.5 - w/2
    y3 += size*0.5 - h/2

    draw.line([x1, y1, x2, y2], black, 5)
    draw.text([x3, y3], str(num), black, font)

im.save('speedometer.png', 'PNG')


######################
# attitude indicator #
######################

im = Image.new('RGBA', (size, size), (0, 0, 0, 0))
draw = ImageDraw.Draw(im)


height = [0, 15, 30, 45, 60]
width = [50, 30, 70, 30, 90]
txt = ['', '', '10', '', '20']

for i in range(5):
    h, w, t = height[i], width[i], txt[i]
    x1, x2 = size * 0.5 - w * 0.5, size * 0.5 + w * 0.5
    y1, y2 = size * 0.5 + h, size * 0.5 - h
    
    draw.line([x1,y1,x2,y1], white, 2)
    draw.line([x1,y2,x2,y2], white, 2)
    
    x1 -= size * 0.1
    x2 += size * 0.1
    tw, th = draw.textsize(t, font)
    draw.text([x1 - tw*0.5, y1 - th*0.5], t, white,  font)
    draw.text([x2 - tw*0.5, y1 - th*0.5], t, white,  font)
    draw.text([x1 - tw*0.5, y2 - th*0.5], t, white,  font)
    draw.text([x2 - tw*0.5, y2 - th*0.5], t, white,  font)

im.save('attitude_indicator.png', 'PNG')




im = Image.new('RGBA', (size, size), (0, 0, 0, 0))
draw = ImageDraw.Draw(im)

y = size * 0.5
draw.line([size*0.4,y,size*0.6,y], (54, 205, 196, 255), 10)

im.save('attitude_indicator_background.png', 'PNG')

