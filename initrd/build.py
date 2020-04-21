import os

files = os.listdir("./files")

build = "./make_initrd"

for i in range(0, len(files)):
    build = build + " ./files/" + files[i] + " " + files[i]
    
os.system(build)