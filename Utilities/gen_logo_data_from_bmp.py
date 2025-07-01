# Generate logo
# Save bitmap as 8-bit!

# 88*44 matrix, 4 bits/px, 2 px/byte, max 16 colors
def printhex(prompt,hdata):
    print(prompt,end=' ')
    print(' '.join('{:02x}'.format(c) for c in hdata))

f = open('Logo.bmp','rb')
filecontents = f.read()
f.close()

# Mapping from bitmap palette -> program palette
#   0 -> blk
#   1 -> yel
#   2 -> ora
#   3 -> blk
palmap = [1,6,13]

# Number of colors
nrcdata = filecontents[0x2E:0x32]
nr_colors = int.from_bytes(nrcdata,byteorder='little')

imgoffsdata = filecontents[0x0A:0x0E]
img_offs = int.from_bytes(imgoffsdata,byteorder='little')

# Print palette from bitmap file
palette = filecontents[0x36:img_offs]

print('Palette (%d colors):' % nr_colors)
for i in range(nr_colors):
    pal_entry = palette[4*i:4*(i+1)]
    #printhex('--',pal_entry)
    r = int(pal_entry[2])
    g = int(pal_entry[1])
    b = int(pal_entry[0])
    print('  %d: [%3d,%3d,%3d]' % (i,r,g,b))

# Quit, if mapped color count does not match
if len(palmap) != nr_colors:
    print('palmap = %d colors, cannot convert' % len(palmap))
    quit()

# Bypass headers
bmpdata = filecontents[img_offs:]
#print(img_offs,len(bmpdata))

# Only logo area rows from bottom to up
# Pick only pixel rows 42...66
for rownr in range(66,42,-1):
    rowdata = bmpdata[88*rownr:88*(rownr+1)]

    #printhex('',rowdata)

    # Convert one row of data to array of 44 byte values
    byterow = []
    for bytenr in range(44):
        bytedata = rowdata[2*bytenr:2*(bytenr+1)]

        # Convert 2 bitmap pixels to one byte
        byte = 0
        for bitnr in range(2):
            bitdata = bytedata[bitnr:bitnr+1]
            bits = palmap[int.from_bytes(bitdata)]
            byte = byte*16 + bits
        byterow.append(byte)

    #print(byterow)

    # Print bytes as C initializer
    print('    { ',end='');
    for i in range(44):
        if i < 43:
            print('0x%02X,' % byterow[i],end='')
        else:
            print('0x%02X },' % byterow[i])
