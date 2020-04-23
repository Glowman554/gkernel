import os

files = os.listdir("./files")

build = "./make_initrd file.txt file.txt"

for i in range(0, len(files)):
    build = build + " ./files/" + files[i] + " " + files[i]
    
os.system(build)
