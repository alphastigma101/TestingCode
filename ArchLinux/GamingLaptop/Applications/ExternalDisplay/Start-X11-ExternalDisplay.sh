#!/bin/bash
### GLOBAL VARIABLES ###
DISPLAY_STEAM="FALSE"
#DISPLAY_SOMETHING="FALSE"
#DISPLAY_SOMETHING="FALSE"
#DISPLAY_SOMETHING="FALSE"
#DISPLAY_SOMETHING="FALSE"
#DISPLAY_SOMETHING="FALSE"

# See if you can make xdotool more secure
if []; then # Check to see if StopExternalDisplay is running
  systemctl --user start StopExternalDisplay
fi 
while true; do
	# Use xev or xwininfo to find out the window and event details
	if [ xdotool search --class "Steam" behave %@ mouse-click ]; then # See if user has opened steam
    if [ $DISPLAY_STEAM -eq "FALSE" ]; then # Check DISPLAY_STEAM
      xinit /usr/bin/steam -- :1 vt$XDG_VTNR # Start a X11 server
      sleep 5 # Wait for it to load up
      DISPLAY_STEAM="TRUE"
    else 
      # Use this section to catch errors or debug 
      # make it so you can use a certain hot key to exit out
      # If the server crashes, get the full report of it crashing
      # To make things easier, you probably want to create a function that records the crashes because steam isn't the only application you should run in it's own server

    fi
  # Keep adding more if statements here and make sure that sleep 1 is not nested in any if statements!
  #
  #
    sleep 1
  fi
done
