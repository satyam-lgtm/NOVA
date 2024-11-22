# NOVA (Navigation in Orbital Velocity Analyzer)

## Overview
NOVA is a sophisticated rocket flight simulation system designed to analyze and visualize the complex dynamics of rocket launches and atmospheric flight. The simulator provides high-fidelity modeling of various physical phenomena affecting rocket flight, making it valuable for preliminary mission planning and educational purposes.

![NOVA]("assets/image.png" "Optional title")

The system models:
- Gravitational forces with real-time altitude compensation
- Atmospheric effects through multiple layers
- Complex aerodynamic interactions
- Multi-engine propulsion systems
- Real-time fuel consumption and mass changes
- Thrust vectoring via gimbal control
- Flight path visualization and comprehensive data logging

## Features In-Depth

### 1. Physics Simulation Engine

#### Gravitational Model
- Implements Newton's universal law of gravitation
- Accounts for gravitational field variations with altitude
- Uses Earth's actual mass (5.972e24 kg) and radius (6,371 km)
- Real-time gravitational force calculations based on position

#### Atmospheric Model
- Multi-layer atmospheric simulation
- Temperature gradient modeling (-6.5°C/km lapse rate)
- Pressure calculations using barometric formula
- Density variations based on ideal gas law
- Scale height consideration (7,400m)

#### Aerodynamics System
- Comprehensive drag modeling:
  - Subsonic regime (Mach < 0.8): Constant drag coefficient
  - Transonic regime (0.8 ≤ Mach ≤ 1.2): Linear interpolation
  - Supersonic regime (Mach > 1.2): Wave drag consideration
- Lift calculations based on angle of attack
- Dynamic pressure effects
- Reynolds number considerations
- Real-time aerodynamic coefficient updates

### 2. Advanced Propulsion System

#### Engine Management
- Multiple engine support with individual characteristics
- Real-time thrust calculations with altitude compensation
- Specific impulse variations with altitude
- Throttle control (0-100%)

#### Fuel System
- Precise fuel mass tracking
- Real-time consumption calculations
- Mass flow rate modeling
- Center of mass updates based on fuel usage

#### Thrust Vectoring
- Gimbal control system (±5° range)
- Real-time thrust direction updates
- Independent X and Y axis control
- Automatic thrust direction optimization

### 3. Data Analysis and Output

#### Real-time Data Collection
- High-precision time stamping
- Comprehensive state vector logging:
  - Position (x, y, z coordinates)
  - Velocity components
  - Acceleration vectors
  - Current mass
  - Fuel status
  - Engine parameters
  - Atmospheric conditions
  - Flight dynamics

#### Data Output Format
Flight data is saved to `flight_data.csv` with columns:
- Time (seconds)
- Altitude (meters)
- Velocity (m/s)
- Acceleration (m/s²)
- Mass (kg)
- Fuel Ratio (0-1)

## Requirements

### System Requirements
- C++17 compatible compiler (g++ recommended)
- Python 3.x for visualization
- Terminal with ANSI color support
- Minimum 1GB RAM
- x86_64 processor architecture

### Software Dependencies
- Standard C++ libraries
- Python libraries (for visualization):
  - matplotlib
  - pandas
  - numpy

## Installation & Setup

1. Clone the repository:
```bash
git clone https://github.com/thevoxium/NOVA.git
cd nova
```

2. Ensure you have the required compilers and Python:
```bash
g++ --version  # Should be 7.0 or higher
python3 --version  # Should be 3.6 or higher
```

3. Install Python dependencies:
```bash
pip3 install matplotlib pandas numpy
```

## Running NOVA

### Basic Execution
Run the simulation with default parameters:
```bash
g++ -std=c++17 -I src/ src/main.cpp -o nova && ./nova && python3 screen.py
```

This command:
1. Compiles the C++ code with C++17 standard
2. Includes headers from src/ directory
3. Creates executable named 'nova'
4. Runs the simulation
5. Launches the Python visualization script

### Default Configuration
The simulation starts with these parameters:
- Initial altitude: 100m above sea level
- Rocket specifications:
  - Length: 20 meters
  - Diameter: 2 meters
  - Wet mass: 5000 kg
  - Dry mass: 2000 kg
- Engine specifications:
  - Maximum thrust: 100 kN
  - Specific impulse: 300 seconds
  - Throat area: 0.5 m²
  - Expansion ratio: 20:1
- Initial conditions:
  - Zero initial velocity
  - Vertical orientation
  - Full fuel tanks
  - Sea level atmospheric conditions

## Modifying Simulation Parameters

### Rocket Configuration
Edit `main.cpp` to modify rocket parameters:
```cpp
RocketBody rocket(
    20.0,    // Length (m)
    2.0,     // Diameter (m)
    5000.0,  // Wet mass (kg)
    2000.0   // Dry mass (kg)
);
```

### Engine Settings
Adjust engine parameters in `main.cpp`:
```cpp
propulsion.addEngine(
    100000.0,  // Max thrust (N)
    300.0,     // Specific impulse (s)
    0.5,       // Throat area (m²)
    20.0       // Expansion ratio
);
```

### Initial Conditions
Modify the initial state in `main.cpp`:
```cpp
State initialState(
    Vec3(Constants::EARTH_RADIUS + 100.0, 0, 0),  // Position
    Vec3(0, 0, 0),                                // Velocity
    Vec3(),                                       // Acceleration
    rocket.getMass(),                             // Mass
    0.0                                           // Initial time
);
```

## Understanding Output

### Console Output
During simulation, the console displays:
- Real-time force calculations
- Current altitude
- Velocity magnitude
- Remaining fuel percentage
- Engine status
- Error messages (if any)

### Data Visualization
The Python script (`screen.py`) generates plots showing:
- Altitude vs. Time
- Velocity vs. Time
- Acceleration vs. Time
- Mass vs. Time
- Fuel consumption rate
- Trajectory visualization

## Known Limitations

1. Atmospheric Model:
   - No wind effects
   - Simplified temperature gradient
   - No weather conditions

2. Aerodynamics:
   - Basic drag coefficient model
   - Simplified lift calculations
   - No lateral aerodynamic effects

3. Propulsion:
   - Ideal rocket equation assumptions
   - No nozzle flow separation modeling
   - Simplified specific impulse calculations

4. Physics:
   - No Earth rotation effects
   - No Coriolis force
   - No relativistic effects

## Troubleshooting

### Common Issues

1. Compilation Errors:
   ```
   Solution: Ensure C++17 compiler is installed and properly configured
   ```

2. Python Visualization Errors:
   ```
   Solution: Verify Python dependencies are installed correctly
   ```

3. Data File Access:
   ```
   Solution: Check write permissions in the current directory
   ```

## Future Improvements

1. Enhanced Physics:
   - Earth rotation effects
   - Advanced atmospheric modeling
   - Wind effects and turbulence

2. Additional Features:
   - Real-time 3D visualization
   - Multiple stage support
   - Flight control systems
   - Mission planning interface

3. Performance Optimizations:
   - Parallel computation support
   - GPU acceleration
   - Adaptive time stepping



## License
MIT

## Acknowledgments
- Physical constants from NASA technical documents
- Atmospheric model based on U.S. Standard Atmosphere
- Numerical methods inspired by modern spacecraft dynamics texts
