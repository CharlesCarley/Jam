
import glob

print("# Icons")
print("")

id = 0
svg = glob.glob('ic_*.svg') 
for file in svg:
    file = file.replace("\\", "/")
    print("[![Icon%i](%s)](%s)"%(id, file, file))
    id+=1


print("")
print("")
