import numpy as np
# delta = 1.0
# v = 0.0

# omega = np.sqrt(delta**2 + v**2)

# den = np.sqrt(2.0 * omega * (omega+delta))

# a = (omega+delta)/den
# b = v/den

# print (f"({a}, {b})")

a = 0.8
b = 0.6
slope = 2.0*a*b / (a**2 - b**2)

print(slope)