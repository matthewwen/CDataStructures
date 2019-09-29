class element:
	two: int
	three: int

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

def print_list(list, limit):
	idx = 0
	while list[idx + 1] < limit:
		idx = idx + 1
	for i in range(idx, -1, -1):
		print(list[i])

list = smooth_number_3(10001)

#print_list(list, 1001)
print_list(list, 10001)
# print_list(list, 100001)
# print_list(list, 1000001)

