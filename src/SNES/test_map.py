def lorom(address):
    bank = address&0x7f0000
    address&=0x7fff
    address|=bank>>1
    return address

def unmap_lorom(address):
    bank = address&0x3f8000
    address&=0x00ffff
    address|=bank<<1
    address|=0x808000
    return address

#ADC
print(hex(lorom(0x8000)))
#print(hex(lorom(0x00ffc0)))
    