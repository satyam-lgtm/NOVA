import subprocess
import tkinter as tk
from tkinter import messagebox
import json

# Default rocket data
rocket_data = {
    "rocket": {
        "length": 20.0,
        "diameter": 2.0,
        "wet_mass": 5000.0,
        "dry_mass": 2000.0
    },
    "propulsion": {
        "fuel_mass": 3000.0,
        "engines": [
            {
                "thrust": 100000.0,
                "burn_rate": 300.0,
                "efficiency": 0.9,
                "nozzle_diameter": 2.0
            }
        ]
    }
}

with open('src/config.json', 'w') as json_file:
    json.dump(rocket_data, json_file, indent=4)

# Function to update the JSON data based on user inputs
def save_data():
    try:
        # Extract values from the entry widgets
        rocket_data['rocket']['length'] = float(entry_length.get())
        rocket_data['rocket']['diameter'] = float(entry_diameter.get())
        rocket_data['rocket']['wet_mass'] = float(entry_wet_mass.get())
        rocket_data['rocket']['dry_mass'] = float(entry_dry_mass.get())
        
        rocket_data['propulsion']['fuel_mass'] = float(entry_fuel_mass.get())

        engine = rocket_data['propulsion']['engines'][0]
        engine['thrust'] = float(entry_thrust.get())
        engine['burn_rate'] = float(entry_burn_rate.get())
        engine['efficiency'] = float(entry_efficiency.get())
        engine['nozzle_diameter'] = float(entry_nozzle_diameter.get())
        
        with open('src/config.json', 'w') as json_file:
            json.dump(rocket_data, json_file, indent=4)

        # Success message
        messagebox.showinfo("Success", "Data saved successfully!")
    
    except ValueError:
        messagebox.showerror("Invalid Input", "Please enter valid numerical values.")

# Function to load JSON data into the GUI
def load_data():
    entry_length.delete(0, tk.END)
    entry_length.insert(0, str(rocket_data['rocket']['length']))
    
    entry_diameter.delete(0, tk.END)
    entry_diameter.insert(0, str(rocket_data['rocket']['diameter']))
    
    entry_wet_mass.delete(0, tk.END)
    entry_wet_mass.insert(0, str(rocket_data['rocket']['wet_mass']))
    
    entry_dry_mass.delete(0, tk.END)
    entry_dry_mass.insert(0, str(rocket_data['rocket']['dry_mass']))
    
    entry_fuel_mass.delete(0, tk.END)
    entry_fuel_mass.insert(0, str(rocket_data['propulsion']['fuel_mass']))
    
    entry_thrust.delete(0, tk.END)
    entry_thrust.insert(0, str(rocket_data['propulsion']['engines'][0]['thrust']))
    
    entry_burn_rate.delete(0, tk.END)
    entry_burn_rate.insert(0, str(rocket_data['propulsion']['engines'][0]['burn_rate']))
    
    entry_efficiency.delete(0, tk.END)
    entry_efficiency.insert(0, str(rocket_data['propulsion']['engines'][0]['efficiency']))
    
    entry_nozzle_diameter.delete(0, tk.END)
    entry_nozzle_diameter.insert(0, str(rocket_data['propulsion']['engines'][0]['nozzle_diameter']))
    
def run_simulation():
    subprocess.call(["g++", "-std=c++17", "-I", "src/", "src/main.cpp", "-o", "nova"])
    subprocess.call(["./nova"])
    subprocess.call(["python3", "screen.py"])

# Create the main window
root = tk.Tk()
root.title("NOVA")
root.configure(background='#040716')
root.geometry('470x650')
root.resizable(False, False)

# Font
font_style = ("Helvetica", 14)
label_font = ("Helvetica", 14, 'bold')
entry_font = ("Helvetica", 14)

frame = tk.Frame(root, bg='#040716')
frame.grid(padx=30, pady=30)

label_length = tk.Label(frame, text="Rocket Length (m):", font=label_font, bg='#040716', fg='white')
label_length.grid(row=0, column=0, sticky='w', pady=10)
entry_length = tk.Entry(frame, font=entry_font)
entry_length.grid(row=0, column=1, pady=10)

label_diameter = tk.Label(frame, text="Rocket Diameter (m):", font=label_font, bg='#040716', fg='white')
label_diameter.grid(row=1, column=0, sticky='w', pady=10)
entry_diameter = tk.Entry(frame, font=entry_font)
entry_diameter.grid(row=1, column=1, pady=10)

label_wet_mass = tk.Label(frame, text="Rocket Wet Mass (kg):", font=label_font, bg='#040716', fg='white')
label_wet_mass.grid(row=2, column=0, sticky='w', pady=10)
entry_wet_mass = tk.Entry(frame, font=entry_font)
entry_wet_mass.grid(row=2, column=1, pady=10)

label_dry_mass = tk.Label(frame, text="Rocket Dry Mass (kg):", font=label_font, bg='#040716', fg='white')
label_dry_mass.grid(row=3, column=0, sticky='w', pady=10)
entry_dry_mass = tk.Entry(frame, font=entry_font)
entry_dry_mass.grid(row=3, column=1, pady=10)

label_fuel_mass = tk.Label(frame, text="Fuel Mass (kg):", font=label_font, bg='#040716', fg='white')
label_fuel_mass.grid(row=4, column=0, sticky='w', pady=10)
entry_fuel_mass = tk.Entry(frame, font=entry_font)
entry_fuel_mass.grid(row=4, column=1, pady=10)

label_thrust = tk.Label(frame, text="Engine Thrust (N):", font=label_font, bg='#040716', fg='white')
label_thrust.grid(row=5, column=0, sticky='w', pady=10)
entry_thrust = tk.Entry(frame, font=entry_font)
entry_thrust.grid(row=5, column=1, pady=10)

label_burn_rate = tk.Label(frame, text="Burn Rate (kg/s):", font=label_font, bg='#040716', fg='white')
label_burn_rate.grid(row=6, column=0, sticky='w', pady=10)
entry_burn_rate = tk.Entry(frame, font=entry_font)
entry_burn_rate.grid(row=6, column=1, pady=10)

label_efficiency = tk.Label(frame, text="Engine Efficiency:", font=label_font, bg='#040716', fg='white')
label_efficiency.grid(row=7, column=0, sticky='w', pady=10)
entry_efficiency = tk.Entry(frame, font=entry_font)
entry_efficiency.grid(row=7, column=1, pady=10)

label_nozzle_diameter = tk.Label(frame, text="Nozzle Diameter (m):", font=label_font, bg='#040716', fg='white')
label_nozzle_diameter.grid(row=8, column=0, sticky='w', pady=10)
entry_nozzle_diameter = tk.Entry(frame, font=entry_font)
entry_nozzle_diameter.grid(row=8, column=1, pady=10)

# Create buttons for saving data and running simulation
button_save = tk.Button(root, text="Save Changes", font=("Helvetica", 14, 'bold'), command=save_data, bg="#28a745", fg="white", width=24)
button_save.grid(row=9, column=0, columnspan=2, pady=20)

button_load = tk.Button(root, text="Run Simulation", font=("Helvetica", 14, 'bold'), command=run_simulation, bg="#007bff", fg="white", width=24)
button_load.grid(row=10, column=0, columnspan=2, pady=20)

# Load initial data into the fields
load_data()

# Start the Tkinter event loop
root.mainloop()