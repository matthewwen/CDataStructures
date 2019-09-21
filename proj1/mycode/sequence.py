def smooth_number_3(n):
	res = []
	for i in range(0,n) :
		res.append(i)

	res[0] = 1
	p2, p3 = 0, 0

	for i in list(range(1, n)):
		if res[p2] * 2 == res[i - 1]:
			p2 = p2 + 1
		if res[p3] * 3 == res[i - 1]:
			p3 = p3 + 1
		u2 = res[p2] * 2
		u3 = res[p3] * 3
		if u2 < u3:
			p2 = p2 + 1
			res[i] = u2
		else:
			p3 = p3 + 1
			res[i] = u3
	return res

print("{}".format(smooth_number_3(20)))
