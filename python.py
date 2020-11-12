import csv
a = []
b = []
with open('results.csv', newline='') as csvfile:
    spamreader = csv.reader(csvfile)
    for row in spamreader:
    	a.append(float(row[0]))
    	b.append(float(row[1]))

print(a)
print(b)

import matplotlib.pyplot as plt 

left = [1, 2.5, 4, 5.5, 7] 
height1 = a
label = ['HILL Climbing', 'HILL Climbing\nRandom Restart', 'HILL Climbing\nSimulated Annealing', 'Random Restart and\nSimulated Annealing', 'A* Optimal\nSolution']
plt.bar(left, height1, tick_label = label, width = 1.5, color = ['red', 'green']) 
plt.xlabel('\nAlgorithms used') 
plt.ylabel('Success Rate (out of 1)') 
plt.title('Success Rates for various algorithms') 
plt.show()
 
height2 = b
plt.bar(left, height2, tick_label = label, width = 1.5, color = ['blue', 'orange']) 
plt.xlabel('\nAlgorithms used') 
plt.ylabel('Search cost') 
plt.title('Search costs for various algorithms') 
plt.show()