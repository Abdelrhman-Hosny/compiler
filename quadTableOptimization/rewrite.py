
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

splitted_lines = [ line[1:-2].split(',') for line in lines]

splitted_lines = [ [line[0].strip(), line[1].strip(), line[2].strip(), line[3].strip()] for line in splitted_lines]
print(splitted_lines)
# remove repeated labels
label_indices = []
jumping_lines = []
for i , (destination, op1, operation, op2) in enumerate(splitted_lines):
    if operation == "label":
        label_indices.append(i)
    elif operation == "goto":
        jumping_lines.append(i)

print(label_indices)
i = 0
same_indices = []
while i < len(label_indices) - 1:
    if label_indices[i] + 1 == label_indices[i + 1]:
        # equal indices is an array with labels that
        # are similar (we remove all of them except 1)
        equal_indices = [label_indices[i]]
        while (i < len(label_indices) - 1) and label_indices[i] + 1 == label_indices[i + 1]:
            equal_indices.append(label_indices[i + 1])
            i = i + 1
        
        same_indices.append(equal_indices)
    i = i + 1


for indices in same_indices:
    label_representative = indices[0]
    label_replace = splitted_lines[label_representative][0]
    label_list = [ splitted_lines[i][0] for i in indices ]
    for index in indices:
        splitted_lines[index][0] = label_replace
    
    for jump_line_index in jumping_lines:
        if splitted_lines[jump_line_index][0] in label_list:
            splitted_lines[jump_line_index][0] = label_replace


for indices in same_indices:
    if len(indices) == 1: continue

    for index in range(1, len(indices)):
        splitted_lines.remove(splitted_lines[indices[index]])


with open(quad_file_path, "w") as file:
    for line in splitted_lines:
        file.write(",".join(line) + "\n")

# now we have all destinations and operands