f = open("serdump.txt", "r")
z = f.readlines()
data = sorted(z, key = lambda x: int(x[0]))
g = open("sorteddump.txt", "w")
for line in data:
    g.write(line)
g.close()