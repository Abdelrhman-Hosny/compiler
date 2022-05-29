
quad_file_path = "../src/quadruples.txt"

with open(quad_file_path, "r") as file:
    lines = file.readlines()

operations = {
    "label": "Label Definition",
    "goto": "Goto",
    "function": "Function Definition",
    "call": "Function Call",
    "param": "Function Parameter",
    "arg": "Function Argument",
}

print(lines)
splitted_lines = [ line[1:-2].split(',') for line in lines]

# remove unused variables
destinations = []
operands = []
for destination, op1, operation, op2 in splitted_lines:
    if operation not in operations.keys():
        destinations.append(destination)
        if op1 != "":
            operands.append(op1)
        if op2 != "":
            operands.append(op2)

# now we have all destinations and operands