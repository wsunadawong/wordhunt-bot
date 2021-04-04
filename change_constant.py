
string = '''
{
  {103, 89, 79, 69},
  {89, 75, 60, 43},
  {82, 63, 45, 29},
  {77, 56, 38, 19}
};
'''
arr = eval(string.strip().replace('{', '[').replace('}', ']').replace(';', ''))
print(arr)

k = 3

for i in range(4):
    for j in range(4):
        arr[i][j] = arr[i][j] + k

print(str(arr).replace('[', '\n{').replace(']', '}') + ';')
