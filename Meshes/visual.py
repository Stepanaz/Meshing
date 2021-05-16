import numpy as np
import matplotlib.pyplot as plt
#import plotly.graph_objects as go
# Download data set from plotly repo

pts = np.loadtxt('mesh.txt')
x, y, z = pts.T

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.scatter3D(x, y, z, c=z, cmap='Greens');
#fig = go.Figure(data=[go.Mesh3d(x=x, y=y, z=z, color='lightpink', opacity=0.50)])
fig.show()
