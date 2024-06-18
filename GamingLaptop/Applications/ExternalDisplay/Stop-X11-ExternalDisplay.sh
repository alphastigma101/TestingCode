#!/bin/bash
# This script needs to be enabled and started by systemctl --user 
# See if you can make xdotool more secure
while true; do
  # Use xev or xwininfo to find out the window and event details
  if [ ! xdotool search --class "Steam" behave %@ mouse-click ]; then # See if user closed steam
    # Check and see if the event is gone and if so stop StartExternalDisplay
    systemctl --user stop StartExternalDisplay
    sleep 1 # Sleep and let the code below get executed
  else
    if []; then # Check StartExternalDisplay status
      # If it is not running
      systemctl --user start StartExternalDisplay
      break
    elif []; then 
      # if it running 
      sleep 1
      
		fi
	fi
done
