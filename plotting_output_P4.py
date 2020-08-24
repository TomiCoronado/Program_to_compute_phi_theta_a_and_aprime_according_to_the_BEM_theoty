import pandas as pd
from matplotlib import pyplot as plt 

output_P4 = pd.read_csv("output_P4.csv")

plt.plot(output_P4.iloc[:,0],output_P4.iloc[:,3])

""" plt.xlim(-5, 5)
plt.ylim(-5, 5)
plt.title("Phase Diagram")
plt.xlabel("y1")
plt.ylabel("y2") """
plt.show()



