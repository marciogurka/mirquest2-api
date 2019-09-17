my_file = open('hairpin.fa', 'r')
lines = my_file.readlines()
y = ''
fileText = ""
text = []
for line in lines:
    if line.rstrip("\r\n"):
        if line.rstrip("\r\n").find(">") == 0:
            if text:
                fileText = fileText + ''.join(text)
                fileText = fileText + "\n"
            fileText = fileText + line.rstrip("\r\n")
            fileText = fileText + "\n"
            y = ''
            text = []
        else:
            y = ''.join(line.rstrip("\r\n"))
            text.append(y)
fileText = fileText + ''.join(text)
print(fileText)

fh = open("test.txt", "w")
fh.write(fileText)
fh.close()