input = """"""

def do(name, index, value):
	trues = [name + "::East::Low", name + "::North::Low", name + "::South::Low", "true", name + "::West::Low"]
	falses = [name + "::East::None", name + "::North::None", name + "::South::None", "false", name + "::West::None"]
	if value == "true":
		return trues[index]
	return falses[index]

for line in input.split("\n"):
	if not "Wall(" in line:
		print(line)
		continue
	line = line.strip()
	start = line.find("(") + 1
	end = line.find(")", start)
	name = line.split("::")[0].split(" ")[1]
	result = ",".join(do(name, index, value) for index, value in enumerate(line[start:end].split(",")))
	print(line[0:start] + result + line[end:])
