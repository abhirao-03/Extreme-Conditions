# THIS VISUALISATION SCRIPT WAS WRITTEN IN WHOLE BY Gemini-3-Pro AS A PLACE HOLDER.
# IN THE FUTURE, I PLAN TO USE VISIT/PARAVIEW TO VISUALISE LARGE-SCALE SIMULATIONS.

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
import sys

# Configuration
INPUT_FILE = './simulation_output.csv'
OUTPUT_VIDEO = './simulation.gif'
VAR_TO_PLOT = 'density'  # Options: density, x_momentum, y_momentum, energy, pressure, velocity
FPS = 30

print(f"Reading data from {INPUT_FILE}...")
try:
    # Read the CSV
    # If file is HUGE (multi-GB), we might need to use chunking, 
    # but for typical 2D sims < 2GB, this is fine.
    df = pd.read_csv(INPUT_FILE)
except FileNotFoundError:
    print("Error: Could not find simulation_output.csv")
    sys.exit(1)

print("Processing data structure...")

# 1. Extract Grid Dimensions
# We assume the grid is Cartesian.
unique_x = np.sort(df['x'].unique())
unique_y = np.sort(df['y'].unique())
unique_t = np.sort(df['timestep'].unique())

Nx = len(unique_x)
Ny = len(unique_y)
Nt = len(unique_t)

print(f"Grid Size: {Nx} x {Ny}")
print(f"Time Steps: {Nt}")

# 2. Sort data to ensure correct reshaping
# We sort by timestep, then Y, then X to match typical array layout
df = df.sort_values(by=['timestep', 'y', 'x'])

# 3. Derive extra variables (Optional)
if VAR_TO_PLOT == 'pressure' or VAR_TO_PLOT == 'velocity':
    gamma = 1.4
    rho = df['density'].values
    mx = df['x_momentum'].values
    my = df['y_momentum'].values
    E = df['energy'].values
    
    # Avoid divide by zero
    rho[rho == 0] = 1e-6
    
    vx = mx / rho
    vy = my / rho
    v_sq = vx**2 + vy**2
    
    if VAR_TO_PLOT == 'pressure':
        df['pressure'] = (gamma - 1.0) * (E - 0.5 * rho * v_sq)
    elif VAR_TO_PLOT == 'velocity':
        df['velocity'] = np.sqrt(v_sq)

# 4. Reshape Data into (Time, Y, X)
# The values() call returns a flattened array, we reshape it to 3D
try:
    data_3d = df[VAR_TO_PLOT].values.reshape(Nt, Ny, Nx)
    time_values = df['time'].unique() # Get actual time values for the title
except ValueError as e:
    print("Error during reshaping. Your CSV might have incomplete frames.")
    print(f"Expected {Nt*Ny*Nx} rows, got {len(df)}")
    sys.exit(1)

# 5. Setup Plotting
fig, ax = plt.subplots(figsize=(8, 6))

# Determine global min/max for colorbar stability
vmin = np.min(data_3d)
vmax = np.max(data_3d)

# Initial Plot (Frame 0)
# extent=[min_x, max_x, min_y, max_y] maps array indices to physical units
extent = [unique_x[0], unique_x[-1], unique_y[0], unique_y[-1]]

img = ax.imshow(data_3d[0], 
                origin='lower', # Important: puts (0,0) at bottom left
                extent=extent, 
                cmap='inferno',     # 'jet' or 'inferno' look good for shocks
                vmin=vmin, 
                vmax=vmax,
                interpolation='nearest') # 'bilinear' makes it smooth, 'nearest' shows pixels

plt.colorbar(img, label=VAR_TO_PLOT.capitalize())
ax.set_xlabel("X")
ax.set_ylabel("Y")
title_text = ax.set_title(f"Time: {time_values[0]:.4f}")

print("Generating animation...")

def update(frame):
    # Update the image data
    img.set_data(data_3d[frame])
    # Update the title
    title_text.set_text(f"Time: {time_values[frame]:.4f}")
    return img, title_text

anim = animation.FuncAnimation(fig, update, frames=Nt, interval=1000/FPS, blit=False)

# 6. Save or Show
if "-save" in sys.argv or True: # Force save for now
    try:
        print(f"Saving to {OUTPUT_VIDEO}...")
        anim.save(OUTPUT_VIDEO, writer='ffmpeg', fps=FPS, dpi=150)
        print("Done!")
    except Exception as e:
        print(f"Could not save video (ffmpeg might be missing). Error: {e}")
        plt.show()
else:
    plt.show()