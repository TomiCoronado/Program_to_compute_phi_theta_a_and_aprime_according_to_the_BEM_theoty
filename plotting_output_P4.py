import pandas as pd
from matplotlib import pyplot as plt 

output_P4 = pd.read_csv("output_P4.csv")

plt.figure(1)
plt.plot(output_P4.iloc[:,0],output_P4.iloc[:,1])
plt.title("a distribution")
plt.xlabel("radius")
plt.ylabel("a")

plt.figure(2)
plt.plot(output_P4.iloc[:,0],output_P4.iloc[:,2])
plt.title("a' distribution")
plt.xlabel("radius")
plt.ylabel("a'")

plt.figure(3)
plt.plot(output_P4.iloc[:,0],output_P4.iloc[:,3])
plt.title("Theta distribution")
plt.xlabel("radius")
plt.ylabel("theta")

plt.show()



