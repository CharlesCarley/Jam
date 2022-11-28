
import glob

print("")
print("<RCC>")
print("  <qresource prefix=\"/icons\">")
svg = glob.glob('ic_*.svg') 
for file in svg:

    file = file.replace("\\", "/")
    print("    <file>%s</file>"%file)
print("  </qresource>")
print("</RCC>")
