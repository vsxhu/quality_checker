fin = open("./Benchmarks/m1_straightlines_image.txt","rb")
linecnt = 0
for l in fin:
	line = l.strip()
	if line:
		linecnt+=1
		s = l.split(' ')
		print len(s)

print "\n\n",linecnt
