# Generate 16-byte palette entries from 24-bit RGB-values

# 24-bit RGB-values to be converted to palette. This will output C initializations like:
#    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },    // 0 = blank
#    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },    // 1 = non-transparent black
#    { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 },    // 2 = white
#    { 1,0,0,1,0,0,1,1,1,1,0,1,1,0,1,1 },    // 3 = red, ghost 0
#    ...

rgb_vals = [
    [0,0,0,'blank'],
    [0,0,0,'non-transparent black'],
    [255,255,255,'white'],
    [200,0,0,'red, ghost 0'],
    [10,219,6,'green (fruits)'],
    [0,0,255,'blue'],
    [235,235,0,'yellow, Pacman'],
    [255,120,255,'pink, ghost 1'],
    [0,200,200,'cyan, ghost 2'],
    [255,184,81,'brown, ghost 3'],
    [35,121,178,'maze'],
    [226,162,154,'dots'],
    [255,184,81,'ghost door'],
    [255,106,0,'orange (logo)'],
    [255,142,142,'pink (fruits)'],
    [255,216,0,'yellow (fruits)']
]

# ----------------------------------------------------

def sRGBtoLinear(sRGB):
    #return 256*sRGB

    normalized = sRGB/255

    if normalized < 0.04045:
        linear = normalized / 12.92
    else:
        linear = ((normalized + 0.055)/1.055) ** 2.4

    linear16 = int(linear*65535)

    return linear16

# -------------------------------------------------------
# Main
    
r_bits = [0] * 16
g_bits = [0] * 16
b_bits = [0] * 16
palrow = [0] * 16

row = 0
for rgb in rgb_vals:
    r = sRGBtoLinear(rgb[0])
    g = sRGBtoLinear(rgb[1])
    b = sRGBtoLinear(rgb[2])
    c = rgb[3]

    #print(r,g,b,c)

    mask = 0x8000
    for i in range(16):
        if r & mask:
            r_bits[i] = 1
        else:
            r_bits[i] = 0
            
        if g & mask:
            g_bits[i] = 1
        else:
            g_bits[i] = 0
            
        if b & mask:
            b_bits[i] = 1
        else:
            b_bits[i] = 0
            
        mask = int(mask/2)

    #print('R:',r_bits)
    #print('G:',g_bits)
    #print('B:',b_bits)
    
    for i in range(16):
        palrow[i] = r_bits[i] + 2*g_bits[i] + 4*b_bits[i]

    print('    { ',end='')
    for i in range(15):
        print('%d,' % palrow[i],end='')
    print('%d },    // %d = %s' % (palrow[15],row,c))

    row += 1

