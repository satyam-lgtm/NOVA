import matplotlib.animation as animation
import matplotlib.patheffects as path_effects
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.gridspec import GridSpec
from matplotlib.patches import Arc, Circle, FancyArrowPatch, Rectangle


class EnhancedRocketVisualizer:
    def __init__(self, csv_file):
        self.data = pd.read_csv(csv_file)
        
        plt.style.use('dark_background')
        self.fig = plt.figure(figsize=(20, 11))
        self.fig.patch.set_facecolor('#040716')
        
        # Increase spacing between subplots and margins
        gs = GridSpec(6, 6, figure=self.fig)
        gs.update(wspace=0.5, hspace=1.0)  # Increased spacing
        
        # Adjust margins
        self.fig.subplots_adjust(top=0.92, bottom=0.08, left=0.08, right=0.95)
        
        self.ax_main = self.fig.add_subplot(gs[:, 2:])
        self.ax_telemetry = self.fig.add_subplot(gs[0:2, 0:2])
        self.ax_trajectory = self.fig.add_subplot(gs[2:4, 0:2])
        self.ax_mission = self.fig.add_subplot(gs[4:6, 0:2])
        
        self.telemetry_texts = {}
        self.mission_texts = {}
        
        # Get max altitude for scaling
        self.max_altitude = self.data['Altitude'].max()
        self.max_time = self.data['Time'].max()
        
        # Define rocket dimensions
        self.rocket_height = 50
        self.rocket_width = 12
        
        # Initialize trajectory line
        self.trajectory_line = None
        self.trajectory_point = None
        
        self.setup_theme()
        self.setup_displays()

    def setup_theme(self):
        for ax in [self.ax_main, self.ax_telemetry, self.ax_trajectory, self.ax_mission]:
            ax.set_facecolor('#040716')
            
            # Remove the grey background rectangles
            for spine in ax.spines.values():
                spine.set_color('#1E90FF')
                spine.set_alpha(0.3)
                spine.set_linewidth(1)
            
            ax.tick_params(colors='#4A4A8A', length=6, labelsize=10)

    def setup_displays(self):
        self.setup_main_view()
        self.setup_telemetry_display()
        self.setup_trajectory_view()
        self.setup_mission_status()

    def setup_main_view(self):
        self.ax_main.clear()
        
        ground_y = -100
        ground_width = 1000
        
        # Atmospheric glow
        gradient = np.linspace(0, 1, 100)
        for g in gradient:
            self.ax_main.add_patch(Arc(
                (0, ground_y), 
                ground_width + g*50, 
                (ground_width + g*50)/2,
                theta1=0, theta2=180, 
                color='#1E90FF', 
                alpha=0.02*(1-g),
                lw=1
            ))
        
        # Ground arc
        self.ax_main.add_patch(Arc(
            (0, ground_y), 
            ground_width, 
            ground_width/2,
            theta1=0, theta2=180, 
            color='#1E90FF', 
            alpha=0.5,
            lw=2
        ))
        
        self.ax_main.set_xlim(-ground_width/2, ground_width/2)
        self.ax_main.set_ylim(ground_y, self.max_altitude * 1.2)
        
        # Grid
        self.ax_main.grid(True, color='#1E90FF', alpha=0.1, linestyle='--', linewidth=0.5)
        
        # Altitude markers
        altitudes = np.arange(0, self.max_altitude * 1.2, 1000)
        for alt in altitudes:
            self.ax_main.axhline(y=alt, color='#1E90FF', alpha=0.1, linestyle=':')
            if alt > 0:
                self.ax_main.text(
                    -ground_width/2 * 0.95, 
                    alt, 
                    f'{alt/1000:.0f}km',
                    color='#4A4A8A',
                    fontsize=10
                )
        
        self.ax_main.set_xticks([])
        self.ax_main.set_yticks([])

    def setup_telemetry_display(self):
        self.ax_telemetry.clear()
        
        self.ax_telemetry.text(
            0.5, 1.1,  # Adjusted position
            'TELEMETRY DATA',
            color='#00FFFF',
            fontsize=14,
            fontweight='bold',
            family='monospace',
            ha='center',
            transform=self.ax_telemetry.transAxes
        )
        
        params = [
            ('ALTITUDE', 'm', '{:,.1f}'),
            ('VELOCITY', 'm/s', '{:.1f}'),
            ('ACCELERATION', 'm/s²', '{:.1f}'),
            ('MASS', 'kg', '{:,.0f}'),
            ('AIR DENSITY', 'kg/m³', '{:.3f}'),
            ('TEMPERATURE', 'K', '{:.1f}'),
            ('MACH', '', '{:.2f}')
        ]
        
        for i, (param, unit, _) in enumerate(params):
            y_pos = 0.85 - i * 0.11
            # Parameter label
            self.ax_telemetry.text(
                0.05, y_pos,
                param,
                color='#9370DB',
                fontfamily='monospace',
                fontsize=12,
                fontweight='bold'
            )
            # Value display
            self.telemetry_texts[param] = self.ax_telemetry.text(
                0.45, y_pos,
                f"0 {unit}",
                color='#00FF00',
                fontfamily='monospace',
                fontsize=12,
                fontweight='bold'
            )
        
        self.ax_telemetry.set_xticks([])
        self.ax_telemetry.set_yticks([])

    def setup_trajectory_view(self):
        self.ax_trajectory.clear()
        
        # Title with adjusted position
        self.ax_trajectory.text(
            0.5, 1.1,  # Adjusted position
            'ALTITUDE PROFILE',
            color='#00FFFF',
            fontsize=14,
            fontweight='bold',
            family='monospace',
            ha='center',
            transform=self.ax_trajectory.transAxes
        )
        
        # Set up axes
        self.ax_trajectory.grid(True, color='#1E90FF', alpha=0.2, linestyle='--')
        self.ax_trajectory.set_xlabel('Time (s)', color='#4A4A8A', fontsize=12)
        self.ax_trajectory.set_ylabel('Altitude (km)', color='#4A4A8A', fontsize=12)
        
        # Set fixed limits for the plot
        self.ax_trajectory.set_xlim(0, self.max_time)
        self.ax_trajectory.set_ylim(0, self.max_altitude/1000 * 1.1)
        
        # Make grid more visible
        self.ax_trajectory.grid(True, color='#1E90FF', alpha=0.2, linestyle='--', linewidth=0.5)
        
        # Initialize the trajectory line
        self.trajectory_line, = self.ax_trajectory.plot([], [], color='#00FF00', linewidth=2)
        self.trajectory_point, = self.ax_trajectory.plot([], [], 'ro', markersize=8)
        
        # Customize tick parameters
        self.ax_trajectory.tick_params(axis='both', colors='#4A4A8A', labelsize=10)
        
        # Make spines more visible
        for spine in self.ax_trajectory.spines.values():
            spine.set_color('#1E90FF')
            spine.set_alpha(0.3)

    def setup_mission_status(self):
        self.ax_mission.clear()
        
        self.ax_mission.text(
            0.5, 1.1,  # Adjusted position
            'MISSION STATUS',
            color='#00FFFF',
            fontsize=14,
            fontweight='bold',
            family='monospace',
            ha='center',
            transform=self.ax_mission.transAxes
        )
        
        # Mission timer
        self.mission_texts['time'] = self.ax_mission.text(
            0.5, 0.7,
            'T+ 00:00:00',
            color='#00FF00',
            fontfamily='monospace',
            fontsize=20,
            fontweight='bold',
            ha='center'
        )
        
        # Phase indicator
        self.mission_texts['phase'] = self.ax_mission.text(
            0.5, 0.4,
            'PHASE: PRE-LAUNCH',
            color='#00FF00',
            fontfamily='monospace',
            fontsize=16,
            fontweight='bold',
            ha='center'
        )
        
        # Status message
        self.mission_texts['status'] = self.ax_mission.text(
            0.5, 0.2,
            'SYSTEMS CHECK',
            color='#00FF00',
            fontfamily='monospace',
            fontsize=14,
            ha='center'
        )
        
        self.ax_mission.set_xticks([])
        self.ax_mission.set_yticks([])

    def draw_rocket(self, x, y, velocity):
        if velocity < 0:  # Descending
            start = (x, y + self.rocket_height)
            end = (x, y)
        else:  # Ascending or stationary
            start = (x, y)
            end = (x, y + self.rocket_height)
        
        rocket = FancyArrowPatch(
            start, end,
            color='white',
            linewidth=3,
            mutation_scale=self.rocket_width,
            path_effects=[
                path_effects.Stroke(linewidth=6, foreground='#00FF00', alpha=0.3),
                path_effects.Normal()
            ]
        )
        
        # Engine flame
        if abs(velocity) > 0:
            flame_colors = ['#FF4500', '#FF8C00', '#FFD700']
            for i, color in enumerate(flame_colors):
                flame_start = end if velocity < 0 else start
                flame_length = (15-i*4)
                flame_end = (x, y + self.rocket_height + flame_length) if velocity < 0 else (x, y - flame_length)
                
                flame = FancyArrowPatch(
                    flame_start,
                    flame_end,
                    color=color,
                    alpha=0.7-i*0.2,
                    linewidth=4-i,
                    mutation_scale=self.rocket_width-(i*2)
                )
                self.ax_main.add_patch(flame)
        
        return rocket

    def update(self, frame):
        current_data = self.data.iloc[frame]
        prev_altitude = self.data.iloc[frame-1]['Altitude'] if frame > 0 else current_data['Altitude']
        
        # Update main view
        self.ax_main.clear()
        self.setup_main_view()
        
        # Draw rocket
        rocket = self.draw_rocket(0, current_data['Altitude'], current_data['Velocity_Magnitude'])
        self.ax_main.add_patch(rocket)
        
        # Update trajectory plot
        plot_data = self.data.iloc[:frame+1]
        self.trajectory_line.set_data(plot_data['Time'], plot_data['Altitude']/1000)
        self.trajectory_point.set_data([current_data['Time']], [current_data['Altitude']/1000])
        
        # Update telemetry
        value_map = {
            'ALTITUDE': current_data['Altitude'],
            'VELOCITY': current_data['Velocity_Magnitude'],
            'ACCELERATION': current_data['Acceleration_Magnitude'],
            'MASS': current_data['Mass'],
            'AIR DENSITY': current_data['Air_Density'],
            'TEMPERATURE': current_data['Temperature'],
            'MACH': current_data['Mach_Number']
        }
        
        for param, text_obj in self.telemetry_texts.items():
            if param == 'ALTITUDE':
                formatted_value = f"{value_map[param]:,.1f}"
            else:
                formatted_value = f"{value_map[param]:.1f}"
            text_obj.set_text(formatted_value)
        
        # Update mission time
        total_seconds = int(current_data['Time'])
        hours = total_seconds // 3600
        minutes = (total_seconds % 3600) // 60
        seconds = total_seconds % 60
        self.mission_texts['time'].set_text(f"T+ {hours:02d}:{minutes:02d}:{seconds:02d}")
        
        # Update phase based on velocity and altitude change
        if current_data['Altitude'] < prev_altitude and current_data['Velocity_Magnitude'] > 0:
            phase = "DESCENT PHASE"
            status = "CONTROLLED DESCENT"
        elif current_data['Velocity_Magnitude'] > 0:
            phase = "ASCENT PHASE"
            status = "NOMINAL FLIGHT PATH"
        elif current_data['Velocity_Magnitude'] < 0:
            phase = "DESCENT PHASE"
            status = "CONTROLLED DESCENT"
        else:
            phase = "PRE-LAUNCH"
            status = "SYSTEMS CHECK"
            
        self.mission_texts['phase'].set_text(f"PHASE: {phase}")
        self.mission_texts['status'].set_text(status)
        
        return [self.trajectory_line, self.trajectory_point, 
                *self.telemetry_texts.values(),
                *self.mission_texts.values(),
                rocket]

    def animate(self):
        ani = animation.FuncAnimation(
            self.fig,
            self.update,
            frames=len(self.data),
            interval=1000,  # Updated to 1 second interval
            blit=True
        )
        plt.show()

if __name__ == "__main__":
    visualizer = EnhancedRocketVisualizer('flight_data.csv')
    visualizer.animate()
