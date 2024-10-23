from rapidfuzz import fuzz
import os
from re import sub

remove_strs = ['\(.{1,4}\)','\[.*\]']

fixed_names = {}

def fixed():
    for f in os.listdir('roms'):
        if f.split('.')[-1]=="nes":
            fixed_names[f] = f[:-4]
            for s in remove_strs:
                fixed_names[f] = sub(s,'',fixed_names[f])
            while fixed_names[f][-1]==" ":
                fixed_names[f] = fixed_names[f][:-1]
        else:
            fixed_names[f]=''
def calc_ratio(file,s):
    normal = fuzz.ratio(file,s)
    partial = fuzz.partial_ratio(file,s)
    token_sort = fuzz.token_sort_ratio(file,s)
    token_set = fuzz.token_set_ratio(file,s)
    w_ratio = fuzz.WRatio(file,s)
    final_ratio = (normal+partial+token_sort+token_set+w_ratio)/5
    return final_ratio
def proc(s):
    keyval = [(calc_ratio(fixed_names[f],s),calc_ratio(f,s),f) for f in os.listdir('roms')]
    return [j[2] for j in sorted([i for i in keyval if i[0]>=60],key=lambda v: (v[:2]))]

fixed()